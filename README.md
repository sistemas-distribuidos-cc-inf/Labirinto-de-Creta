# Labirinto-de-Creta

## Compile and run Server:

1. javac *.java

2. javah ServerImplements

3. gcc -I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux -shared -o libmyjni.so LabirintoDeCreta.c -fPIC

4. rmic ServerImplements

5. rmiregistry

6. java -Djava.library.path=. Server

## Compile and run Client:

1. Copy stub from server to client path

2. javac *.java

3. java -Djava.library.path=. Client
