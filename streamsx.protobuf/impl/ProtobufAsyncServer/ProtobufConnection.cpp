#include "ProtobufConnection.hpp"
#include <stdio.h>
#include <string>
#include <set>

using streams_boost::asio::ip::tcp;

std::set<ProtobufConnection::pointer> connections;

/**
 * public:
 */

ProtobufConnection::pointer ProtobufConnection::create(streams_boost::asio::io_service & io_service, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue) {
    ProtobufConnection::pointer newPointer = pointer(new ProtobufConnection(io_service, pthreadMutex, pthreadCond, tupleQueue));
    return newPointer;
}

tcp::socket & ProtobufConnection::socket() {
    return socket_;
}

void ProtobufConnection::start() {
    //printf("Starting connection\n");
    connections.insert(shared_from_this());
    streams_boost::asio::async_read(socket_,
        streams_boost::asio::buffer(&protobufHeader.buffer, 4),
        streams_boost::bind(&ProtobufConnection::handleReadHeader, shared_from_this(), streams_boost::asio::placeholders::error));
}

/**
 * private:
 */

ProtobufConnection::ProtobufConnection(streams_boost::asio::io_service & io_service, pthread_mutex_t * pthreadMutex, pthread_cond_t * pthreadCond, std::queue<std::string> * tupleQueue)
: socket_(io_service) {
    this->pthreadMutex = pthreadMutex;
    this->pthreadCond = pthreadCond;
    this->tupleQueue = tupleQueue;
}

void ProtobufConnection::handleReadHeader(const streams_boost::system::error_code & error) {
    if (!error) {
        protobufData = new unsigned char[protobufHeader.size];
        //printf("New record of length: %d\n", protobufHeader.size);
        streams_boost::asio::async_read(socket_,
            streams_boost::asio::buffer(protobufData, protobufHeader.size),
            streams_boost::bind(&ProtobufConnection::handleReadProto, shared_from_this(), streams_boost::asio::placeholders::error));
    } else {
        socket_.close();
        connections.erase(shared_from_this());
        //printf("Connection done\n");
    }
}

void ProtobufConnection::handleReadProto(const streams_boost::system::error_code & error) {
    if (!error) {
        //printf("Read the data\n");
        //pthread_mutex_lock(pthreadMutex);
        //printf("Copying %d bytes\n", protobufHeader.size);
        tupleQueue->push(std::string((char *) protobufData, (size_t) protobufHeader.size));
        //printf("tupleQueue size: %d\n", tupleQueue->size());
        //pthread_mutex_unlock(pthreadMutex);
        //printf("Mutex released\n");
        pthread_cond_signal(pthreadCond);

        delete protobufData;
        protobufData = NULL;

        start();
    } else {
        socket_.close();
        connections.erase(shared_from_this());
        //printf("Error reading serialized protobuf stream!\n");
    }
}
