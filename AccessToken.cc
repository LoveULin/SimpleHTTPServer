
#include <string>
#include <sstream>
#include <random>
#include "AccessToken.h"

namespace
{
    std::default_random_engine gen;
}

namespace AccessTokenService
{

const std::string AccessToken::GetToken() const noexcept
{
    return m_accessToken;
}

void AccessToken::RefreshToken() noexcept
{
    // a fake access token
    std::uniform_int_distribution<int> dist(1, 10000); 
    std::ostringstream ss;
    ss << dist(gen);
    // set the access token
    m_accessToken.assign(ss.str());  
}

}
