cmake_minimum_required(VERSION 2.8)
project(OmegleSpy CXX)

#Boost
set(BOOST_INCLUDEDIR "/Users/hocheung20/boost/Boost_1_55_0/")
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIR})

#cpp-netlib
set(cppnetlib_INCLUDE_DIR /Users/hocheung20/cpp-netlib-0.10.1/src)
set(cppnetlib_LIBRARY_DIR /Users/hocheung20/cpp-netlib-0.10.1/bin/libs/network/src)
set(cppnetlib_LIBRARIES
    ${cppnetlib_LIBRARY_DIR}/libcppnetlib-client-connections.a
    ${cppnetlib_LIBRARY_DIR}/libcppnetlib-server-parsers.a
    ${cppnetlib_LIBRARY_DIR}/libcppnetlib-uri.a)
include_directories(${cppnetlib_INCLUDE_DIR})

#openssl
find_package(OpenSSL REQUIRED)

#jsoncpp
find_library(jsoncpp_LIBRARY NAMES json_linux-gcc-4.2.1_libmt PATHS /Users/hocheung20/jsoncpp-0.5.0/src/libs/linux-gcc-4.2.1)
include_directories(/Users/hocheung20/jsoncpp-0.5.0/src/include)

#Sources
set(OMEGLESPY_SRCS
    main.cpp
    OmegleSession.cpp
    OmegleAPI.cpp)

#Headers
set(OMEGLESPY_HDRS
    OmegleSession.h
    OmegleAPI.h)

#Bundle
add_executable(OmegleSpy ${OMEGLESPY_SRCS} ${OMEGLESPY_HDRS})
target_link_libraries(OmegleSpy ${OPENSSL_LIBRARIES} ${cppnetlib_LIBRARIES} ${jsoncpp_LIBRARY} ${Boost_LIBRARIES})
install(TARGETS OmegleSpy DESTINATION ${CMAKE_BINARY_DIR})