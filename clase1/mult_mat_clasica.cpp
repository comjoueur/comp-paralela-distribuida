#include<bits/stdc++.h>
#define MAX 100
using namespace std;

void print_matrix(int mat[MAX][MAX]) {
  for(int i = 0; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      cout << mat[i][j] << " ";
    } cout << endl;
  }
}

void mult_matrices(int aMatrix[MAX][MAX], int bMatrix[MAX][MAX], int product[MAX][MAX]) {
  for(int row = 0; row < MAX; row++) {
    for(int col = 0; col < MAX; col++) {
      for(int inner = 0; inner < MAX; inner++) {
          product[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
      }
    }
  }
}

int main() {
  int aMatrix[MAX][MAX] = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
  int bMatrix[MAX][MAX] = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
  int product[MAX][MAX];
  memset(product, 0, sizeof(product));
  mult_matrices(aMatrix, bMatrix, product);
  // print_matrix(product);
}