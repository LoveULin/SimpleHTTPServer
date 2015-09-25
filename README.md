# SimpleHTTPServer
Building by Proxygen and LibcURL

Building:
    g++ -std=c++11 AccessToken.cc AccessTokenHandler.cc OurCURL.cc main.cc -lcurl -lproxygenhttpserver -lfolly -lglog -lgflags -pthread
