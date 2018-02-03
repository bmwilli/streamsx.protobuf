#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <streams_boost/bind.hpp>
#include <streams_boost/shared_ptr.hpp>
#include <streams_boost/enable_shared_from_this.hpp>
#include <streams_boost/asio.hpp>

#include <pthread.h>
#include <queue>

using streams_boost::asio::ip::tcp;

class ProtobufConnection : public streams_boost::enable_shared_from_this<ProtobufConnection> {
    public:
        typedef streams_boost::shared_ptr<ProtobufConnection> pointer;
        static pointer create(streams_boost::asio::io_service & io_service, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue);

        tcp::socket & socket();
        void start();

    private:
        ProtobufConnection(streams_boost::asio::io_service & io_service, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue);

        void handleReadHeader(const streams_boost::system::error_code & error);
        void handleReadProto(const streams_boost::system::error_code & error);

        tcp::socket socket_;

        union {
            unsigned char buffer[4];
            uint32_t size;
        } protobufHeader;
        unsigned char * protobufData;

        pthread_mutex_t * pthreadMutex;
        pthread_cond_t * pthreadCond;

        std::queue<std::string> * tupleQueue;
};
