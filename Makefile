# Run the haversine binary
test: compile
	test/haversine test.sqlite3

# Compile the haversine binary
compile: init
	mkdir -p bin
	gcc -Wall -lsqlite3 test/main.c src/haversine.c -o test/haversine

# Create the test.sqlite3 file
init:
	sqlite3 test.sqlite3 < test/seed.sql
