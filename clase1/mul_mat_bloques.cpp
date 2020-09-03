#include<iostream>
using namespace std;


int main() {
  int MAX;
  cin >> MAX;
  int aMatrix[MAX][MAX];
  int bMatrix[MAX][MAX];
  int product[MAX][MAX];

  int s = MAX / 2;
  for(int i = 0; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      product[i][j] = 0;
    }
  }

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