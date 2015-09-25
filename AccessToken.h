
#pragma once

#include <map>

namespace AccessTokenService
{

class AccessToken
{
public:
    const std::string GetToken() const noexcept;
    void RefreshToken() noexcept;
private:
    std::string m_accessToken;
};

class AccessTokenManage
{
public:

private:
    // rehash maybe will result in disaster, so we use std::map temporary
    std::map<std::string, std::string> m_accessTokens;
};

}
