#include <iostream>
#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <limits.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <string>
#include <vector>
#include "Sieve.h"

#define PORT 9078

using namespace std;

int main(int argc, char const *argv[]) { 

  if (argc != 3) {
    cerr << "Usage: ./Client <thing-number> <upper bound>\n";
    return -1;
  }

  int thing_number = atoi(argv[1]);
  int max_val = atoi(argv[2]);
  int inet_add = 0;

  typedef int64_t size_type;
  struct sockaddr_in address; 
  int sock = 0;
  struct sockaddr_in serv_addr; 
  vector<int> vec;
  vector<int> master_list;
  Sieve *sieveA; 

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
    cerr << "Socket creation error\n";
    exit(EXIT_FAILURE); 
  } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(PORT); 

  if (thing_number == 0) { /* Convert IPv4 and IPv6 addresses from text to binary form */
    inet_add = inet_pton(AF_INET, "10.35.195.46", &serv_addr.sin_addr);
  } else if (thing_number == 1) {
    inet_add = inet_pton(AF_INET, "10.35.195.47", &serv_addr.sin_addr);
  } else if (thing_number == 2) {
    inet_add = inet_pton(AF_INET, "172.28.8.165", &serv_addr.sin_addr);
  } else {
    inet_add = inet_pton(AF_INET, "172.28.8.166", &serv_addr.sin_addr);
  }

  if(inet_add <= 0) { 
    cerr << "Invalid address, or address not supported\n";
    exit(EXIT_FAILURE); 
  } 

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
    cerr << "Connection failed\n";
    exit(EXIT_FAILURE); 
  } 
  
  for (int i = 2; i <= max_val; i++) {
    vec.push_back(i);
  }

  int one_prime = 0;
  vector<int> vectorA = sieveA->runSieve(vec);
  one_prime = vectorA.at(0);
  vectorA.erase(vectorA.begin());
  master_list.push_back(one_prime);

  cout << "Prime: " << master_list.at(0) << endl;
  size_type sz = vectorA.size();

  send(sock, &sz, sizeof(sz), 0);
  send(sock, &vectorA[0], sz * sizeof(int), 0);
  cout << "Sent: "; 
  cout << vectorA[0] << "," << vectorA[1] << ","
    << vectorA[2] << ",...," << vectorA.at(vectorA.size() - 3) << ","
    << vectorA.at(vectorA.size() - 2) << "," << vectorA.at(vectorA.size() - 1)
    << "\n\n\n";
  
  int bytes_recv = 0;
  bool empty = false;
  int num = 0;

  while (num < (int)sqrt(max_val)) { 
    recv(sock, &sz, sizeof(sz), MSG_WAITALL);
    vectorA.resize(sz);
    bytes_recv = recv(sock, &vectorA[0], sz * sizeof(int), MSG_WAITALL);

    recv(sock, &one_prime, sizeof(int), MSG_WAITALL);
    master_list.push_back(one_prime);

    vectorA = sieveA->runSieve(vectorA);

    cout << "\nRecd: ";
    cout << vectorA[0] << "," << vectorA[1] << ","
      << vectorA[2] << ",...," << vectorA.at(vectorA.size() - 3) << ","
      << vectorA.at(vectorA.size() - 2) << "," << vectorA.at(vectorA.size() - 1);
    cout << "\n";
    cout << "Prime: ";
    cout << master_list.back();
    cout << "\n";

    /* now going to send it back */
    sz = vectorA.size();
    send(sock, &sz, sizeof(sz), 0);
    send(sock, &vectorA[0], sz * sizeof(int), 0);

    cout << "Sent: "; 
    cout << vectorA[0] << "," << vectorA[1] << ","
      << vectorA[2] << ",...," << vectorA.at(vectorA.size() - 3) << ","
      << vectorA.at(vectorA.size() - 2) << "," << vectorA.at(vectorA.size() - 1)
      << "\n\n\n";

    num++;
  }

  recv(sock, &sz, sizeof(sz), MSG_WAITALL);
  vectorA.resize(sz);
  bytes_recv = recv(sock, &vectorA[0], sz * sizeof(int), MSG_WAITALL);

  /* append masterlist with what was received */
  master_list.insert(master_list.end(), vectorA.begin(), vectorA.end());
  cout << "\n***All Primes***:\n";
  cout << master_list[0] << "," << master_list[1] << ","
    << master_list[2] << ",...," << master_list.at(master_list.size() - 3) << ","
    << master_list.at(master_list.size() - 2) << "," << master_list.at(master_list.size() - 1)
    << "\n\n\n";
  cout << "There were " << master_list.size() + 1 << " primes found\n";

  close(sock);
  return 0; 
} 

