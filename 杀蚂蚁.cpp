#include<iostream>
#include<cstdio>
#include<cmath>
#include<ctime>
#include<conio.h>
#include<windows.h>
#define db double
#define ll long long
#define beg 0
#define end 20001
#define eps 1e-10
#define C ch=getch()
#define N 200100
#define M 110
COORD coord= {0,0};
HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;

ll m,n,s,T=1,TT,tt,dir[6][2]= {0,1,1,0,0,-1,-1,0,0,1,1,0},xxs[M][M],nxt[N],last[N];
ll ca,cake,mon=500,need=200,c_need,fs,sh[N],dj[N],cst[N];
db r[N];
//xxs记录蚂蚁留下的信息素,dir数组表示蚂蚁的移动的几个方向
//nxt和last两个数组表示链表的指针，记录蚂蚁
//ca表示蚂蚁数量，cake表示持有蛋糕的蚂蚁
//mon表示钱的数量，need表示初始炮塔价格，c_need表示炸弹价格，fs表示分数
//sh,dj,cst,r三个数组分别表示炮塔的伤害，等级，总价和攻击半径
bool use[M][M],ZT;
//use记录该点是否已被蚂蚁占用，ZT表示是否暂停
char ch;

class Node
{
public:
	ll x,y;
	Node operator - (const Node &u) const//两点相减
	{
		Node res;
		res.x=x-u.x;
		res.y=y-u.y;
		return res;
	}
	db operator *(const Node &u) {return x*u.x+y*u.y;}//两点点积
} now;
inline db po(db u,ll v)//乘法快速幂
{
	db res=1;
	for(;v;)
	{
		if(v&1) res*=u;
		v>>=1;
		u*=u;
	}
	return res;
}
class Ant
{
public:
	ll xl,sx,old,id;//分别表示蚂蚁的血量，总血量，年龄和编号
	bool have;//记录蚂蚁手里是否有蛋糕
	Node pos,bef;//当前位置和上一步的位置
	bool ok(ll u,ll v)//判断该点是否合法
	{
		if(u<0 || u>m || v<0 || v>n || use[u][v] || u==bef.x&&v==bef.y) return 0;
		return 1;
	}
	void add(ll u)//新出现一只蚂蚁，初始化其各个数值，同时插入链表
	{
		ca++;
		id=u;
		c_need=po(1.1,u/6)*200;
		pos.x=pos.y=0;
		bef=pos;
		use[0][0]=1;
		xl=sx=po(1.1,(u+5)/6)*10;
		nxt[last[u]=last[end]]=u;
		last[nxt[u]=end]=u;
	}
	void GG()//该蚂蚁死亡，计算奖励，同时在链表中删除
	{
		mon+=pow((id+5)/6,1.2);
		ca--;
		fs+=xl;
		if(have) cake=0;
		use[pos.x][pos.y]=0;
		nxt[last[id]]=nxt[id];
		last[nxt[id]]=last[id];
	}
	void move()//蚂蚁移动一步
	{
		ll i,d=0,p,q,mx=-1;
		use[pos.x][pos.y]=0;
		if(!have)
		{
			for(i=0; i<4; i++)
			{
				p=pos.x+dir[i][0];
				q=pos.y+dir[i][1];
				if(!ok(p,q)) continue;
				if(xxs[p][q]>mx)
				{
					mx=xxs[p][q];
					d=i;
				}
			}
		}
		else
		{
			for(i=2; i<6; i++)
			{
				p=pos.x+dir[i][0];
				q=pos.y+dir[i][1];
				if(!ok(p,q)) continue;
				if(xxs[p][q]>mx)
				{
					mx=xxs[p][q];
					d=i;
				}
			}
			if(d>=4) d-=4;
		}
		if(mx!=-1)
		{
			if(old%5==4)
			{
				for(i=(!d?3:d-1);; i=(!i?3:i-1))
				{
					p=pos.x+dir[i][0];
					q=pos.y+dir[i][1];
					if(ok(p,q)) break;
				}
				bef=pos;
				pos.x=pos.x+dir[i][0];
				pos.y=pos.y+dir[i][1];
			}
			else
			{
				bef=pos;
				pos.x=pos.x+dir[d][0];
				pos.y=pos.y+dir[d][1];
			}
		}
		else bef=pos;
		use[pos.x][pos.y]=1;
		if(pos.x==m && pos.y==n && !cake) cake=id,have=1,xl=min(xl+sx/2,sx);
	}
} ant[N];
Node pt[N];//记录炮塔位置

