/*
  Idea:
    Assign every C characters with 1, and every S characters with -2. A certain range from the strip could be sold under "2 Chocolate 1 
    Strawberry" promo if and only if their sum is 0. Do prefix sum, and store its value in an array. if Prefix Sum-i, which is the sum of 
    all the values from 1 to i, is equal to Prefix Sum-j and j<i then, it could be infered that the sum of all the assigned values from j+1
    to i is equal to 0. Using the information that is stored in the array, we could find the number of valid j.
*/
#include <bits/stdc++.h>
using namespace std;
const int tambah=2000000;
const int MAXVAL=4000000;
int panjang,status=0,occ[MAXVAL+5]={};
string input;
long long ans=0;
int main()
{
	cin>>panjang;
	cin>>input;
	occ[tambah]=1;
	for(int i=0;i<panjang;i++)
	{
		status+=(input[i]=='C')?1:-2;
		ans+=occ[status+tambah];
		occ[status+tambah]++;
	}
	cout<<ans<<endl;
}
