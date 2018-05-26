#echo


rm server

#echo server executable removed


gcc -w -o server server.c;

#echo server compilation complete

#echo starting server

./server 5004