inline db lenf(Node u,Node v){return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y);}//两点间距离的平方
inline db len(Node u,Node v){return sqrt(lenf(u,v));}//两点间距离
inline bool in(ll u,ll v){return lenf(ant[u].pos,pt[v])<r[v]*r[v]+eps;}//判断蚂蚁u是否在炮塔v的攻击范围内
inline db cj(Node u,Node v){return u.x*v.y-u.y*v.x;}//两点的叉积

inline db dis(Node u,Node v,Node w)//计算w到线段uv的最短距离
{
	if((w-u)*(v-u)>eps && (w-v)*(u-v)>eps)
	{
		return fabs(cj(w-u,v-u))/len(u,v);
	}
	else return sqrt(min(lenf(u,w),lenf(v,w))+eps);
}

inline void ata(ll u,ll v)//计算炮塔u打向蚂蚁v的沿路伤害
{
	ll i,j;
	for(i=nxt[beg]; i!=end; i=nxt[i])
	{
		if(dis(pt[u],ant[v].pos,ant[i].pos)-eps<0.5)
		{
			ant[i].xl-=sh[u];
			fs+=sh[u];
		}
	}
}

inline void out()//输出地图
{
	SetConsoleCursorPosition(hOutput, coord);
	ll i,j;
	char mm[M][M];
	for(i=0; i<=m; i++)
	{
		for(j=0; j<=n; j++)
		{
			mm[i][j]='.';
		}
	}
	for(i=nxt[beg]; i!=end; i=nxt[i]) mm[ant[i].pos.x][ant[i].pos.y]='A';//将蚂蚁加入地图
	for(i=1; i<=s; i++) mm[pt[i].x][pt[i].y]='X';//将炮塔加入地图
	if(!cake) mm[m][n]='C';
	else mm[ant[cake].pos.x][ant[cake].pos.y]='C';//将蛋糕加入地图
	mm[now.x][now.y]='O';//将光标加入地图
	for(i=0; i<=m; i++)
	{
		for(j=0; j<=n; j++)
		{
			printf("%c",mm[i][j]);
		}
		puts("");
	}
	cout<<"money:"<<mon;//输出钱的数量
	for(i=1; i<=s; i++)
	{
		if(now.x==pt[i].x&&now.y==pt[i].y) break;
	}
	if(i<=s) cout<<"   range:"<<r[i]<<"   "<<"attack:"<<sh[i]<<"   "<<"need:"<<(dj[i]+1)*(dj[i]+1)*10;//光标处有炮塔，输出炮塔信息
	else
	{
		for(i=nxt[beg]; i!=end; i=nxt[i])
		{
			if(ant[i].pos.x==now.x&&ant[i].pos.y==now.y) break;
		}
		if(i!=end) cout<<"   HP:"<<ant[i].xl<<"   level:"<<(ant[i].id+5)/6;//光标处有蚂蚁，输出蚂蚁信息
		else cout<<"   build:"<<s*100+need<<"   c_need:"<<c_need;//光标处什么都没有，输出造塔，炸弹价格
	}
	cout<<"   score:"<<fs<<"                                            "<<endl;//输出分数
}

inline bool judge(ll u,ll v)//判断（u，v）处是否可以造塔
{
	if(u==m && v==n || !u && !v) return 0;
	ll i;
	for(i=1; i<=s; i++)
	{
		if(abs(u-pt[i].x)<=1 && abs(v-pt[i].y)<=1) return 0;
	}
	return 1;
}

