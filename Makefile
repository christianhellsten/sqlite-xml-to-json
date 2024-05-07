SQLITE_FOLDER = sqlite-amalgamation-3450300
CC = gcc
CFLAGS = -g
INCLUDES = -I/opt/homebrew/include/json-c -I/opt/homebrew/include -I$(SQLITE_FOLDER)/
LIBS = -L/opt/homebrew/lib -lsqlite3 -ljson-c -lxml2

extension: sqlite3 xml_to_json.dylib

install:
	brew install gcc json-c

test: sqlite3 xml_to_json.dylib
	./dist/sqlite3 -cmd ".load ./dist/xml_to_json.dylib" -cmd "select xml_to_json('<A id=\"1\"><B namespace:id=\"2\">C</B></A>');" -cmd ".exit"

sqlite3: $(SQLITE_FOLDER)/shell.c $(SQLITE_FOLDER)/sqlite3.c
	$(CC) -DSQLITE_THREADSAFE=0 $(SQLITE_FOLDER)/shell.c $(SQLITE_FOLDER)/sqlite3.c -ldl -lm -o dist/sqlite3

xml_to_json.dylib: xml_to_json.c extension.c xml_to_json.h
	$(CC) $(CFLAGS) -shared -o dist/xml_to_json.dylib xml_to_json.c extension.c $(INCLUDES) $(LIBS)

clean:
	rm -rf dist/*

XML_FILES := $(shell ls fixtures/*.xml)
JSON_FILES := $(patsubst fixtures/%.xml, fixtures/%.json, $(XML_FILES))

test_xml_to_json: convert_xml_to_json_test $(JSON_FILES)

fixtures/%.json: fixtures/%.xml convert_xml_to_json_test
	./convert_xml_to_json_test < $< > $@.temp
	@if diff -q $@ $@.temp >/dev/null; then \
		echo "Test passed: $<"; \
	else \
		echo "Test failed: $<"; \
	fi
	@rm -f $@.temp

convert_xml_to_json_test: convert_xml_to_json_test.c xml_to_json.c xml_to_json.h
	gcc -o convert_xml_to_json_test convert_xml_to_json_test.c xml_to_json.c -I/opt/homebrew/include/json-c -L/opt/homebrew/lib -lsqlite3 -ljson-c -lxml2 -I/opt/homebrew/include/json-c -I/opt/homebrew/include -Isqlite-amalgamation-3450300/ -L/opt/homebrew/lib

.PHONY: test_xml_to_json
