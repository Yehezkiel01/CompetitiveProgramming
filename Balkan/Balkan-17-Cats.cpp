/*
	Idea:
	We need to reduce the problem into a simpler problem.
	
	First, assume there is no lion, and we group every consecutive cats and dogs. 
	One of the observation is that if we move a cat or dog, we will also move all of their consecutive friends. 
	
	After that we only need to find a way to merge several group of cats and several group of dogs by moving them 
	and also put some separator, the lion. Normally, putting seperator cost 1 movement, however, there are exception
	for several consecutive cats or dogs who don't need additional cost to put the separator
	because at first ,before any move was done, there is at least a lion besides the group.
	
	Do some precomputation to find the groups and also the special groups,groups that don't need additional cost 
	to put separator before them. Then, we run dp with assumption that if we move a group X to join the group Y 
	(a group that won't be moved), we may assume that we remove X from the array and it will be inserted somewhere save.
	Be careful with the case that all of the cats or dogs are removed, it means that they couldn't join anyone, 
	therefore there might be additional cost.
	
	My dp has 3 states, the position, the number of group that is not removed, and boolean that determine 
	whether this is the first group or not (this is important because the transition will be affected).
	In order to reduce the memory, we need to use flying table.
*/
#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define eb emplace_back
const int MAXN=5000;
const int INF=MAXN+5;
int tc,n,arr[MAXN+5],tambahSatu;
vector <int> weight,tambah;

bool homo(){
	int jenis=-1;
	for(int i=1;i<=n;i++)
	{
		if(arr[i]==2)
			continue;
		if(jenis==-1)
			jenis=arr[i];
		if(jenis!=arr[i])
			return false;
	}
	return true;
}
bool possible(){
	for(int i=1;i<=n;i++)
	{
		if(arr[i]==2)
			return true;
	}
	return false;
}
int total2(){
	int ret=0;
	for(int i=1;i<=n;i++)
	{
		if(arr[i]==2)
			ret++;
	}
	return ret;
}
void kasusKhusus(){
	if(arr[1]==2||arr[n]==2)
		tambahSatu=0;
	for(int i=2;i<=n;i++)
	{
		if(arr[i]==arr[i-1]&&arr[i-1]==2)
			tambahSatu=0;
	}
}

void perbaikiInput(){
	int temp=0,jenis=-1,_tambah=1;
	for(int i=1;i<=n;i++)
	{
		if(arr[i]==2)
			continue;
		if(jenis==-1)
		{
			if(i!=1&&arr[i-1]==2)
				_tambah=0;
			jenis=arr[i];
		}
		
		if(jenis!=arr[i])
		{
			weight.pb(temp);
			tambah.pb(_tambah);
			
			jenis=arr[i];
			temp=0;
			_tambah=1;
			
			if(i!=1&&arr[i-1]==2)
				_tambah=0;
		}
		temp++;
	}
	weight.pb(temp);
	tambah.pb(_tambah);
	
	n=weight.size();
}

int dp[3][MAXN+5][2],id[3]={0,1,2};
void shift(){
	int temp=id[2];
	id[2]=id[1];
	id[1]=id[0];
	id[0]=temp;
}
void bottomup(){
	for(int ulang=0;ulang<2;ulang++)
	{
		for(int i=0;i<=n;i++)
			dp[id[0]][i][0]=dp[id[0]][i][1]=(i==0)?0:INF;
		shift();
	}
	
	for(int i=n-1;i>=0;i--)
	{
		for(int j=0;j<=n;j++)
		{
			for(int k=0;k<2;k++)
			{
				dp[id[0]][j][k]=INF;
				if(k)			//berarti pertama
				{
					if(j)			//case dia dak dipindahkan
					{
						dp[id[0]][j][k]=min(dp[id[0]][j][k],dp[id[1]][j-1][0]);
					}
					//case dia dipindahkan
					{
						dp[id[0]][j][k]=min(dp[id[0]][j][k],dp[id[1]][j][1]+weight[i]);
					}
				}
				else
				{
					if(j)			//case dia dak dipindahkan
					{
						dp[id[0]][j][k]=min(dp[id[0]][j][k],dp[id[1]][j-1][0]+tambah[i]);			//dia butuh pemisah tambahan antara dia dan sebelumnya
					}
					
					//case dia dipindahkan
					{
						dp[id[0]][j][k]=min(dp[id[0]][j][k],dp[id[2]][j][0]+weight[i]);				//depannya sudah gak perlu dipindahkan lagi
					}
				}
			}
		}
		if(i)
			shift();
	}
	
	for(int j=0;j<=n;j++)
		for(int k=0;k<2;k++)
			dp[id[1]][j][k]=dp[id[2]][j][k]=dp[id[0]][j][k];
}
void inis(){
	weight.clear();
	tambah.clear();
	tambahSatu=1;
}
int main()
{
	scanf("%d",&tc);
	for(int test=1;test<=tc;test++)
	{
		inis();	
		scanf("%d",&n);
		for(int i=1;i<=n;i++)
			scanf("%d",&arr[i]);
		if(homo())
		{
			printf("0\n");
			continue;
		}
		
		if(!possible())
		{
			printf("-1\n");
			continue;
		}
		kasusKhusus();
		int maxTarget=total2()+1;
		perbaikiInput();
		assert(n>=2);
		bottomup();
		int ans=INF;
		for(int i=2;i<=min(n,maxTarget);i++)
			ans=min(ans,dp[0][i][1]);
		ans=min(ans,dp[0][1][1]+tambahSatu);
		assert(ans!=INF);
		printf("%d\n",ans);
	}
}
