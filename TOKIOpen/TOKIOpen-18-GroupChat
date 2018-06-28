/*
  Idea:
                  /\
                 /  \
                /    \
                \    /            This 2D figure is a square that is rotated by 45 degree.
                 \  /
                  \/
    From observations, it could be noticed that every 2 points that are located inside the figure will have manhattan distance less than
    the square's diagonal. Therefore, iff the minimum delay is X, it should be true that we could cover all the points using two of this
    figure whose diagonal are X. Rotate all the coordinate by 45 degrees using (x+y,x-y) formula and do binary search to find the right
    X. Let imagine that after rotation, there is a smallest rectangle, whose side parallel to the X axis, that covers all the points.
    In order to check whether an X is valid or not, we only need to put the two square at two opposite corner of the rectangle and check 
    all the other points in O(N). It is considered valid when every points are included in the two squares.
*/
#include "group.h"
#include <bits/stdc++.h>
#include <vector>
using namespace std;
#define fi first
#define se second
#define mp make_pair
typedef long long LL;
const int MAXN=100000;
int n,x[MAXN+5],y[MAXN+5];
struct rect{
	LL x1,y1,x2,y2;
	rect(){}
	rect(LL x,LL y,LL size){					//harus ujung kiri bawah
		x1=x;
		y1=y;
		x2=x+size;
		y2=y+size;
	}
	rect(LL x,LL y){
		x1=x2=x;
		y1=y2=y;
	}
	void expand(LL x,LL y){
		x1=min(x,x1);
		x2=max(x,x2);
		y1=min(y,y1);
		y2=max(y,y2);
	}
	
	bool inside(LL x,LL y){
		return (x1<=x&&x<=x2&&y1<=y&&y<=y2);
	}
	
	pair<rect,rect> produce(LL size,int jenis){
		if(jenis)
			return mp(rect(x1,y2-size,size),rect(x2-size,y1,size));
		else
			return mp(rect(x1,y1,size),rect(x2-size,y2-size,size));
	}
};

rect kotakAsli;
bool terbagiDua(LL size){
	for(int ulang=0;ulang<2;ulang++)
	{
		bool terbagi=true;
		pair<rect,rect> daerah=kotakAsli.produce(size,ulang);
		for(int i=0;i<n;i++)
		{
			if(!(daerah.fi.inside(x[i],y[i])||daerah.se.inside(x[i],y[i])))
				terbagi=false;
		}
		if(terbagi)
			return true;
	}
	return false;
}
int getMinimumDelay(int N,vector<int> X,vector<int> Y) {
	n=N;
	for(int i=0;i<n;i++)
	{
		x[i]=X[i]-Y[i];
		y[i]=X[i]+Y[i];
	}
	
	kotakAsli=rect(x[0],y[0]);
	for(int i=1;i<n;i++)
		kotakAsli.expand(x[i],y[i]);
	
	LL bawah=0,atas=2000000000,mid;
	while(bawah<=atas)
	{
		mid=(bawah+atas)>>1;
		if(terbagiDua(mid))
			atas=mid-1;
		else
			bawah=mid+1;
	}
	return bawah;
}
