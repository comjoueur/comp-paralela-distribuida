#include<iostream>
using namespace std;


int main() {
  int MAX;
  cin >> MAX;
  int aMatrix[MAX][MAX];
  int bMatrix[MAX][MAX];
  int product[MAX][MAX];

  for(int i = 0; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      product[i][j] = 0;
    }
  }

  for(int row = 0; row < MAX; row++) {
    for(int col = 0; col < MAX; col++) {
      for(int inner = 0; inner < MAX; inner++) {
          product[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
      }
    }
  }
}