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

#include "ProtobufConnection.hpp"

using streams_boost::asio::ip::tcp;

class ProtobufServer {
    public:
        ProtobufServer(streams_boost::asio::io_service & io_service, const tcp::endpoint & endpoint, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue);

        void startAccept();
        void handleAccept(ProtobufConnection::pointer session, const streams_boost::system::error_code & error);

    private:
        streams_boost::asio::io_service & io_service_;
        tcp::acceptor acceptor_;

        pthread_mutex_t * pthreadMutex;
        pthread_cond_t * pthreadCond;

        std::queue<std::string> * tupleQueue;
};
