.PHONY: all build-parse-sample tcp-sample clean

all: build-parse-sample tcp-sample sample-1

build-parse-sample:
	make -C BuildParseSample

tcp-sample:
	make -C TCPSample

sample-1:
	make -C Sample1

clean:
	make -C BuildParseSample clean
	make -C TCPSample clean
	make -C Sample1 clean
