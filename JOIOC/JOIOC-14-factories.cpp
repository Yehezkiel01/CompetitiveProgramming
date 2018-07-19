/*
  Main Idea:
    Update the distance of each node of the first set to its ancestors in centroid, so each node in centroid only keep the smallest
distance to one of its descendant which is included in the first set. Then, for each node in the second set, we traverse to its ancestor
and combine the ancestor's stored value with the distance of the node and the ancestor. the answer is the minimum combined value. 
    In order to find distance of two nodes, we could use LCA.
*/
#include "factories.h"
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define eb emplace_back
#define pb push_back
#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
typedef long long LL;
typedef long double LD;
typedef pair<int,int> pii;
const int logn=19;
const int MAXN=500000;
int par[logn+3][MAXN+5]={},depth[MAXN+5]={},n;
LL jarak[MAXN+5]={};
vector <pii> node[MAXN+5];
inline void inisLCA(int now,int _par,int _depth,LL _jarak){
	depth[now]=_depth;
	jarak[now]=_jarak;
	par[0][now]=_par;
	for(auto v:node[now])
	{
		if(v.fi==_par)
			continue;
		inisLCA(v.fi,now,_depth+1,_jarak+v.se);
	}
}
inline void inisLCA(){
	inisLCA(1,0,0,0);
	for(int j=1;j<logn;++j)
		for(int i=1;i<=n;++i)
			par[j][i]=par[j-1][par[j-1][i]];
}
inline int LCA(int u,int v){
	if(depth[u]>depth[v])
		swap(u,v);
	int beda=depth[v]-depth[u],lsone;
	while(beda)
	{
		lsone=beda&-beda;
		v=par[__builtin_popcount(lsone-1)][v];
		beda^=lsone;
	}
	if(u==v)
		return u;
	for(int i=logn-1;i>=0;--i)
	{
		if(par[i][u]!=par[i][v])
			u=par[i][u],v=par[i][v];
	}
	return par[0][u];
}
inline LL dist(int u,int v){
	return jarak[u]+jarak[v]-(jarak[LCA(u,v)]<<1);
}

int parCtr[MAXN+5]={},subtree[MAXN+5]={};
int hitungSubtree(int now,int tadi){
	assert(parCtr[now]==-1);
	subtree[now]=1;
	for(auto v:node[now])
	{
		if(v.fi==tadi||parCtr[v.fi]!=-1)
			continue;
		subtree[now]+=hitungSubtree(v.fi,now);
	}
	return subtree[now];
}
void buatCtr(int now,int tadi){
	assert(parCtr[now]==-1);
	int target=hitungSubtree(now,-1)>>1;
	
	int pilihan=-1,skipAja=tadi;
	bool lanjut=true;
	while(lanjut)
	{
		lanjut=false;
		for(auto v:node[now])
		{
			if(v.fi==skipAja||parCtr[v.fi]!=-1)
				continue;
			if(subtree[v.fi]>target)
			{
				lanjut=true;
				pilihan=v.fi;
			}
		}
		if(lanjut)
		{
			skipAja=now;
			now=pilihan;
		}
	}
	
	parCtr[now]=tadi;
	for(auto v:node[now])
	{
		if(parCtr[v.fi]==-1)
			buatCtr(v.fi,now);
	}
}
vector <LL> distPar[MAXN+5];
inline void distancePar(){
	for(int i=1;i<=n;i++)
		for(int j=i;j;j=parCtr[j])
			distPar[i].pb(dist(i,j));
}
inline void buatCtr(){
	memset(parCtr,-1,sizeof(parCtr));
	buatCtr(1,0);
	distancePar();
}
void Init(int N,int A[],int B[],int D[]){
	n=N;
	for(int i=0;i<N-1;++i)
	{
		++A[i];++B[i];
		node[A[i]].eb(B[i],D[i]);
		node[B[i]].eb(A[i],D[i]);
	}
	inisLCA();
	memset(parCtr,-1,sizeof(parCtr));
	buatCtr();
}

int lastReset[MAXN+5]={},kejadian=0;
LL registered[MAXN+5];
const LL INF=1000000000000000;
inline void ngeFix(int pos){
	if(lastReset[pos]!=kejadian)
	{
		registered[pos]=INF;
		lastReset[pos]=kejadian;
	}
}
inline void update(int pos){
	for(int i=0,now=pos;now;i++,now=parCtr[now])
	{
		ngeFix(now);
		registered[now]=min(registered[now],distPar[pos][i]);
	}
}
inline LL Query(int pos){
	LL ret=INF;
	for(int i=0,now=pos;now;i++,now=parCtr[now])
	{
		ngeFix(now);
		ret=min(ret,registered[now]+distPar[pos][i]);
	}
	return ret;
}
LL Query(int S,int X[],int T,int Y[]){
	for(int i=0;i<S;++i)	++X[i];
	for(int i=0;i<T;++i)	++Y[i];
	++kejadian;
	for(int i=0;i<S;++i)
		update(X[i]);
	LL ret=INF;
	for(int i=0;i<T;++i)
		ret=min(ret,Query(Y[i]));
	return ret;
}
