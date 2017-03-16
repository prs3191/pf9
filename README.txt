to compile:
-----------

g++ -std=c++11 server.cpp -o server
g++ -std=c++11 client.cpp -o client

to run in terminal 1: (run this first)
----------------------------------------
./server --server-ip 127.0.0.1 --server-port 1300

to run in terminal 2:
---------------------
./client --server-ip 127.0.0.1 --server-port 1300

example output from terminal 1:
-------------------------------
Entered ip:127.0.0.1
Entered port number:1304
Binding successful..
Connection accepted

in server received bytes:10485760

example output from terminal 2:
--------------------------------
Entered ip:127.0.01
Entered port number:1304
Connect successful..
Starting upload test 

upload speed = 6.985e+06 bits/sec
starting download test

download speed = 5.986e+06 bits/sec
