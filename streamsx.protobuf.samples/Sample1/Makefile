.PHONY: all clean

APPDIR = $(shell basename `pwd`)
PROTOFILE = "addressbook.proto"
MAIN_COMPOSITE = "Sample1"
all:
	../../streamsx.protobuf/bin/spl-schema-from-protobuf . ${PROTOFILE}
	sc -c -M ${MAIN_COMPOSITE} -t ../../streamsx.protobuf:$(STREAMS_SPLPATH) -w -Wl,-rpath="'\$$\$$ORIGIN/../toolkits/$(APPDIR)/impl/lib'"
	#sc -M ${MAIN_COMPOSITE} -t ../../streamsx.protobuf:$(STREAMS_SPLPATH) -w -Wl,-rpath="'\$$\$$ORIGIN/../toolkits/$(APPDIR)/impl/lib'"

clean:
	rm -rf output
	rm -f toolkit.xml
	rm -rf impl/lib
	rm -rf tutorial
	rm -rf impl
