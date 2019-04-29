#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>

using namespace std;

int main () {
  // create a socket
  int listening = socket(AF_INET, SOCK_STREAM, 0);

  if(listening == -1) {
    cerr << "There was an error creating the socket\n";
    return -1;
  }

  // bind the socket to an IP/PORT
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(9077); /* host to network - handles endianness */
  inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //pton -> pointer to number

  if(bind(listening, (sockaddr*) &hint, sizeof(hint)) == -1) {
    cerr << "Can't bind to IP / Port\n";
    return -2;
  }

  // mark the socket for listening in
  if(listen(listening, SOMAXCONN) == -1) {
    cerr << "The server failed to listen\n";
    return -3;
  }

  // accept a call
  sockaddr_in client;
  socklen_t client_size = sizeof(client);
  char host[NI_MAXHOST]; // 1025 in size -> 1024 plus a 0 bit
  char service[NI_MAXSERV]; // 32 
  
  //accept an incoming connection
  int client_sock = accept(listening, (sockaddr*) &client, &client_size);

  if(client_sock == -1) {
    cerr << "There was an error with the client connection\n";
    return -4;
  }
    
  // close the listening socket
  close(listening);

  memset(host, 0, NI_MAXHOST);
  memset(service, 0, NI_MAXSERV);

  int result = getnameinfo(
          (sockaddr*) &client,
          sizeof(client),
          host,
          NI_MAXHOST,
          service,
          NI_MAXSERV,
          0);
 
  if(result) {
   cout << host << " connected on " << service << endl;
  } else {
   inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
   cout << host << " connected on " << ntohs(client.sin_port) << endl;
  }

  // while receiving, display and echo message
  char buf[4096];
  while(1) {
    // clear the buffer
    memset(buf, 0, 4096);
     
    // wait for message
    int bytes_recvd = recv(client_sock, buf, 4096, 0); 
    if(bytes_recvd == -1) {
      cerr << "There was a connection issue\n";
      break;
    }

    if(bytes_recvd == 0) {
      cout << "The client diconnected\n";
      break;
    }
    
    // display message
    cout << "Received: " << string(buf, 0, bytes_recvd) << endl;

    // resend message
    send(client_sock, buf, bytes_recvd + 1, 0);
  }

  // close the socket
  close(client_sock);

  return 0;
}
