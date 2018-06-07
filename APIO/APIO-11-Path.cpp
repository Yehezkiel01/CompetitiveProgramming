/*
	Idea:
		The most important part is realizing that there are only little important points, therefore they could be compressed 
		and we only need to run dijkstra. 
		
		The important points include all the Deehive corner, the office, and the home. 
		if we could move from deehive X to deehive Y from a certain part or segment of the deehive X edge and deehive Y edge,
		these part endpoints are also considered as important points. In the end, we make edge between those important points, 
		if we could move from a point to the other point while respecting the rules. 
*/
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back
#define all(x) (x).begin(),(x).end()
typedef pair<int,int> pii;
typedef long long LL;
const int MAXN=1000;
const int MAXNODE=12*MAXN;

int tc;

pii awal,akhir;
map <pii,int> press;
vector <pair<int,LL> > node[MAXNODE+5];
LL ans;
int nilai(pii titik){
	if(press.find(titik)==press.end())
	{
		int temp=press.size();
		node[temp].clear();
		press[titik]=temp;
	}
	assert(press.size()<=MAXNODE);
	return press[titik];
}

struct rect{
	int x1,y1,x2,y2;
	rect(){
		
	}
	rect(pii p1,pii p2){
		x1=min(p1.fi,p2.fi);
		x2=max(p1.fi,p2.fi);
		
		y1=min(p1.se,p2.se);
		y2=max(p1.se,p2.se);
	}
	void reverse(){
		swap(x1,y1);
		swap(x2,y2);
	}
	
	bool prioritas() const{
		return (x1==x2&&y1==y2);
	}
	
	bool operator < (const rect &other) const{
		if(x1!=other.x1)
			return x1<other.x1;
		return prioritas()>other.prioritas();
	}
	
	bool inside(pii pos){
		return (x1<pos.fi&&pos.fi<x2&&y1<pos.se&&pos.se<y2);
	}
};
vector <rect> kotak;

struct seg{
	int l,r,x;
	seg(){
	}
	seg(int _l,int _r,int _x){
		l=_l;r=_r;x=_x;
	}
	bool operator < (const seg &other) const{
		if(l!=other.l)
			return l<other.l;
		return r<other.r;
	}
};
int tukar;
set<seg> daftar;
LL dist(pii p1,pii p2){
	return (LL) abs(p1.fi-p2.fi)+abs(p1.se-p2.se);
}
void buatEdge(pii p1,pii p2){
	if(p1==p2)
		return;
	if(tukar)
		swap(p1.fi,p1.se);
	if(tukar)
		swap(p2.fi,p2.se);
	int u=nilai(p1);
	int v=nilai(p2);
	
	node[u].eb(v,dist(p1,p2));
	node[v].eb(u,dist(p1,p2));
}
void buatEdge(seg seg1,seg seg2){
	pii a,b;
	a=mp(seg1.x,max(seg1.l,seg2.l));
	b=mp(seg2.x,max(seg1.l,seg2.l));
	buatEdge(a,b);
	
	a=mp(seg1.x,min(seg1.r,seg2.r));
	b=mp(seg2.x,min(seg1.r,seg2.r));
	buatEdge(a,b);
}
void buatEdge(seg baru){				//sekaligus basmi yang intersect
	int l=baru.l,xl;
	int r=baru.r,xr;
	
	auto it=daftar.lower_bound(seg(baru.l,baru.r,-1));
	
	if(it!=daftar.begin()&&prev(it)->r>=baru.l)				//karena beririsan
		it=prev(it);
	
	while(it!=daftar.end()&&it->l<=baru.r)			//while intersect
	{
		buatEdge(*it,baru);
		
		if(it->l<l)
		{
			l=it->l;
			xl=it->x;
		}
		
		if(it->r>r)
		{
			r=it->r;
			xr=it->x;
		}
		
		it=next(it);
		daftar.erase(prev(it));
	}
	
	if(l<baru.l)
		daftar.insert(seg(l,baru.l-1,xl));
	if(baru.r<r)
		daftar.insert(seg(baru.r+1,r,xr));
}
void masukkan(int id){
	seg now;
	now.l=kotak[id].y1;
	now.r=kotak[id].y2;
	now.x=kotak[id].x1;
	
	buatEdge(now);
	
	now.x=kotak[id].x2;
	daftar.insert(now);
}
void construct(){
	sort(all(kotak));
	daftar.clear();
	for(int i=0;i<kotak.size();i++)
		masukkan(i);
}


