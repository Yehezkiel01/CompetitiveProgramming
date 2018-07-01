/*
  Idea:
    It is important to know that the total number of divisor from 1 to 300000 will be less than 4000000. It could be found from brute 
    force or just assume the upper bound is 300000 log 300000.
    
    Because the integer are added in increasing order, it would be easy to find the range, which contains only numbers between A and B 
    inclusively, from which we could gather the answer. In the end, after the range is known, run line sweep to find all the answer.
    
    Further explanation about the line sweep:
    Mantain the number of divisor in another array. Loop the integers in the order they are added, find all the divisor of the integer 
    using sieve, finally, find all the information needed to answer the queries.
*/
#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(x) (x).begin(),(x).end()
#define cerr if(false)cerr
typedef pair<int,int> pii;
const int MAXQ=300000;
const int MAXVAL=300000;
int value[MAXQ+5],pembagi[MAXVAL+5],q,n=0;
string command[MAXQ+5];
void sieve(){
	for(int i=1;i<=MAXVAL;i++)
		pembagi[i]=i;
	for(int i=2;i*i<=MAXVAL;i++)
	{
		if(pembagi[i]!=i)
			continue;
		for(int j=i*i;j<=MAXVAL;j+=i)
		{
			if(pembagi[j]==j)
				pembagi[j]=i;
		}
	}
}

int lower(int angka){
	return lower_bound(value+1,value+n+1,angka)-value;
}

struct trio{
	int id,m,kali;
	trio(){
		
	}
	trio(int _m,int _id,int _kali){
		m=_m;
		id=_id;
		kali=_kali;
	}
	void doIt();
};
int divisor[MAXVAL+5]={},ans[MAXQ+5]={};

void trio::doIt(){
	ans[id]+=kali*divisor[m];
}
vector <trio> pending[MAXQ+5];

vector <pii> daftar;
void isiDaftar(int angka){
	daftar.clear();
	while(angka>1)
	{
		if(daftar.size()&&pembagi[angka]==daftar.back().fi)
			daftar.back().se++;
		else
			daftar.eb(pembagi[angka],1);
		angka/=pembagi[angka];
	}
}
int occ=0;
void update(int idx,int angka){
	if(idx==daftar.size())
	{
		occ++;
		divisor[angka]++;
	}
	else
	{
		for(int i=0;i<=daftar[idx].se;i++,angka*=daftar[idx].fi)
			update(idx+1,angka);
	}
}
int main()
{
	sieve();
	cin>>q;
	for(int kweery=1;kweery<=q;kweery++)
	{
		cin>>command[kweery];
		if(command[kweery]=="ADD")
		{
			int val;
			cin>>val;
			value[++n]=val;
			
			if(n>1)	
				assert(value[n]>value[n-1]);
		}
		else if(command[kweery]=="ASK")
		{
			int a,b,m;
			cin>>a>>b>>m;
			int l=lower(a);
			int r=lower(b);
			if(r==n+1||value[r]>b)
				r--;
			pending[l-1].push_back(trio(m,kweery,-1));
			pending[r].push_back(trio(m,kweery,1));
		}
	}
	//just ignore pending[0];
	for(int i=1;i<=n;i++)
	{
		isiDaftar(value[i]);
		update(0,1);
		for(auto isi:pending[i])
			isi.doIt();
	}
	for(int i=1;i<=q;i++)
	{
		if(command[i]=="ASK")
			cout<<ans[i]<<endl;
	}
}
