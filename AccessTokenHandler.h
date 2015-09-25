
#pragma once

#include <proxygen/httpserver/RequestHandler.h>

namespace AccessTokenService
{

class AccessToken;

class AccessTokenHandler : public proxygen::RequestHandler
{
public:
    explicit AccessTokenHandler(AccessToken *token);
    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;

private:
    AccessToken *m_token {nullptr};
    std::unique_ptr<folly::IOBuf> body_;
};

}