vector<pii> tampung;
void gabung(int x,int y1,int y2){
	if(y1==y2)
		return;
	auto it=lower_bound(all(tampung),mp(x,y1));
	assert(*it==mp(x,y1));
	
	pii last=mp(x,y1);
	it=next(it);
	while(it!=tampung.end()&&(*it).se!=y2)
	{
		buatEdge(last,*it);
		last=*it;
		
		it=next(it);
	}
	
	assert(*it==mp(x,y2));
	buatEdge(last,*it);
}
void tambahEdge(){
	for(auto isi:press)
		tampung.pb(isi.fi);
	for(tukar=0;tukar<2;tukar++)
	{
		sort(all(tampung));
		for(auto isi:kotak)
		{
			gabung(isi.x1,isi.y1,isi.y2);
			gabung(isi.x2,isi.y1,isi.y2);
		}
		
		for(auto &isi:tampung)
			swap(isi.fi,isi.se);
		for(auto &isi:kotak)
			isi.reverse();
	}
}


const LL INF=100000000000000LL;
LL jarak[MAXNODE+5];
LL dijkstra(int s,int t){
	int n=press.size();
	for(int i=0;i<n;i++)
		jarak[i]=INF;
	
	priority_queue <pair<LL,int> > cari;
	jarak[s]=0;
	cari.push(mp(-jarak[s],s));
	while(!cari.empty())
	{
		pair<LL,int> u=cari.top();
		cari.pop();
		if(jarak[u.se]!=-u.fi)
			continue;
		u.fi=-u.fi;
		for(auto v:node[u.se])
		{
			if((LL) u.fi+v.se<jarak[v.fi])
			{
				jarak[v.fi]=u.fi+v.se;
				cari.push(mp(-jarak[v.fi],v.fi));
			}
		}
	}
	
	if(jarak[t]==INF)
		return -1;
	else
		return jarak[t];
}
void inis(){
	tampung.clear();
	press.clear();
	kotak.clear();
	ans=0;
}
int main()
{
	scanf("%d",&tc);
	for(int test=1;test<=tc;test++)
	{
		inis();
		
		scanf("%d%d",&awal.fi,&awal.se);
		scanf("%d%d",&akhir.fi,&akhir.se);
		nilai(awal);
		nilai(akhir);
		kotak.pb(rect(awal,awal));
		kotak.pb(rect(akhir,akhir));
		
		int n;
		scanf("%d",&n);
		for(int i=0;i<n;i++)
		{
			pii p1,p2;
			scanf("%d%d%d%d",&p1.fi,&p1.se,&p2.fi,&p2.se);
			
			nilai(mp(p1.fi,p1.se));nilai(mp(p1.fi,p2.se));
			nilai(mp(p2.fi,p1.se));nilai(mp(p2.fi,p2.se));
			
			kotak.eb(p1,p2);
			
			assert(!(kotak.back().inside(awal)||kotak.back().inside(akhir)));
		}
		
		for(tukar=0;tukar<2;tukar++)
		{
			construct();
			for(auto &isi:kotak)
				isi.reverse();
		}
		
		tambahEdge();
		
		ans=dijkstra(nilai(awal),nilai(akhir));
		if(ans==-1)
			printf("No Path\n");
		else
			printf("%lld\n",ans);
	}
}
