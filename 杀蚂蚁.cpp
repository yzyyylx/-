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
//xxs��¼�������µ���Ϣ��,dir�����ʾ���ϵ��ƶ��ļ�������
//nxt��last���������ʾ�����ָ�룬��¼����
//ca��ʾ����������cake��ʾ���е��������
//mon��ʾǮ��������need��ʾ��ʼ�����۸�c_need��ʾը���۸�fs��ʾ����
//sh,dj,cst,r��������ֱ��ʾ�������˺����ȼ����ܼۺ͹����뾶
bool use[M][M],ZT;
//use��¼�õ��Ƿ��ѱ�����ռ�ã�ZT��ʾ�Ƿ���ͣ
char ch;

class Node
{
public:
	ll x,y;
	Node operator - (const Node &u) const//�������
	{
		Node res;
		res.x=x-u.x;
		res.y=y-u.y;
		return res;
	}
	db operator *(const Node &u) {return x*u.x+y*u.y;}//������
} now;
inline db po(db u,ll v)//�˷�������
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
	ll xl,sx,old,id;//�ֱ��ʾ���ϵ�Ѫ������Ѫ��������ͱ��
	bool have;//��¼���������Ƿ��е���
	Node pos,bef;//��ǰλ�ú���һ����λ��
	bool ok(ll u,ll v)//�жϸõ��Ƿ�Ϸ�
	{
		if(u<0 || u>m || v<0 || v>n || use[u][v] || u==bef.x&&v==bef.y) return 0;
		return 1;
	}
	void add(ll u)//�³���һֻ���ϣ���ʼ���������ֵ��ͬʱ��������
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
	void GG()//���������������㽱����ͬʱ��������ɾ��
	{
		mon+=pow((id+5)/6,1.2);
		ca--;
		fs+=xl;
		if(have) cake=0;
		use[pos.x][pos.y]=0;
		nxt[last[id]]=nxt[id];
		last[nxt[id]]=last[id];
	}
	void move()//�����ƶ�һ��
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
Node pt[N];//��¼����λ��

inline db lenf(Node u,Node v){return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y);}//���������ƽ��
inline db len(Node u,Node v){return sqrt(lenf(u,v));}//��������
inline bool in(ll u,ll v){return lenf(ant[u].pos,pt[v])<r[v]*r[v]+eps;}//�ж�����u�Ƿ�������v�Ĺ�����Χ��
inline db cj(Node u,Node v){return u.x*v.y-u.y*v.x;}//����Ĳ��

inline db dis(Node u,Node v,Node w)//����w���߶�uv����̾���
{
	if((w-u)*(v-u)>eps && (w-v)*(u-v)>eps)
	{
		return fabs(cj(w-u,v-u))/len(u,v);
	}
	else return sqrt(min(lenf(u,w),lenf(v,w))+eps);
}

inline void ata(ll u,ll v)//��������u��������v����·�˺�
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

inline void out()//�����ͼ
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
	for(i=nxt[beg]; i!=end; i=nxt[i]) mm[ant[i].pos.x][ant[i].pos.y]='A';//�����ϼ����ͼ
	for(i=1; i<=s; i++) mm[pt[i].x][pt[i].y]='X';//�����������ͼ
	if(!cake) mm[m][n]='C';
	else mm[ant[cake].pos.x][ant[cake].pos.y]='C';//����������ͼ
	mm[now.x][now.y]='O';//���������ͼ
	for(i=0; i<=m; i++)
	{
		for(j=0; j<=n; j++)
		{
			printf("%c",mm[i][j]);
		}
		puts("");
	}
	cout<<"money:"<<mon;//���Ǯ������
	for(i=1; i<=s; i++)
	{
		if(now.x==pt[i].x&&now.y==pt[i].y) break;
	}
	if(i<=s) cout<<"   range:"<<r[i]<<"   "<<"attack:"<<sh[i]<<"   "<<"need:"<<(dj[i]+1)*(dj[i]+1)*10;//��괦�����������������Ϣ
	else
	{
		for(i=nxt[beg]; i!=end; i=nxt[i])
		{
			if(ant[i].pos.x==now.x&&ant[i].pos.y==now.y) break;
		}
		if(i!=end) cout<<"   HP:"<<ant[i].xl<<"   level:"<<(ant[i].id+5)/6;//��괦�����ϣ����������Ϣ
		else cout<<"   build:"<<s*100+need<<"   c_need:"<<c_need;//��괦ʲô��û�У����������ը���۸�
	}
	cout<<"   score:"<<fs<<"                                            "<<endl;//�������
}

