#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#define P pair<int,int>
#define mp make_pair
#define fi first
#define se second
#define C ch=getch()
#define N 5
#define M 30010
using namespace std;

int n=3;
char ch;

namespace Move
{
	int x,y;
	char player_pos='#';
	inline void init(){x=y=1;}
	inline void work(char u)
	{
		if(u=='w' && x>1) x--;
		if(u=='s' && x<n) x++;
		if(u=='a' && y>1) y--;
		if(u=='d' && y<n) y++;
	}
}

namespace Qp
{
	char mm[N][N];
	inline void init()
	{
		int i,j;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				mm[i][j]='.';
			}
		}
	}
	inline void out()
	{
		system("cls");
		int i,j;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				if(i==Move::x && j==Move::y) printf("%c",Move::player_pos);
				else printf("%c",mm[i][j]);
			}
			puts("");
		}
	}
	inline int play(int u,int v,bool w)
	{
		if(mm[u][v]!='.') return -1;
		int i,j;
		if(!w) mm[u][v]='O';
		else mm[u][v]='X';
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++) if(mm[i][j]!='O') break;if(j>n) return 1;
			for(j=1;j<=n;j++) if(mm[i][j]!='X') break;if(j>n) return 2;
			for(j=1;j<=n;j++) if(mm[j][i]!='O') break;if(j>n) return 1;
			for(j=1;j<=n;j++) if(mm[j][i]!='X') break;if(j>n) return 2;
		}
		for(i=1;i<=n;i++) if(mm[i][i]!='O') break;if(i>n) return 1;
		for(i=1;i<=n;i++) if(mm[i][i]!='X') break;if(i>n) return 2;
		for(i=1;i<=n;i++) if(mm[i][n-i+1]!='O') break;if(i>n) return 1;
		for(i=1;i<=n;i++) if(mm[i][n-i+1]!='X') break;if(i>n) return 2;
		return 0;
	}
	inline int get()
	{
		int i,j,t=1,res=0;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				if(mm[i][j]=='O') res+=t;
				else if(mm[i][j]=='X') res+=t*2;
				t*=3;
			}
		}
		return res;
	}
}

namespace Cpu
{
	int dp[M][3],mm[N][N];
	vector<P> to[M][3];
	inline void init(){memset(dp,-1,sizeof(dp));}
	inline void get(int u)
	{
		int i,j,t=1;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				int tmp=u/t%3;
//				cout<<" "<<u<<" "<<t<<" "<<tmp<<endl;
				mm[i][j]=tmp;
				t*=3;
			}
		}
	}
	inline int get()
	{
		int i,j,t=1,res=0;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				res+=mm[i][j]*t;
				t*=3;
			}
		}
		return res;
	}
	inline int win()
	{
		int i,j;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++) if(mm[i][j]!=1) break;if(j>n) return 1;
			for(j=1;j<=n;j++) if(mm[i][j]!=2) break;if(j>n) return 2;
			for(j=1;j<=n;j++) if(mm[j][i]!=1) break;if(j>n) return 1;
			for(j=1;j<=n;j++) if(mm[j][i]!=2) break;if(j>n) return 2;
		}
		for(i=1;i<=n;i++) if(mm[i][i]!=1) break;if(i>n) return 1;
		for(i=1;i<=n;i++) if(mm[i][i]!=2) break;if(i>n) return 2;
		for(i=1;i<=n;i++) if(mm[i][n-i+1]!=1) break;if(i>n) return 1;
		for(i=1;i<=n;i++) if(mm[i][n-i+1]!=2) break;if(i>n) return 2;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				if(!mm[i][j])
					return 0;
			}
		}
		return 3;
	}
	int ask(int u,int v)
	{
		if(dp[u][v]!=-1) return dp[u][v];
		get(u);
		int w=win();
		if(w==3) return dp[u][v]=3;
		if(w==1) return v==1?dp[u][v]=1:dp[u][v]=2;
		if(w==2) return v==1?dp[u][v]=2:dp[u][v]=1;
		int i,j,t,r,res=0;
		P gg;
		dp[u][v]=2;
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				get(u);
				if(mm[i][j]) continue;
				gg=mp(i,j);
				mm[i][j]=v;
				t=get(),r=ask(t,3-v);
				if(r==2)
				{
					if(res!=2) to[u][v].clear(),res=2;
					to[u][v].push_back(mp(i,j));
					dp[u][v]=1;
				}
				else if(r==3)
				{
					if(res==2) continue;
					if(res!=3) to[u][v].clear(),res=3;
					to[u][v].push_back(mp(i,j));
					dp[u][v]=3;
				}
			}
		}
		if(res>=2) return dp[u][v];
		get(u);
		for(i=1;i<=n;i++) for(j=1;j<=n;j++) if(!mm[i][j]) to[u][v].push_back(mp(i,j));
		return 2;
	}
	inline P play(int u,int v)
	{
		ask(u,v);
		return to[u][v][rand()%to[u][v].size()];
	}
}

inline void init()
{
	Qp::init();
	Move::init();
}

int main()
{
	srand(time(0));
	Cpu::init();
	int i,j,t,res;
	bool xh;
	while(1)
	{
		init();
		puts("choose£º");
		scanf("%lld",&xh);
		bool chess=0,now=xh;
		Qp::out();
		for(i=1;i<=n*n;i++)
		{
			if(now)
			{
				for(C;;C)
				{
					if(ch=='w' || ch=='a' || ch=='s' || ch=='d')
					{
						Move::work(ch);
						Qp::out();
					}
					else if(ch==' ' || ch=='q')
					{
						res=Qp::play(Move::x,Move::y,chess);
						if(res!=-1) break;
					}
				}
			}
			else
			{
				Sleep(100);
				P pos=Cpu::play(Qp::get(),chess+1);
				res=Qp::play(pos.fi,pos.se,chess);
				if(res==-1) puts("mdzz"),system("pause");
			}
			Qp::out();
			if(res) break;
			chess^=1;
			now^=1;
		}
		if(i>n*n) puts("Draw");
		else if(res+xh==2) puts("Player Wins!");
		else puts("Cpu Wins!");
		system("pause");
		system("cls");
	}
}
