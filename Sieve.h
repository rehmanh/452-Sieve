#ifndef SIEVE_H
#define SIEVE_H

#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Sieve {
  private:
    int max;
    vector<int> vec;
  protected:
  public:
    Sieve();
    vector<int> runSieve(vector<int> vec);
    void print(vector<int> vec);
    bool isEmpty(vector<int> vec);
};

#endif
