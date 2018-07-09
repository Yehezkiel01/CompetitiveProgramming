/*
  Idea:
    From observation, we knew that the maximum number of critical edges in a graph is N-1. My idea is to build a forest spanning tree
and find all the critical edges in that spanning tree. This is supported by the fact that only those edges, used to build the forest
spanning tree, might become critical. At the beginning, there are n separated component. Then, we process the edge one by one. 
    
    If an edge connects two nodes that lies in the same component, then that edge must be not critical. Moreover, a cycle will be formed
with that edge and several others edge within the component. We need to mark those edges as not critical efficiently, I use LCA and
something similiar to prefix sum. 
    
    If an edge connects two nodes that lies in different component, then that edge might be critical. In addition, we need to merge the
two components, it would be beneficial to merge the smaller size component to the bigger size component so that we do not have to
rebuild the LCA many times.
*/
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back
#define gc getchar_unlocked
#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
typedef pair<int,int> pii;
const int MAXN=100000;
inline void scan(int &angka){
	char input=gc();
	while(!('0'<=input&&input<='9'))	input=gc();
	angka=0;
	while('0'<=input&&input<='9')	angka=(angka<<1)+(angka<<3)+input-'0',input=gc();
}
int n,m,i;

const int logn=17;
vector <pii> edgelist;
vector <int> node[MAXN+1];
int kandungan[MAXN+1],par[MAXN+1][logn]={},delta[MAXN+1]={},depth[MAXN+1]={};
bitset<MAXN+1> report;
void dfs(const int &now){
	for(int i=1;i<logn;++i)
		par[now][i]=par[par[now][i-1]][i-1];
	
	for(int i=0;i<node[now].size();++i)
	{
		int tempDfs=(edgelist[node[now][i]].fi==now)?edgelist[node[now][i]].se:edgelist[node[now][i]].fi;
		if(tempDfs==par[now][0])
			continue;
		par[tempDfs][0]=now;
		depth[tempDfs]=depth[now]+1;
		kandungan[tempDfs]=node[now][i];
		dfs(tempDfs);
	}
}
inline int lca(int u,int v){
	if(depth[u]>depth[v])
		swap(u,v);
	int beda=depth[v]-depth[u];
	while(beda)
	{
		v=par[v][__builtin_popcount((beda&-beda)-1)];
		beda-=beda&-beda;
	}
	if(u==v)
		return u;
	for(i=logn-1;i>=0;--i)
	{
		if(par[u][i]!=par[v][i])
		{
			u=par[u][i];
			v=par[v][i];
		}
	}
	return par[u][0];
}
int findReport(const int &now){
	int status=delta[now];
	delta[now]=0;			//done urusannya, ngereport karena butuh di reset
	for(int i=0;i<node[now].size();++i)
	{
		int tempDfs=(edgelist[node[now][i]].fi==now)?edgelist[node[now][i]].se:edgelist[node[now][i]].fi;
		if(tempDfs==par[now][0])
			continue;
		status+=findReport(tempDfs);
	}
	if(status!=0&&kandungan[now]!=-1)				//part of cycle berarti
		report[kandungan[now]]=false;
	return status;
}

class UFDS{
	private:
	int p[MAXN+1],size[MAXN+1];
	int finds(const int &u){
		if(p[u]!=u)
			p[u]=finds(p[u]);
		return p[u];
	}
	public:
	inline void inis(){
		for(int i=1;i<=n;++i)
		{
			p[i]=i;
			size[i]=1;
		}
	}
	inline bool boss(const int &pos){
		return p[pos]==pos;
	}
	inline bool connected(const int &u,const int &v){
		return finds(u)==finds(v);
	}
	inline void gabung(int &u,int &v){
		int rootU=finds(u);
		int rootV=finds(v);
		if(rootU==rootV)
			return;
		if(size[rootU]<size[rootV])
		{
			swap(rootU,rootV);
			swap(u,v);
		}
		size[rootU]+=size[rootV];
		p[rootV]=rootU;
		
		findReport(rootV);
		par[v][0]=u;
		depth[v]=depth[u]+1;
		kandungan[v]=edgelist.size();
		dfs(v);
	}
};
UFDS disjointSet;
int main()
{	
	scan(n),scan(m);
	disjointSet.inis();
	
	int u,v;
	for(int i=1;i<=m;++i)
	{
		scan(u),scan(v);
		if(disjointSet.connected(u,v))
		{
			delta[lca(u,v)]-=2;
			delta[u]++;
			delta[v]++;
		}
		else
		{
			disjointSet.gabung(u,v);
			node[u].eb(edgelist.size());
			node[v].eb(edgelist.size());
			report[edgelist.size()]=true;
			edgelist.eb(u,v);
		}
	}
	for(i=1;i<=n;++i)
		if(disjointSet.boss(i))
			findReport(i);
	for(i=0;i<edgelist.size();++i)
	{
		if(!report[i])
			continue;
		printf("%d %d\n",edgelist[i].fi,edgelist[i].se);
	}
}
