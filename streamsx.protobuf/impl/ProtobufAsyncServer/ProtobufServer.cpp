//
// async_server.cpp
//

#include "ProtobufServer.hpp"
#include <stdio.h>

using streams_boost::asio::ip::tcp;

ProtobufServer::ProtobufServer(streams_boost::asio::io_service & io_service, const tcp::endpoint & endpoint, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue)
    : io_service_(io_service), acceptor_(io_service, endpoint) {
    this->pthreadMutex = pthreadMutex;
    this->pthreadCond = pthreadCond;
    this->tupleQueue = tupleQueue;

    startAccept();

    //printf("Setting pthreadMutex in server to %d\n", (unsigned long) pthreadMutex);
}

void ProtobufServer::startAccept() {
    ProtobufConnection::pointer newConnection = ProtobufConnection::create(acceptor_.get_io_service(), pthreadMutex, pthreadCond, tupleQueue);

    acceptor_.async_accept(newConnection->socket(),
        streams_boost::bind(&ProtobufServer::handleAccept, this, newConnection, streams_boost::asio::placeholders::error));
}

void ProtobufServer::handleAccept(ProtobufConnection::pointer session, const streams_boost::system::error_code & error) {
    if (!error) {
        //printf("Got connection!\n");
        session->start();
    } else {
        //printf("Error accepting connection!\n");
    }

    startAccept();
}
