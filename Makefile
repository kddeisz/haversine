run: compile
	bin/haversine test.sqlite3

compile: init
	mkdir -p bin
	gcc -Wall -lsqlite3 src/main.c src/haversine.c -o bin/haversine

init:
	sqlite3 test.sqlite3 < init.sql
