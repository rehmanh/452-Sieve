#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

#define PORT 9077

using namespace std;

int main() {

  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  string hello = "Hello from the servevr";

  // creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    cerr << "Creating socket failed";
    exit(EXIT_FAILURE);
  }

  // forcefully attach socket to port
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    cerr << "There was an error in set sock opt";
    exit(EXIT_FAILURE);
  }
  
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    cerr << "Binding to the socket failed";
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    cerr << "Error while Listening";
    exit(EXIT_FAILURE);
  }
  
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
    cerr << "There was an error with accepting socket";
    exit(EXIT_FAILURE);
  }

  /* testing print hostname of server */
  char hostname[HOST_NAME_MAX];
  int host_n = gethostname(hostname, HOST_NAME_MAX);
  cout << host_n << " is the hostname\n";

  valread = read(new_socket, buffer, 1024);
  cout << buffer << endl;
  send(new_socket, &hello, strlen(hello.c_str()), 0);
  cout << "hello message sent" << endl;
  return 0;
}
