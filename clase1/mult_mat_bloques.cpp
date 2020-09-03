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
  int s = MAX / 3 + 1;

  int temp;
  for(int jj = 0;jj < MAX;jj += s){
    for(int kk = 0;kk < MAX;kk += s){
      for(int i = 0;i < MAX; i++){
        for(int j = jj; j < ((jj+s)>MAX?MAX:(jj+s)); j++){
          temp = 0;
          for(int k = kk; k < ((kk+s)>MAX?MAX:(kk+s)); k++){
            temp += aMatrix[i][k] * bMatrix[k][j];
          }
          product[i][j] += temp;
        }
      }
    }
  }
}

int main() {
  int aMatrix[MAX][MAX];
  int bMatrix[MAX][MAX];
  int product[MAX][MAX];
  memset(product, 0, sizeof(product));

  mult_matrices(aMatrix, bMatrix, product);
  // print_matrix(product);
}