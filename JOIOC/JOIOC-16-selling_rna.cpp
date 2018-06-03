#include <bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(),(x).end()
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define mp make_pair
typedef pair<int,int> pii;
const int MAXQ=100000;
int n,q,ans[MAXQ+5];
string s[MAXQ+5],t[MAXQ+5];
string balik(string s){
	int panjang=s.length();
	string ret="";
	for(int i=panjang-1;i>=0;i--)
		ret+=s[i];
	return ret;
}

struct nodeTrie{
	int occ;
	list<pair<nodeTrie *,char> > anak;
	nodeTrie(){
		occ=0;
	}
	~nodeTrie(){
		anak.clear();
	}
	
};
class inTrie{
	private:
	int sumLength;
	nodeTrie *head;
	
	public:
	inTrie(){
		sumLength=0;
		head=new(nodeTrie);
	}
	
	void add(string s){
		int panjang=s.length();
		sumLength+=panjang;
		
		auto now=head;
		for(int i=0;i<panjang;i++)
		{
			auto it=now->anak.begin();
			while(it!=now->anak.end() && it->se<s[i])
				it=next(it);
			if(it==now->anak.end()||it->se>s[i])
				it=now->anak.insert(it,mp(new(nodeTrie),s[i]));//cout<<"Buat baru"<<endl;
			now=it->fi;
			
			now->occ++;
		}
	}
	
	int query(string s){
		int panjang=s.length();
		
		auto now=head;
		for(int i=0;i<panjang;i++)
		{
			auto it=now->anak.begin();
			while(it!=now->anak.end()&&it->se<s[i])
				it=next(it);
			if(it==now->anak.end()||it->se>s[i])
				return 0;
			now=it->fi;
		}
		
		return now->occ;
	}
	
	void merge (nodeTrie *bos,nodeTrie *bawahan){			//after merge di delete
		bos->occ+=bawahan->occ;
		auto it1=bos->anak.begin();
		auto it2=bawahan->anak.begin();
		
		while(it1!=bos->anak.end())
		{
			while(it2!=bawahan->anak.end()&&it1->se > it2->se)
			{
				bos->anak.insert(it1,*it2);
				it2=next(it2);
			}
			if(it2==bawahan->anak.end())
				break;
			if(it1->se==it2->se)
			{
				merge(it1->fi,it2->fi);
				it2=next(it2);
			}
			
			it1=next(it1);
		}
		
		while(it2!=bawahan->anak.end())
		{
			bos->anak.insert(it1,*it2);
			it2=next(it2);
		}
		
		delete bawahan;
	}
	
	void operator +=(inTrie other){
		if(other.sumLength>sumLength)
		{
			swap(other.sumLength,sumLength);
			swap(other.head,head);
		}
		
		merge(head,other.head);
	}
};
struct bigNodeTrie{
	bool gabung;
	inTrie trie;
	list< pair<bigNodeTrie*,char> > anak;
	
	bigNodeTrie(){
		gabung=false;
	}
	
	inTrie merge(){
		if(gabung)
			return trie;
		gabung=true;
		for(auto isi:anak)
			trie+=isi.fi->merge();
		return trie;
	}
};
class outTrie{
	private:
	bigNodeTrie *head;
	bigNodeTrie *pos;
	public:
	
	outTrie(){
		head=new(bigNodeTrie);
	}
	
	bool cariPath(string s,bool buat){
		int panjang=s.length();
		
		pos=head;
		for(int i=0;i<panjang;i++)
		{
			auto it=pos->anak.begin();
			while(it!=pos->anak.end()&& it->se < s[i])
				it=next(it);
			if(it==pos->anak.end()||it->se!=s[i])
			{
				if(!buat)
					return false;
				it=pos->anak.insert(it,mp(new(bigNodeTrie),s[i]));
			}
			pos=it->fi;
		}
		return true;
	}
	void add(string s){
		assert(cariPath(s,true));
		pos->trie.add(balik(s));
	}
	int query(string s,string t){
		if(!cariPath(s,false))
			return 0;
		return pos->merge().query(balik(t));
	}
};
outTrie tampung;

vector <int> urutan;
bool cmpQuery(int a,int b){			//biar dari yang paling dalam
	return s[a].length()>s[b].length();
}
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);cout.tie(0);
	
	cin>>n>>q;
	
	for(int i=1;i<=n;i++)
	{
		string input;
		cin>>input;
		tampung.add(input);					//update di outTrie
	}
	
	for(int i=1;i<=q;i++)
	{
		cin>>s[i]>>t[i];
		urutan.pb(i);
	}
	sort(all(urutan),cmpQuery);
	for(auto isi:urutan)
		ans[isi]=tampung.query(s[isi],t[isi]);			//dijawab di Trie
	
	for(int i=1;i<=q;i++)
		cout<<ans[i]<<endl;
}
