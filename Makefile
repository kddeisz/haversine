# Run the haversine binary
run: compile
	bin/haversine test.sqlite3

# Compile the haversine binary
compile: init
	mkdir -p bin
	gcc -Wall -lsqlite3 src/main.c src/haversine.c -o bin/haversine

# Create the test.sqlite3 file
init:
	sqlite3 test.sqlite3 < init.sql
