
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

#define N 5
#define M 20010
int n = 3, now = 1, st = 1, difficulty = 1;
int cnt_win, cnt_lose, cnt_draw;

struct P{int fi, se;};
inline P mp(int u, int v)
{
	P res;
	res.fi = u;
	res.se = v;
	return res;
}

inline CString turn(int u)
{
	CString res=0;
	for (; u; u /= 10) res = (char)(u % 10 + '0') + res;
	return res;
}

struct Qp
{
	int ee,mm[N][N];
	P emp[10];
	/*
	* 0-----empty
	* 1-----O
	* 2-----X
	*/
	inline void init() { memset(mm, 0, sizeof(mm)); }
	/*
	* 0----playing
	* 1----O
	* 2----X
	* 3----Draw
	*/
	inline int win()
	{
		int i, j;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++) if (mm[i][j] != 1) break; if (j > n) return 1;
			for (j = 1; j <= n; j++) if (mm[i][j] != 2) break; if (j > n) return 2;
			for (j = 1; j <= n; j++) if (mm[j][i] != 1) break; if (j > n) return 1;
			for (j = 1; j <= n; j++) if (mm[j][i] != 2) break; if (j > n) return 2;
		}
		for (i = 1; i <= n; i++) if (mm[i][i] != 1) break; if (i > n) return 1;
		for (i = 1; i <= n; i++) if (mm[i][i] != 2) break; if (i > n) return 2;
		for (i = 1; i <= n; i++) if (mm[i][n - i + 1] != 1) break; if (i > n) return 1;
		for (i = 1; i <= n; i++) if (mm[i][n - i + 1] != 2) break; if (i > n) return 2;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				if (!mm[i][j])
					return 0;
			}
		}
		return 3;
	}
	inline int play(int u, int v, int w)
	{
		if (mm[u][v]) return -1;
		mm[u][v] = w;
		return win();
	}
	inline int get()
	{
		int i, j, t = 1, res = 0;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				res += mm[i][j] * t;
				t *= 3;
			}
		}
		return res;
	}
	inline void get(int u)
	{
		int i, j, t = 1;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				mm[i][j] = u/t%3;
				t *= 3;
			}
		}
	}
	inline void get_empt()
	{
		int i, j;
		ee = 0;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				if(!mm[i][j])
					emp[++ee] = mp(i, j);
			}
		}
	}
	inline P rd()
	{
		get_empt();
		return emp[rand() % ee + 1];
	}
}board;

namespace Cpu
{
	int dp[M][2],pos[M][2];
	P to[M][2][10];
	inline void init() { memset(dp, -1, sizeof(dp)); }
	/*
	* v==0 ----- O's turn
	* v==1 ----- X's turn
	* dp[u][v]==3 ---- Draw
	* dp[u][v]==2 ---- Lose
	* dp[u][v]==1 ---- Win
	* dp[u][v]==-1 ---- Unknown
	*/
	int ask(int u, int v)
	{
		if (dp[u][v] != -1) return dp[u][v];
		Qp now;
		now.get(u);
		int w = now.win();
		if (w == 3) return dp[u][v] = 3;
		if (w == 1) return !v ? dp[u][v] = 1 : dp[u][v] = 2;
		if (w == 2) return !v ? dp[u][v] = 2 : dp[u][v] = 1;
		int i, j, t, r, res = 0;
		dp[u][v] = 2;
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				if (now.mm[i][j]) continue;
				now.mm[i][j] = v+1;
				t = now.get(), r = ask(t, v^1);
				now.mm[i][j] = 0;
				if (r == 2)
				{
					if (res != 2) pos[u][v]=0, res = 2;
					to[u][v][++pos[u][v]]=mp(i, j);
					dp[u][v] = 1;
				}
				else if (r == 3)
				{
					if (res == 2) continue;
					if (res != 3) pos[u][v]=0, res = 3;
					to[u][v][++pos[u][v]] = mp(i, j);
					dp[u][v] = 3;
				}
			}
		}
		if (res >= 2) return dp[u][v];
		now.get(u);
		for (i = 1; i <= n; i++) for (j = 1; j <= n; j++) if (!now.mm[i][j]) to[u][v][++pos[u][v]] = mp(i, j);
		return 2;
	}
	inline P play(int v)
	{
		if (rand() % 4 < difficulty)
		{
			int u = board.get();
			ask(u, v);
			return to[u][v][rand() % pos[u][v] + 1];
		}
		else return board.rd();
	}
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, win_txt);
	DDX_Control(pDX, IDC_EDIT2, lose_txt);
	DDX_Control(pDX, IDC_EDIT3, draw_txt);
	DDX_Control(pDX, IDC_EDIT5, st_txt);
	DDX_Control(pDX, IDC_EDIT4, difficulty_txt);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_STN_CLICKED(IDC_STATIC1, &CMFCApplication1Dlg::OnStnClickedStatic1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCApplication1Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCApplication1Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCApplication1Dlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMFCApplication1Dlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CMFCApplication1Dlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CMFCApplication1Dlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CMFCApplication1Dlg::OnBnClickedButton15)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	srand(time(0));
	board.init();
	Cpu::init();
	CString str;str = "0";
	win_txt.SetWindowText(str);
	lose_txt.SetWindowText(str);
	draw_txt.SetWindowText(str);
	str = "简单"; difficulty_txt.SetWindowText(str);
	str = "先手"; st_txt.SetWindowText(str);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplication1Dlg::OnStnClickedStatic1()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 2, v = 1;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if(now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC4);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now+1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 3, v = 1;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC7);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 1, v = 1;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC1);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 1, v = 2;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC2);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 1, v = 3;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC3);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 2, v = 2;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC5);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 2, v = 3;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC6);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 3, v = 2;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC8);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	int u, v, res;
	u = 3, v = 3;
	if (board.mm[u][v]) return;

	CBitmap bitmap;
	if (now) bitmap.LoadBitmap(IDB_BITMAP2);
	else bitmap.LoadBitmap(IDB_BITMAP3);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC9);
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);

	board.mm[u][v] = now + 1;
	res = board.win();
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)//cpu's first step
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}

	//Cpu's turn
	now ^= 1;
	P cpu = Cpu::play(now);
	u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	res = board.win();
	now ^= 1;
	if (res)//game over?
	{
		if (res == 3)
		{
			cnt_draw++;
			draw_txt.SetWindowText(turn(cnt_draw));
			AfxMessageBox(TEXT("Draw！"));
		}
		else if (res == now + 1)
		{
			cnt_win++;
			win_txt.SetWindowText(turn(cnt_win));
			AfxMessageBox(TEXT("You Win！"));
		}
		else
		{
			cnt_lose++;
			lose_txt.SetWindowText(turn(cnt_lose));
			AfxMessageBox(TEXT("You Lose！"));
		}
		//clear
		now = 1;
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
		board.init();
		if (!st)
		{
			P cpu = Cpu::play(now);
			u = cpu.fi, v = cpu.se;
			board.mm[u][v] = now + 1;
			CBitmap bmp;
			if (now) bmp.LoadBitmap(IDB_BITMAP2);
			else bmp.LoadBitmap(IDB_BITMAP3);
			if (u == 1)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
			}
			else if (u == 2)
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
			}
			else
			{
				if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
				else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
				else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
			}
			p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
			p->SetBitmap(bmp);
			now ^= 1;
		}
		return;
	}
}

