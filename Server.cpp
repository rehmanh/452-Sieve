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
#include <vector>
#include "Sieve.h"

#define PORT 9078

using namespace std;

int main() {

  int server_fd, cli_sock;
  struct sockaddr_in address;
  typedef int64_t size_type;
  int opt = 1;
  int addrlen = sizeof(address);
  vector<int> vec;
  Sieve *sieveB;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { // creating socket file descriptor
    cerr << "Creating socket failed";
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { // forcefully attach socket to port
    cerr << "There was an error in set sock opt";
    exit(EXIT_FAILURE);
  }
 
  memset(&address, '0', sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  /* error checking in bind */
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    cerr << "Binding to the socket failed\n";
    exit(EXIT_FAILURE);
  }

  /* error checking in listen */
  if (listen(server_fd, SOMAXCONN) < 0) {
    cerr << "Error while Listening";
    exit(EXIT_FAILURE);
  }
  
  /* error checking in accept */
  if ((cli_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
    cerr << "There was an error with accepting socket";
    exit(EXIT_FAILURE);
  }

  size_type sz;
  int bytes_recv = 0;

  vector<int> vectorB; 
  
  while (1) {
    recv(cli_sock, &sz, sizeof(sz), MSG_WAITALL);
    vectorB.resize(sz);
    bytes_recv = recv(cli_sock, &vectorB[0], sz * sizeof(int), MSG_WAITALL);

    cout << "Recvd: ";
    cout << vectorB[0] << "," << vectorB[1] << ","
      << vectorB[2] << "," << ",...," << vectorB.at(vectorB.size() - 3) << ","
      << vectorB.at(vectorB.size() - 2) << "," << vectorB.at(vectorB.size() - 1);
    cout << "\n";

    vectorB = sieveB->runSieve(vectorB); 

    /* store the first value, and pop it */
    int prime = vectorB.at(0);
    vectorB.erase(vectorB.begin());

    cout << "Prime: " << prime << endl;

    /* going to send it right back */
    size_type v_size = vectorB.size();
    send(cli_sock, &v_size, sizeof(v_size), 0);
    send(cli_sock, &vectorB[0], v_size * sizeof(int), 0);

    cout << "Sent: ";
    cout << vectorB[0] << "," << vectorB[1] << ","
      << vectorB[2] << "," << ",...," << vectorB.at(vectorB.size() - 3) << ","
      << vectorB.at(vectorB.size() - 2) << "," << vectorB.at(vectorB.size() - 1);
    cout << "\n\n\n";

    /* send over the prime */
    send(cli_sock, &prime, sizeof(prime), 0);
  }

  /* we are done with the sieve, now want to send over the remaining vector of primes */
  sz = vectorB.size();
  send(cli_sock, &sz, sizeof(sz), 0);
  send(cli_sock, &vectorB[0], sz * sizeof(int), 0);

  close(cli_sock);
  return 0;
}
