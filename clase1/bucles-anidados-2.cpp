#include<iostream>
using namespace std;

int main() {
  int MAX;
  cin >>MAX;

  double A[MAX][MAX], x[MAX], y[MAX];

  for (int j = 0; j < MAX; j++) {
    for(int i = 0; i < MAX; i++) {
      y[i] += A[i][j] * x[j];
    }
  }
}