int main()
{
	now.x=now.y=0;//初始化光标位置
	ll i,j,tmp,mn,t;
	nxt[beg]=end,last[end]=beg;
	puts("请输入地图的长宽");
	cin>>m>>n;m--,n--;
	system("cls");
	out();
	for(;; T++)
	{
		clock_t stt=clock();
		while(!TT && clock()-stt<=300)//每0.3s移动一步
		{
			if(_kbhit())//如果有输入
			{
				C;
				if(ch=='w' && now.x) now.x--;
				else if(ch=='s' && now.x<m) now.x++;
				else if(ch=='a' && now.y) now.y--;
				else if(ch=='d' && now.y<n) now.y++;//光标移动
				else if(ch=='q'&&mon>=need+s*100&&judge(now.x,now.y))//造塔
				{
					mon-=cst[s+1]=need+s*100;
					pt[++s]=now;
					sh[s]=2;
					r[s]=2;
					use[pt[s].x][pt[s].y]=1;
				}
				else if(ch=='e')//炮塔升级
				{
					for(i=1; i<=s; i++)//判断光标处有无炮塔
					{
						if(now.x==pt[i].x&&now.y==pt[i].y) break;
					}
					if(i>s || mon<(dj[i]+1)*(dj[i]+1)*10) continue;
					puts("1:range 2:attack 3:cancel");
					for(C; ch!='1'&&ch!='2'&&ch!='3'; C);
					if(ch=='1') r[i]+=0.5;
					else if(ch=='2') sh[i]+=dj[i]+1;
					else continue;
					mon-=(dj[i]+1)*(dj[i]+1)*10;
					cst[i]+=(dj[i]+1)*(dj[i]+1)*10;
					dj[i]++;
					system("cls");
				}
				else if(ch=='c')//使用炸弹
				{
					if(mon<c_need) continue;
					mon-=c_need;
					for(i=nxt[beg]; i!=end; i=nxt[i])
					{
						ant[i].xl/=2;
					}
				}
				else if(ch=='k')//快进TT步
				{
					cin>>TT;
					system("cls");
					break;
				}
				else if(ch=='p')//暂停 or 继续
				{
					ZT^=1;
				}
				else if(ch=='z')//出售光标处的炮塔
				{
					for(i=1; i<=s; i++)
					{
						if(now.x==pt[i].x&&now.y==pt[i].y) break;
					}
					if(i>s) continue;
					use[now.x][now.y]=0;
					mon+=cst[i]*0.8;
					swap(pt[i],pt[s]);
					swap(sh[i],sh[s]);
					swap(dj[i],dj[s]);
					swap(r[i],r[s]);
					swap(cst[i],cst[s]);//将最后一个炮塔的信息存入当前炮塔
					s--;//删除最后一个炮塔
				}
				out();
			}
		}
		if(TT) TT--;//快进一步
		if(ZT)//暂停则不动
		{
			out();
			continue;
		}
		if(ca<6 && !use[0][0]) tt++,ant[tt].add(tt);//若蚂蚁不满且出生点处无蚂蚁，则新加入蚂蚁
		for(i=nxt[beg]; i!=end; i=nxt[i])
			xxs[ant[i].pos.x][ant[i].pos.y]+=(ant[i].have?5:2);//新增信息素
		for(i=nxt[beg]; i!=end; i=nxt[i])
			ant[i].move();//所有蚂蚁动一下
		for(i=1; i<=s; i++)//炮塔进行攻击
		{
			if(cake&&in(cake,i)) ata(i,cake);//优先打拿着蛋糕的蚂蚁
			else
			{
				mn=1000;
				for(j=nxt[beg]; j!=end; j=nxt[j])
				{
					tmp=lenf(ant[j].pos,pt[i]);
					if(tmp<mn)
					{
						mn=tmp;
						t=j;
					}
				}
				if(in(t,i))
					ata(i,t);//寻找攻击范围内血最少的蚂蚁并进行攻击
			}
		}
		for(i=nxt[beg]; i!=end; i=nxt[i]) if(ant[i].xl<=0) ant[i].GG();//判断蚂蚁是否死亡
		if(cake && !ant[cake].pos.x && !ant[cake].pos.y) break;//若蛋糕被搬回蚂蚁出生点，则游戏结束
		for(i=0; i<=m; i++) for(j=0; j<=n; j++) xxs[i][j]=max(xxs[i][j]-1,0ll);//信息素减少
		for(i=nxt[beg]; i!=end; i=nxt[i]) ant[i].old++;//蚂蚁长大1s
		mon++;//钱++
		out();
	}
	printf("Game over after %d seconds\n",T);
	Sleep(10000);
	system("pause");
	return 0;
}
