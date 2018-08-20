/*
  Idea:
  let assume that the minimal possible number of cobblestone road in any possible spanning tree is L and the maximal possible number of 
cobblestone road in any possible spanning tree is R. The solution may only exists when L <= K <= R. L and R could be found easily by any 
minimum spanning tree algorithm by arranging the order of the edge carefully. In order to find right configuration ,i.e. spanning tree 
containing exactly K cobblestone, we may start from the maximal cobblestone configuration and start to reduce the cobblestone road one 
by one by finding a concrete road which addition to the current spanning tree would cause a cycle containing a cobblestone road. Just 
remove that cobblestone road and add the concrete road to the current spanning tree. Now we have a new spanning tree with R-1 
Cobblestone Road. Repeat this process until we left onlye with K cobblestone road. However, removing them one by one would be too slow. 
Luckily, we could find the R-K new concrete road simultaneously (see the implementation for details). 
*/
#include "lib1740.h"
#include <bits/stdc++.h>
using namespace std;
const int MAXN=20000;
const int MAXM=100000;

struct trio{
	int u,v,c;
	trio(int _u,int _v,int _c){
		u=_u;
		v=_v;
		c=_c;
	}
	trio(){
	}
};
vector <trio> ans;

int n, m, k, u[MAXM+5], v[MAXM+5], c[MAXM+5], par[MAXN+5];

bool taken[MAXM+5];

void reArrange(){
	for(int i=0,j=0;i<m;i++)
	{
		if(c[i]==0)
		{
			swap(u[i],u[j]);
			swap(v[i],v[j]);
			swap(c[i],c[j]);
			j++;
		}
	}
}

int finds(int u){
	if(par[u]!=u)
		par[u]=finds(par[u]);
	return par[u];
}
bool gabung(int u,int v){
	u=finds(u);
	v=finds(v);
	if(u==v)
		return false;
	par[v]=u;
	return true;
}
void reset(){
	for(int i=1;i<=n;i++)
		par[i]=i;
}

bool makeAns(){
	int diambil=0,jumlahEdge=0;
	for(int i=0;i<m;i++)
	{
		if(gabung(u[i],v[i]))
		{
			jumlahEdge++;
			diambil+=c[i]^1;
			if(c[i]==1)
			{
				taken[i]=true;
				ans.emplace_back(u[i],v[i],c[i]);
			}
		}
	}
	if(jumlahEdge<(n-1))			//no configuration exists
		return false;
	
	if(diambil<k)					//too little cobblestone
		return false;
	
	reset();						//reset all the edge in the spanning tree except for the previously taken concrete road
	for(int i=m-1;i>=0;i--)
		if(taken[i])
			gabung(u[i],v[i]);
			
	int buang=diambil-k;
	for(int i=m-1;i>=0&&buang>0;i--)				//add some more concrete stone
	{
		if(taken[i])
			continue;
		if(c[i]==0)
			break;
		if(gabung(u[i],v[i]))
		{
			buang--;
			ans.emplace_back(u[i],v[i],c[i]);
		}
	}
	
	if(buang>0)						//too much cobblestone
		return false;
	
	for(int i=0;i<m&&k>0;i++)			//add the rest of K cobblestone
	{
		assert(c[i]==0);
		if(gabung(u[i],v[i]))
		{
			k--;
			ans.emplace_back(u[i],v[i],c[i]);
		}
		
	}
	assert(k==0);
	return true;
}
int main()
{
	Init(n, m, k, u, v, c);
	reset();
	
	memset(taken,false,sizeof(taken));
	reArrange();
	
	if(!makeAns())
		NoSolution();
	else
	{
		for(auto isi:ans)
			Solution(isi.u,isi.v,isi.c);
	}
}
