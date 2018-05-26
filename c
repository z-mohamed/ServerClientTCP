#echo

echo removing outdated client executable

rm client

echo client executable removed

echo compiling client

gcc -w -o client client.c;

echo client compilation complete

echo client connecting to server at 127.0.0.1 : 5004

./client 127.0.0.1 5004
