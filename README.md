# Multiple Client-Server File Transfer
A simple file transfer program build on the top of TCP client server architecture. The server is a concurrent server, so it can handle multiple clients at the same time.

## Compile the program
gcc server.c -o server <br/>
gcc client.c -o client <br/>
<br/>

## Working 
<br/>
1. Run the server program. <br/>
2. Run the client program. <br/>
3. Type LIST command to list the files in the `server_files` directory. <br/>
4. Type LOAD `filename` to download the file from the server and save it to the `client_files` directory. <br/>
<br/>

### NOTE: 
Do not change the name of the files present in the `server_files` directory. The server and the client program use the names of these files and also do not add any more file as it will not work.
