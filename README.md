# Labirinto-de-Creta

# Compile and run Server:

- javac *.java

- javah ServerImplements

- gcc -I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux -shared -o libmyjni.so LabirintoDeCreta.c -fPIC

- rmiregistry

- java -Djava.library.path=. Server

# Compile and run Client:

- Copy stub to client path

- javac *.java

- java -Djava.library.path=. Client
