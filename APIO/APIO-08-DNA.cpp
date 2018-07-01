/*
  Idea:
    In order to solve this problems, dp with 3 states is needed. DP[i][j][k] returns the total number of form-k combination that could 
    be made by changing the 'N' characters from the suffix of the string starting from i, while j denotes the previous characters.
    At every transition, try putting every nucleotides one by one and sum all the suffix possibilities starting form i+1 by calling
    another dp function with the right states. It is also important to reduce the value of k by 1 if the nucleotide comes before j.
    
    By using the dp, we could easily determine each of the 'N' characters (do it from left to right) by trying the nucleotides according 
    to their order then check whether the rest of the chain has more combination than r or not. If they do, then proceed to the next 
    characters, otherwise try the next nucleotides.
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
int convert(char a){
	if(a=='A') return 1;
	if(a=='C') return 2;
	if(a=='G') return 3;
	return 4;
}
char convert(int a){
	if(a==1) return 'A';
	if(a==2) return 'C';
	if(a==3) return 'G';
	return 'T';
}
const int MAXN=50000;
const int MAXK=10;
const int MAXTYPE=4;
int n,k;
LL r,dp[MAXN+5][MAXTYPE+5][MAXK+5];
string input;
LL topdown(int idx,int tadi,int form){
	if(form==0)
		return 0;
	if(idx==n)
		return 1;
	LL &ret=dp[idx][tadi][form];
	if(ret!=-1)
		return ret;
	ret=0;
	for(int i=1;i<=4;i++)
	{
		if(input[idx]!='N'&&convert(input[idx])!=i)
			continue;
		int nextform=form;
		if(tadi>i)
			nextform--;
		ret+=topdown(idx+1,i,nextform);
	}
	return ret;
}
void generateAns(){
	int tadi=1,form=k;
	for(int i=0;i<n;i++)
	{
		if(input[i]=='N')
		{
			for(int j=1;j<=4;j++)
			{
				int nextform=form;
				if(tadi>j)
					nextform--;
				if(topdown(i+1,j,nextform)<r)
					r-=topdown(i+1,j,nextform);
				else
				{
					input[i]=convert(j);
					break;
				}
			}
		}
		if(tadi>convert(input[i]))
			form--;
		tadi=convert(input[i]);
	}
}
int main()
{
	memset(dp,-1,sizeof(dp));
	cin>>n>>k>>r;
	cin>>input;
	generateAns();
	cout<<input<<endl;
}
