/*
  Idea:
    - At every step, treat every connected component as a single node. We need to find the right disjoint set that will return 1. 
    - Suppose that there are N nodes, which come from n connected component. Divide the nodes into two part, each of which has N/2 nodes.
    - If we do query, and get 1 as the return value, then we have found the right disjoint set.
    - Otherwise, try to divide each part into two other part again, then query them one by one. To speed up the process, we may do the
      query for both part together.
    - We keep on dividing each part into two until we find the right disjoint set. This process will take log(N) queries, as long as you 
      do the query for each part together.
    - Now that we have found the right disjoint set, revert back each node into a connected component, then do binary search to cut down
      the size of each set until only one node left, each of this process will take Log(N) queries.
    - In total, the whole process will take 3 Log(N) queries. we can optimize this into 2 Log(N) queries by reducing the possibility of 
	    the second part, after we have reduced the first part into a single node.
*/
#include "icc.h"
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back
#define all(x) (x).begin(),(x).end()
typedef pair<int,int> pii;
const int MAXN=100;
int par[MAXN+5],n;
int finds(int x){
	if(par[x]!=x)
		par[x]=finds(par[x]);
	return par[x];
}
bool connected(int u,int v){
	return (finds(u)==finds(v));
}
void gabung(int u,int v){
	u=finds(u),v=finds(v);
	if(u==v)
		return;
	par[v]=u;
}

int tanya(vector <int> a,vector <int> b){
	int arr1[105];int arr2[105];
	for(int i=0;i<(int) a.size();i++)
		arr1[i]=a[i];
	for(int i=0;i<(int) b.size();i++)
		arr2[i]=b[i];
	
	return query(a.size(),b.size(),arr1,arr2);
}
void expand(vector <int> &vec){		//vec is only filled with the head of components, we want to find the whole components
	bool root[MAXN+5];
	for(int i=1;i<=n;i++)
		root[i]=false;
	for(auto isi:vec)
	{
		root[isi]=true;
		assert(isi==par[isi]);
	}
	vec.clear();
	for(int i=1;i<=n;i++)
		if(root[finds(i)])
			vec.pb(i);
}
void masukkan(vector <int> &tampung,const vector <int> tambahan){
	for(int isi:tambahan)
		tampung.pb(isi);
}

void perkecil(vector <int> &now,const vector <int> &other){			//Query(now,other) is 1, we only need to find the right "now"
	vector <int> temp;
	while(now.size()>1)				//O(logN)
	{
		temp.clear();
		for(int i=now.size()/2;i<(int) now.size();i++)
			temp.pb(now[i]);
		now.resize(now.size()/2);
		
		if(tanya(now,other)==0)				//O(N)
			now=temp;
	}
	//when only one possibility exist, it must be the right now
}

void bagiDua(vector <int> &vec,int &lebih,vector<vector<int> > &tampung){
	int ambil=vec.size()/2;
	if(lebih&&(vec.size()&1))
		lebih--,ambil++;
	
	vector <int> kiri,kanan;
	for(int i=0;i<(int) vec.size();i++)
		(i<ambil)?kiri.pb(vec[i]):kanan.pb(vec[i]);
	tampung.pb(kiri);tampung.pb(kanan);
}
vector <int> kiri,kanan,possible[MAXN+5];
void findAnggota(){
	vector <vector <int> > anggota,baru;
	anggota.resize(1);
	for(int i=1;i<=n;i++)
	{
		if(par[i]==i)					//the head of component
			anggota[0].pb(i);
	}
	while(true)
	{
		baru.clear();
		int lebih=0;						//to Divide the odd element equally
		for(auto &isi:anggota)
			lebih+=isi.size()%2;
		lebih/=2;
		
		for(auto &isi:anggota)
			bagiDua(isi,lebih,baru);
		assert(lebih==0);
		
		kiri.clear();kanan.clear();
		for(int i=0;i<(int) baru.size();i++)
			(i&1)?masukkan(kanan,baru[i]):masukkan(kiri,baru[i]);
		assert(abs((int) kiri.size() - (int) kanan.size())<=1);
		
		expand(kiri),expand(kanan);
		if(tanya(kiri,kanan))
			break;
		
		anggota=baru;
	}
	
	//Find Possibility, For Optimization
	for(int i=0;i<(int) baru.size();i+=2)
	{
		assert(i+1<(int) baru.size());
		for(auto isi:baru[i])
		{
			possible[isi].clear();
			masukkan(possible[isi],baru[i+1]);
		}
	}
}
void run(int _n){
	n=_n;
	for(int i=1;i<=n;i++)
		par[i]=i;
	
	for(int i=0;i<n-1;i++)
	{
		findAnggota();
		
		perkecil(kiri,kanan);
		kanan=possible[finds(kiri[0])];							//reduce the other part possibility
		expand(kanan);
		perkecil(kanan,kiri);
		
		setRoad(kiri[0],kanan[0]);
		gabung(kiri[0],kanan[0]);
	}
}
