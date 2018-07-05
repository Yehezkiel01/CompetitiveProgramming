/*
Idea:
  Pre-requisites: Suffix Array, LCP (longest common prefix), Line Sweep, Binary Search, Rolling Hash.
   
  Main Algorithm:
  
  Assume the problem only wants to find the largest occurence value of substrings instead of palindromic substrings. It could be found
by using suffix array + lcp + stack. First, build the suffix array and lcp array. After that, for every i and j, where i<=j, update the
answer with lcp(SA[i]...SA[j])*(j-i+1). This process could be sped up by stack into O(N) instead of O(N^2). (HINT: the data inside the
stack should be strictly decreasing from its top to bottom)
  
  Let's get back to the original problem. It could be done in the same way above, if LCPP (longest common prefix palindrome) array are
built. Just replace every lcp in the above algorithm with lcpp and the problem is solved. 
  
  Build LCPP:
  
  To support fast lcpp building process, determining whether a certain range is palindrome in O(1) would be important, hence rolling
hash is needed (HINT: make one prefix hash sum and one suffix hash sum). Also, finding the largest substrings which is palindrome from a
predetermined midpoints in log(N) would be necessary, there are different cases for odd and even substrings as the number of substrings'
midpoints would be different too. find it with binary search + aforementioned rolling hash , so that the expected complexity are met.
  
  Basically, the idea is to find all j from every i, where i<j and S[i..j] is palindrome, afterward find the best j that could be used
for lcpp. Moreover, if an i has a list of valid j, then i+1 could use the list by reducing every value of j by 1, and erasing several j
which is no longer valid. This is supported by the fact that if a range S[l..r] is palindrome than the range S[l+1...r-1] would also be
a palindrome if l+1<=r-1. 
  
  not only fixing the previous list, but the list also need to be added with several new j, that is the palindrome started from the i-th
character to the j-th character and could not be extended to the (i-1)-th and (j+1)-th character. All of the additional range from every
i could be precomputed by trying every midpoints and extend it into the longest palindromic substrings in O(log N) as explained above.
  
  Furthermore, the list could be mantained with set, and finding the best j could be done with lcp and upper_bound, in other words, line
sweep algorithm is used. Finally, the value of j do not have to be reduced one by one as what I mentioned above, Instead, we mantain a
variabel that denotes the amount of minus for all of the integer inside the set.
  
  Overall Complexity is O(|S|log|S|) but, be careful with the constant factor if you are targeting the full mark. See the implementation
below for the details.
*/
#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back
#pragma GCC optimize("Ofast")
#pragma GCC optimize("O3")
typedef long long LL;
typedef pair<int,int> pii;
const int MAXN=300000;
int n;
char input[MAXN+5];

LL ans=0;
inline void update(LL newAns){
	if(newAns>ans)
		ans=newAns;
}

const int logn=19;
int SA[MAXN+5],RA[MAXN+5],tempSA[MAXN+5],tempRA[MAXN+5],cnt[MAXN+5];
inline bool cmpSA(int i,int j){
	return input[i]<input[j];
}
inline void inisSA(){
	for(int i=0;i<n;i++)
		SA[i]=i;
	sort(SA,SA+n,cmpSA);
	
	//rankingnya 1-based untuk memprioritaskan Null Character
	RA[SA[0]]=1;
	for(int i=1;i<n;i++)
	{
		RA[SA[i]]=RA[SA[i-1]];
		if(input[SA[i]]!=input[SA[i-1]])
			RA[SA[i]]++;
	}
}
inline int ranking(int idx){
	if(idx>=n)				//priority NULL Character
		return 0;
	return RA[idx];
}
inline void cSort(int tambah){
	for(int i=0;i<=n;i++)
		cnt[i]=0;
	for(int i=0;i<n;i++)
		cnt[ranking(i+tambah)]++;
	int sum=0;
	for(int i=0;i<=n;i++)
	{
		int temp=cnt[i];
		cnt[i]=sum;
		sum+=temp;
	}
	for(int i=0;i<n;i++)			//dimasukkan sesuai urutan SA
		tempSA[cnt[ranking(SA[i]+tambah)]++]=SA[i];
	for(int i=0;i<n;i++)
		SA[i]=tempSA[i];
}
inline void reRank(int tambah){
	tempRA[SA[0]]=1;
	for(int i=1;i<n;i++)
	{
		tempRA[SA[i]]=tempRA[SA[i-1]];
		if(!(ranking(SA[i])==ranking(SA[i-1])&&ranking(SA[i]+tambah)==ranking(SA[i-1]+tambah)))
			tempRA[SA[i]]++;
	}
	for(int i=0;i<n;i++)
		RA[i]=tempRA[i];
}
inline void buildSA(){
	inisSA();
	for(int i=0;i<logn;i++)
	{
		if((1<<i)>n)
			break;
		cSort(1<<i);
		cSort(0);
		reRank(1<<i);
	}
}

