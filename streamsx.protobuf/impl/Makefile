.PHONY: all protobuf-server clean

all: protobuf-server

protobuf-server: ProtobufAsyncServer/ProtobufServer.cpp ProtobufAsyncServer/ProtobufConnection.cpp
	mkdir -p lib
	mkdir -p include
	g++ -c -fPIC ProtobufAsyncServer/*.cpp -I include/ProtobufAsyncServer -I ${STREAMS_INSTALL}/ext/include -L$(STREAMS_INSTALL)/ext/lib -lboost
	g++ ProtobufServer.o ProtobufConnection.o -shared -o lib/libprotobufserver.so -L$(STREAMS_INSTALL)/ext/lib -lstreams_boost_system -lstreams_boost_thread -lstreams_boost_regex -lstreams_boost_serialization
	rm ProtobufServer.o ProtobufConnection.o

clean:
	rm -f ProtobufServer.o ProtobufConnection.o lib/libprotobufserver.so
	rm -rf lib
