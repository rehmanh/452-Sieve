#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include "Sieve.h"
#include <algorithm>


using namespace std;

Sieve::Sieve() {
  this -> max = max;  
  this -> vec = vec;
}

void Sieve::print(vector<int> vec) {
  if (!vec.empty()) {
    for(int i = 0; i < vec.size(); i++) {
      cout << vec.at(i) << " ";
    }
  }
}

vector<int> Sieve::runSieve(vector<int> vec) {
	int front = vec.at(0);
  for (int i = 1; i < vec.size(); i++) {
    if ((vec.at(i) % front) == 0) {
      vec.erase(vec.begin() + i);
    }
  }
	return vec;
}

bool Sieve::isEmpty(vector<int> vec) {
  return vec.empty();
}


	// for (int i = front * front; i < vec.size(); i+=front){
	// 	// vec.erase(std::remove(vec.begin(), vec.end(), i), vec.end());
  //   // vec.erase(std::remove_if([](int i) { return i % front == 0; });
  //   bool fu = i % front == 0;
  //   vec = std::remove_if(vec.begin(), vec.end(), fu);
	// }