const int totalHash=1;
const LL mods=1000000007LL;
int modulo(LL angka){
	if(angka>=mods||angka<=-mods)
		angka%=mods;
	if(angka<0)
		angka+=mods;
	return angka;
}
LL base,value[26],pangkat[MAXN+5],pre[MAXN+5],suf[MAXN+5];
inline void buildHash(){
	base=37;
	
	for(int i=0;i<26;i++)
		value[i]=modulo( (LL) modulo(rand())* modulo(rand()));
	
	pangkat[0]=1;
	
	for(int i=1;i<=n;i++)
		pangkat[i]=modulo(pangkat[i-1]*base);
	
	suf[n]=0;
	
	for(int i=n-1;i>=0;i--)
		suf[i]=modulo(suf[i+1]*base+value[input[i]-'a']);
	
	pre[0]=value[input[0]-'a'];
	for(int i=1;i<n;i++)
		pre[i]=modulo(pre[i-1]*base+value[input[i]-'a']);
}

inline bool isPalin(int i,int j){
	if(!(0<=i&&j<n))			//out of range
		return false;
	
	LL temp1=modulo(suf[i]-suf[j+1]*pangkat[j-i+1]);
	LL temp2=pre[j];
	if(i)	temp2=modulo(temp2-pre[i-1]*pangkat[j-i+1]);
	
	return (temp1==temp2);
}

inline int expand(int i,int j){
	if(!isPalin(i,j))
		return 0;
	
	int bawah=1,atas=min(i,n-1-j),mid;
	while(bawah<=atas)
	{
		mid=(bawah+atas)>>1;
		if(isPalin(i-mid,j+mid))
			bawah=mid+1;
		else
			atas=mid-1;
	}
	return bawah;
}

int lcp[MAXN+5],urutan[MAXN+5];
inline void buildLCP(){
	for(int i=0;i<n;i++)
		urutan[SA[i]]=i;
	
	for(int i=0,LCP=0;i<n;i++,LCP-=(LCP==0)?0:1)
	{
		if(urutan[i])
		{
			int j=SA[urutan[i]-1];
			while(max(i,j)+LCP<n&&input[i+LCP]==input[j+LCP])
				LCP++;
			lcp[i]=LCP;
		}
		else
			lcp[i]=0;
	}
}

int lcpp[MAXN+5];
vector <int> masuk[MAXN+5],keluar[MAXN+5];
inline void buildLCPP(){				//sekalian yang seorang diri
	//case seorang diri
	int temp;
	for(int i=0;i<n;i++)
	{
		temp=expand(i-1,i);
		update(temp<<1);
		if(temp)
		{
			int tambah=i-1-(temp-1);
			masuk[i-1-(temp-1)].pb((i<<1)-1);
			keluar[i].pb((i<<1)-1);
		}
		
		temp=expand(i,i);
		update((temp<<1)-1);
		if(temp)
		{
			int tambah=i-(temp-1);
			masuk[i-(temp-1)].pb(i<<1);
			keluar[i+1].pb(i<<1);
		}
	}

	set<int> daftar;
	for(int i=0;i<n;i++)
	{
		for(auto isi:masuk[i])
			daftar.insert(isi);
		for(auto isi:keluar[i])
			daftar.erase(isi);
		auto it=daftar.upper_bound((i+lcp[i]-1)+i);
		if(it!=daftar.begin())
		{
			it=prev(it);
			lcpp[i]=(*it-i)-i+1;
		}
		else
			lcpp[i]=0;
	}
}

inline void findAns(){
	stack <pii> tumpuk;
	for(int i=1;i<n;i++)
	{
		int baru=lcpp[SA[i]];
		while(tumpuk.size()&&tumpuk.top().fi>=baru)
		{
			LL pos=i-1;
			LL tinggi=tumpuk.top().fi;
			tumpuk.pop();
			pos=pos-((tumpuk.size())?tumpuk.top().se:0)+1;
			update(pos*tinggi);
		}
		tumpuk.push(mp(baru,i));
	}
	
	while(tumpuk.size())
	{
		LL pos=n-1;
		LL tinggi=tumpuk.top().fi;
		tumpuk.pop();
		pos=pos-((tumpuk.size())?tumpuk.top().se:0)+1;
		update(pos*tinggi);
	}
}
int main()
{
	srand(2911);
	scanf("%s",&input);
	n=strlen(input);
	buildSA();
	buildHash();
	buildLCP();
	buildLCPP();
	findAns();
	cout<<ans<<endl;
}