void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 1;
	CString str; str = "简单";
	str = "简单"; difficulty_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();
	if (!st)
	{
		P cpu = Cpu::play(now);
		int u = cpu.fi, v = cpu.se;
		board.mm[u][v] = now + 1;
		CBitmap bmp;
		if (now) bmp.LoadBitmap(IDB_BITMAP2);
		else bmp.LoadBitmap(IDB_BITMAP3);
		if (u == 1)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
		}
		else if (u == 2)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
		}
		else
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
		}
		p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		p->SetBitmap(bmp);
		now ^= 1;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 2;
	CString str; 
	str = "普通";
	difficulty_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();
	if (!st)
	{
		P cpu = Cpu::play(now);
		int u = cpu.fi, v = cpu.se;
		board.mm[u][v] = now + 1;
		CBitmap bmp;
		if (now) bmp.LoadBitmap(IDB_BITMAP2);
		else bmp.LoadBitmap(IDB_BITMAP3);
		if (u == 1)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
		}
		else if (u == 2)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
		}
		else
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
		}
		p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		p->SetBitmap(bmp);
		now ^= 1;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 3;
	CString str;
	str = "困难";
	difficulty_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();
	if (!st)
	{
		P cpu = Cpu::play(now);
		int u = cpu.fi, v = cpu.se;
		board.mm[u][v] = now + 1;
		CBitmap bmp;
		if (now) bmp.LoadBitmap(IDB_BITMAP2);
		else bmp.LoadBitmap(IDB_BITMAP3);
		if (u == 1)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
		}
		else if (u == 2)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
		}
		else
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
		}
		p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		p->SetBitmap(bmp);
		now ^= 1;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 4;
	CString str;
	str = "地狱";
	difficulty_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();
	if (!st)
	{
		P cpu = Cpu::play(now);
		int u = cpu.fi, v = cpu.se;
		board.mm[u][v] = now + 1;
		CBitmap bmp;
		if (now) bmp.LoadBitmap(IDB_BITMAP2);
		else bmp.LoadBitmap(IDB_BITMAP3);
		if (u == 1)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
		}
		else if (u == 2)
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
		}
		else
		{
			if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
			else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
			else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
		}
		p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		p->SetBitmap(bmp);
		now ^= 1;
	}
}


void CMFCApplication1Dlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	st = 1;
	CString str;
	str = "先手";
	st_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();
}


void CMFCApplication1Dlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	st = 0;
	CString str;
	str = "后手";
	st_txt.SetWindowText(str);
	//clear
	now = 1;
	CBitmap bitmap;
	CStatic* p;
	bitmap.LoadBitmap(IDB_BITMAP1);
	p = (CStatic*)GetDlgItem(IDC_STATIC1); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC2); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC3); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC4); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC5); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC6); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC7); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC8); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	p = (CStatic*)GetDlgItem(IDC_STATIC9); p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); p->SetBitmap(bitmap);
	board.init();

	P cpu = Cpu::play(now);
	int u = cpu.fi, v = cpu.se;
	board.mm[u][v] = now + 1;
	CBitmap bmp;
	if (now) bmp.LoadBitmap(IDB_BITMAP2);
	else bmp.LoadBitmap(IDB_BITMAP3);
	if (u == 1)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC1);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC2);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC3);
	}
	else if (u == 2)
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC4);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC5);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC6);
	}
	else
	{
		if (v == 1) p = (CStatic*)GetDlgItem(IDC_STATIC7);
		else if (v == 2) p = (CStatic*)GetDlgItem(IDC_STATIC8);
		else if (v == 3) p = (CStatic*)GetDlgItem(IDC_STATIC9);
	}
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bmp);
	now ^= 1;
}
