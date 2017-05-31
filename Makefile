run: compile
	./dist test.sqlite3

compile: init
	gcc -Wall -lsqlite3 main.c haversine.c -o dist

init:
	sqlite3 test.sqlite3 < init.sql
