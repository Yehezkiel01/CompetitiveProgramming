/*
  The main idea is to precompute several range value, so that every L and R possiblity could be found by merging at most two range of 
value. Let's take a pivot in n/2, count every range (1,n/2), (2,n/2), ..(n/2,n/2) and also (n/2+1,n/2+1), (n/2+1,n/2+2), ... (n/2+1,n). 
With these ranges, we could answer all query for L<=n/2 and R>=n/2 by merging at most 2 range value only. for L<n/2 and R <n/2 or L>n/2 
and R>n/2 we may apply the same strategy by taking the pivot n/4 and 3n/4. Continue this process (recursion) until all of the L and R 
possibility could be answered.
*/
#include <bits/stdc++.h>
using namespace std;
#include "secret.h"
const int MAXV=1000000000;
const int MAXN=1000;

int n,arr[MAXN+5],hasil[MAXN+5][MAXN+5];
int tanya(int a,int b){
	assert(0<=a&&a<=MAXV);
	assert(0<=b&&b<=MAXV);
	return Secret(a,b);
}

void build(int l,int r){
	if((r-l+1)<2)
		return;
	
	int midL=(l+r)>>1;
	int midR=midL+1;
	
	for(int i=midL,val=arr[midL];i>=l;val=tanya(arr[--i],val))
		hasil[midL][i]=val;
	for(int i=midR,val=arr[midR];i<=r;val=tanya(val,arr[++i]))
		hasil[midR][i]=val;
	
	build(l,midL-1);
	build(midR+1,r);
}
void Init(int N,int _arr[]){
	memset(hasil,-1,sizeof(hasil));
	n=N;
	for(int i=0;i<n;i++)
		arr[i]=hasil[i][i]=_arr[i];
	build(0,n-1);
}
int solve(int kiri,int kanan,int l,int r){
	int midL=(kiri+kanan)>>1;
	int midR=midL+1;
	if(kanan-kiri+1<0)
		return 0;
	
	if(l<=midR&&r>=midL)
	{
		if(l==midR)
			return hasil[midR][r];
		if(r==midL)
			return hasil[midL][l];
		return tanya(hasil[midL][l],hasil[midR][r]);
	}
	else if(l>midR)
	{
		return solve(midR+1,kanan,l,r);
	}
	else if(r<midL)
	{
		return solve(kiri,midL-1,l,r);
	}
}
int Query(int l,int r){
	assert(l<=r);
	return solve(0,n-1,l,r);
}
