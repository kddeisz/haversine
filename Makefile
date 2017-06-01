run_java: java/libhaversine.jnilib java/Haversine.class
	java -classpath java -Djava.library.path=$(PWD)/java Haversine

run_test: test/haversine
	test/haversine test/test.sqlite3

test/haversine: test/test.sqlite3
	gcc -Wall -lsqlite3 test/main.c src/haversine.c -o test/haversine

test/test.sqlite3:
	sqlite3 test/test.sqlite3 < test/seed.sql

src/haversine.o:
	gcc -c src/haversine.c -o src/haversine.o

java/haversine.o:
	gcc -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -c java/haversine.c -o java/haversine.o

java/libhaversine.jnilib: java/haversine.o src/haversine.o
	gcc -dynamiclib -lsqlite3 -o java/libhaversine.jnilib java/haversine.o src/haversine.o

java/Haversine.class:
	javac java/Haversine.java

clean:
	rm -f java/haversine.o java/libhaversine.jnilib java/Haversine.class
	rm -f src/haversine.o
	rm -f test/haversine test/test.sqlite3