inline bool judge(ll u,ll v)//�жϣ�u��v�����Ƿ��������
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
	now.x=now.y=0;//��ʼ�����λ��
	ll i,j,tmp,mn,t;
	nxt[beg]=end,last[end]=beg;
	puts("�������ͼ�ĳ���");
	cin>>m>>n;m--,n--;
	system("cls");
	out();
	for(;; T++)
	{
		clock_t stt=clock();
		while(!TT && clock()-stt<=300)//ÿ0.3s�ƶ�һ��
		{
			if(_kbhit())//���������
			{
				C;
				if(ch=='w' && now.x) now.x--;
				else if(ch=='s' && now.x<m) now.x++;
				else if(ch=='a' && now.y) now.y--;
				else if(ch=='d' && now.y<n) now.y++;//����ƶ�
				else if(ch=='q'&&mon>=need+s*100&&judge(now.x,now.y))//����
				{
					mon-=cst[s+1]=need+s*100;
					pt[++s]=now;
					sh[s]=2;
					r[s]=2;
					use[pt[s].x][pt[s].y]=1;
				}
				else if(ch=='e')//��������
				{
					for(i=1; i<=s; i++)//�жϹ�괦��������
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
				else if(ch=='c')//ʹ��ը��
				{
					if(mon<c_need) continue;
					mon-=c_need;
					for(i=nxt[beg]; i!=end; i=nxt[i])
					{
						ant[i].xl/=2;
					}
				}
				else if(ch=='k')//���TT��
				{
					cin>>TT;
					system("cls");
					break;
				}
				else if(ch=='p')//��ͣ or ����
				{
					ZT^=1;
				}
				else if(ch=='z')//���۹�괦������
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
					swap(cst[i],cst[s]);//�����һ����������Ϣ���뵱ǰ����
					s--;//ɾ�����һ������
				}
				out();
			}
		}
		if(TT) TT--;//���һ��
		if(ZT)//��ͣ�򲻶�
		{
			out();
			continue;
		}
		if(ca<6 && !use[0][0]) tt++,ant[tt].add(tt);//�����ϲ����ҳ����㴦�����ϣ����¼�������
		for(i=nxt[beg]; i!=end; i=nxt[i])
			xxs[ant[i].pos.x][ant[i].pos.y]+=(ant[i].have?5:2);//������Ϣ��
		for(i=nxt[beg]; i!=end; i=nxt[i])
			ant[i].move();//�������϶�һ��
		for(i=1; i<=s; i++)//�������й���
		{
			if(cake&&in(cake,i)) ata(i,cake);//���ȴ����ŵ��������
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
					ata(i,t);//Ѱ�ҹ�����Χ��Ѫ���ٵ����ϲ����й���
			}
		}
		for(i=nxt[beg]; i!=end; i=nxt[i]) if(ant[i].xl<=0) ant[i].GG();//�ж������Ƿ�����
		if(cake && !ant[cake].pos.x && !ant[cake].pos.y) break;//�����ⱻ������ϳ����㣬����Ϸ����
		for(i=0; i<=m; i++) for(j=0; j<=n; j++) xxs[i][j]=max(xxs[i][j]-1,0ll);//��Ϣ�ؼ���
		for(i=nxt[beg]; i!=end; i=nxt[i]) ant[i].old++;//���ϳ���1s
		mon++;//Ǯ++
		out();
	}
	printf("Game over after %d seconds\n",T);
	Sleep(10000);
	system("pause");
	return 0;
}
