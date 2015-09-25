
#pragma once

#include <string>
#include <memory>
#include <boost/thread/detail/singleton.hpp>
#include <boost/property_tree/ptree.hpp>
#include <curl/curl.h>

class OurCURL
{
public:
    // writer callback for LIBCURL
    typedef std::size_t (*writeFunc)(char *ptr, size_t size, size_t nmemb, void *userdata);

public:
    // no default cons
    explicit OurCURL(const char *theURL);
    // template function for value
    template<typename T>
    const CURLcode AddOption(CURLoption option, T value) noexcept;
    // perform the request
    const CURLcode Perform() noexcept;
    // get the data from response
    const std::string GetData(const char *key) const noexcept;

private:
    // singleton class for global init LIBCURL
    class GlobalCURL
    {
    public:
        GlobalCURL() noexcept
        {
            CURLcode ret(curl_global_init(CURL_GLOBAL_ALL));
            if (CURLE_OK != ret) {
                m_status = false;
            }
        }
        const bool GetStatus() const noexcept
        {
            return m_status;
        }
    private:
        bool m_status {true};
    };
    typedef boost::detail::thread::singleton<GlobalCURL> STForGlobalCURL;

    // deleter for unique_ptr
    class Deleter
    {
    public:
        void operator()(CURL *pointer)
        {
            if (nullptr != pointer) {
                curl_easy_cleanup(pointer);
            }
        }
    };
    std::unique_ptr<CURL, Deleter> m_eHandle;
    std::string m_resData;
    writeFunc m_theWriteFunc {nullptr};
    boost::property_tree::ptree m_pt;
};

