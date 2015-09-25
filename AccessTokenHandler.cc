
#include <cassert>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <curl/curl.h>
#include "OurCURL.h"
#include "AccessTokenHandler.h"
#include "AccessToken.h"

namespace
{

const char * const sAppID("");
const char * const sSecret("");
const char * const sTheURL("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=");

}

namespace AccessTokenService
{

AccessTokenHandler::AccessTokenHandler(AccessToken *token) : m_token(token) {}

void AccessTokenHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    if (body_) {
        body_->prependChain(std::move(body));
    }
    else {
        body_ = std::move(body);
    }
}

void AccessTokenHandler::onEOM() noexcept {
    std::string theURL(sTheURL);
    theURL += sAppID; 
    theURL += "&secret=";
    theURL += sSecret;
    try {
        OurCURL curl(theURL.c_str());
        CURLcode ret(curl.Perform());
        // temp code for simplify the logic 
        assert(CURLE_OK == ret);
        // now we can retrieve the data
        std::cout << curl.GetData("access_token") << std::endl;
        std::cout << curl.GetData("expires_in") << std::endl;
    }
    catch(std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    m_token->RefreshToken();
    proxygen::ResponseBuilder(downstream_)
        .status(200, "OK")
        .header("Access-token", m_token->GetToken())
        .body(std::move(body_))
        .sendWithEOM();
}

void AccessTokenHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
    // handler doesn't support upgrades
}

void AccessTokenHandler::onUpgrade(proxygen::UpgradeProtocol protocol) noexcept {
    // handler doesn't support upgrades
}

void AccessTokenHandler::requestComplete() noexcept {
    delete this;
}

void AccessTokenHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}

}

