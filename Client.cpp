#include <iostream>
#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <string>

#define PORT 9077

using namespace std;

int main(int argc, char const *argv[]) { 

  if (argc != 2) {
    cerr << "Usage: ./Client n\n" << "\tWhere n is the Thing Number Server is on\n";
    return -1;
  }

  int thing_number = atoi(argv[1]);
  int inet_add = 0;

  struct sockaddr_in address; 
  int sock = 0, valread; 
  struct sockaddr_in serv_addr; 
  string hello = "Hello from client"; 
  char buffer[1024] = {0}; 
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
    cerr << "Socket creation error\n";
    return -1; 
  } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(PORT); 

  if (thing_number == 0) {
    inet_add = inet_pton(AF_INET, "10.35.195.46", &serv_addr.sin_addr);
  } else if (thing_number == 1) {
    inet_add = inet_pton(AF_INET, "10.35.195.47", &serv_addr.sin_addr);
  } else if (thing_number == 2) {
    inet_add = inet_pton(AF_INET, "172.28.8.165", &serv_addr.sin_addr);
  } else {
    inet_add = inet_pton(AF_INET, "172.28.8.166", &serv_addr.sin_addr);
  }

  // Convert IPv4 and IPv6 addresses from text to binary form 
  // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
  if(inet_add <= 0) { 
    cerr << "Invalid address, or address not supported\n";
    return -2; 
  } 

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
    cerr << "Connection failed\n";
    return -3; 
  } 

  char hostname[1024];
  gethostname(hostname, 1024);
  puts(hostname);

  send(sock, &hello, strlen(hello.c_str()), 0 ); 
  printf("Hello message sent\n"); 
  valread = read( sock , buffer, 1024); 
  printf("%s\n",buffer ); 
  return 0; 
} 
