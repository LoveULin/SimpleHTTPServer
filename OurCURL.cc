
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "OurCURL.h"

namespace
{

// the default writer function for curl
const std::size_t defaultWriteFunc(char *ptr, size_t size, size_t nmemb, void *userData)
{
    // maybe 0 size of data
    if ((0 == size) || (0 == nmemb)) {
        return 0;
    }
    std::string * const theData(static_cast<std::string*>(userData));
    (void)theData->append(ptr, (size * nmemb));
    return (size * nmemb);
}

}

OurCURL::OurCURL(const char *theURL)
{
    bool ret(false);
    do {
        // singleton will do the global init
        if (!STForGlobalCURL::instance().GetStatus()) {
            break; 
        }
        // unique_ptr will do the jobs of cleanup
        std::unique_ptr<CURL, Deleter> eHandle(curl_easy_init(), Deleter());
        if (!eHandle) {
            break;
        }
        // set the URL
        CURLcode cCode(curl_easy_setopt(eHandle.get(), CURLOPT_URL, theURL));
        if (CURLE_OK != cCode) {
            break;
        }
        // set the default write function and data for user 
        cCode = curl_easy_setopt(eHandle.get(), CURLOPT_WRITEFUNCTION, defaultWriteFunc);
        if (CURLE_OK != cCode) {
            break;
        }
        cCode = curl_easy_setopt(eHandle.get(), CURLOPT_WRITEDATA, &m_resData);
        if (CURLE_OK != cCode) {
            break;
        }
        // everything is done, save the handle
        m_eHandle.swap(eHandle);
        ret = true;
    } while (false);
    if (!ret) {
        // otherwise, throw a exception
        throw std::runtime_error("Create easy handle of curl FAILED");
    }
}

template <typename T>
const CURLcode OurCURL::AddOption(CURLoption option, T value) noexcept
{
    // just forward the interface just now
    return curl_easy_setopt(m_eHandle.get(), option, value);   
}

const CURLcode OurCURL::Perform() noexcept
{
    // clear the data of last perform first
    m_resData.clear();
    // go!!
    CURLcode ret(curl_easy_perform(m_eHandle.get()));
    if ((CURLE_OK == ret) && (!m_resData.empty())) {
        // if succeed, read the json response
        std::stringstream ss(m_resData);
        boost::property_tree::read_json<boost::property_tree::ptree>(ss, m_pt);
    }
    return ret;
}

const std::string OurCURL::GetData(const char *key) const noexcept
{
    // read data from the json property_tree(by BOOST)
    return m_pt.get<std::string>(key);
}

