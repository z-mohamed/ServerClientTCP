#echo

#echo removing outdated client executable

rm client

#echo client executable removed

#echo compiling client

gcc -w -o client client.c;

#echo client compilation complete

#echo client connecting to server at 192.168.1.142 : 5000

./client 192.168.1.142 5004
