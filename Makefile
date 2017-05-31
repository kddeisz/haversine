run_test: test/haversine
	test/haversine test/test.sqlite3

test/haversine: test/test.sqlite3
	gcc -Wall -lsqlite3 test/main.c src/haversine.c -o test/haversine

test/test.sqlite3:
	sqlite3 test/test.sqlite3 < test/seed.sql
