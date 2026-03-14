#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<mmsystem.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#pragma comment(lib,"winmm.lib")
IMAGE bk;
IMAGE img_player[8];
IMAGE img_zidan[4];
IMAGE img_dj[2][8];
int scord = 0;
//窗口大小设置
enum ck
{
	WIDTH = 640,
	HEIGHT = 860,
	zd1=15,
	zd2=20,
	dj_=10,
	a1=101,
	b1=102,
};
//定时器
bool TimeSleep(DWORD time, int id)
{
	static DWORD t[10];
	if ((clock()-t[id])>=time)
	{
		t[id] = clock();
		return true;
	}
	return false;
}
//属性定义
struct plance
{
	int x;
	int y;
	bool live;
	int hp;
	int w;
	int h;
	int type;
	int sw = 0;
	int s = 2;
}plyer, bull[zd1], dj[dj_],dbull[zd2];
//数据显示
void showme()
{
	for (int i = 0; i < dj_; i++)
	{
		printf("%d %d %d %d %d %d************%d\n", dj[i].x, dj[i].y, dj[i].live, dj[i].sw, dj[i].type, dj[i].s, i);
	}
}
//图片加载
void loadImage()

{
	loadimage(&bk, "./pecture./bei jing.png");
	loadimage(&img_player[0], "./pecture./p0.0.jpg");
	loadimage(&img_player[1], "./pecture./p0.jpg");
	loadimage(&img_player[2], "./pecture./p1.0.jpg");
	loadimage(&img_player[3], "./pecture./p1.jpg");
	loadimage(&img_player[4], "./pecture./p2.0.jpg");
	loadimage(&img_player[5], "./pecture./p2.jpg");
	loadimage(&img_player[6], "./pecture./p3.0.jpg");
	//子弹
	loadimage(&img_zidan[0], "./pecture./zd1.jpg");
	loadimage(&img_zidan[1], "./pecture./zd2.jpg");
	loadimage(&img_zidan[2], "./pecture./zd3.jpg");
	loadimage(&img_zidan[3], "./pecture./zd4.jpg");
	//敌机
	loadimage(&img_dj[0][0], "./pecture./d1.0.jpg");
	loadimage(&img_dj[0][1], "./pecture./d1.1.jpg");
	loadimage(&img_dj[0][2], "./pecture./d1.2.jpg");
	loadimage(&img_dj[0][3], "./pecture./d1.3.jpg");
	loadimage(&img_dj[0][4], "./pecture./d1.4.jpg");
	loadimage(&img_dj[0][5], "./pecture./d1.5.jpg");
	loadimage(&img_dj[0][6], "./pecture./d1.6.jpg");
	loadimage(&img_dj[0][7], "./pecture./d1.7.jpg");
	loadimage(&img_dj[1][0], "./pecture./d2.0.jpg");
	loadimage(&img_dj[1][1], "./pecture./d2.1.jpg");
	loadimage(&img_dj[1][2], "./pecture./d2.2.jpg");
	loadimage(&img_dj[1][3], "./pecture./d2.3.jpg");
	loadimage(&img_dj[1][4], "./pecture./d2.4.jpg");
	loadimage(&img_dj[1][5], "./pecture./d2.5.jpg");
	loadimage(&img_dj[1][6], "./pecture./d2.6.jpg");
	loadimage(&img_dj[1][7], "./pecture./d2.7.jpg");
}
//飞机属性设定
void djHp(int i)
{
	int sl = rand() % 10;
	if (sl<3)
	{
		dj[i].live = true;
		dj[i].x = rand() % (WIDTH - 100);
		dj[i].y = -90;
		dj[i].type = a1;
		dj[i].hp = 3;
		dj[i].w = 68;
		dj[i].h = 87;
		dj[i].sw = 0; 
		dj[i].s = 2;
	}
	else {
		dj[i].live = true;
		dj[i].x = rand() % (WIDTH - 100);
		dj[i].y = -90;
		dj[i].type = b1;
		dj[i].hp = 1;
		dj[i].w = 64;
		dj[i].h = 64;
		dj[i].sw = 0;
		dj[i].s = 2;
	}
}
//初始化数据
void gameInit()
{
	loadImage();
	plyer.x = WIDTH / 2 - 64;
	plyer.y = HEIGHT - 128;
	plyer.h = 128;
	plyer.w = 128;
	plyer.hp = 10;
	plyer.live = true;
	for (int i = 0; i < zd1; i++)
	{
		bull[i].live = false;
	}
	for (int i = 0; i < zd2; i++)
	{
		dbull[i].live = false;
	}
	for (int i = 0; i < dj_; i++)
	{
		dj[i].sw = 0;
		dj[i].live = false;
	}
}
//音乐播放
void music()
{
	mciSendString("open ./music./背景音乐.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
	if (!plyer.live)
	{
		mciSendString("close BGM", 0, 0, 0);
	}
}
//产生敌机
void creatdj(DWORD time)
{
	if (TimeSleep(time, 0))
	{
		for (int i = 0; i < dj_; i++)
		{
			if (!dj[i].live)
			{
				djHp(i);
				break;
			}
		}
}	
}
//敌机爆炸动画
void djboom(DWORD time)
{
	
	for (int i = 0; i < dj_; i++)
	{
		if (dj[i].s == 8)
		{
			continue;
		}
		if (dj[i].sw && dj[i].s == 2 && !dj[i].live&&TimeSleep(100,6))
		{
			mciSendString("close 爆炸", 0, 0, 0);
			mciSendString("open ./music./爆炸.mp3 alias 爆炸", 0, 0, 0);
			mciSendString("play 爆炸 ", 0, 0, 0);
		}
		if (dj[i].sw&&dj[i].s >= 2 && !dj[i].live)
		{
			
			if (dj[i].type == a1)
			{
				putimage(dj[i].x, dj[i].y, &img_dj[0][dj[i].s + 1], NOTSRCERASE);
				putimage(dj[i].x, dj[i].y, &img_dj[0][dj[i].s], SRCINVERT);
				if (dj[i].s < 7 && TimeSleep(time, 3))
				{
					dj[i].s += 2;
				}
			}
			else if (dj[i].type == b1)
			{
				putimage(dj[i].x, dj[i].y, &img_dj[1][dj[i].s + 1], NOTSRCERASE);
				putimage(dj[i].x, dj[i].y, &img_dj[1][dj[i].s], SRCINVERT);
				if (dj[i].s < 7 && TimeSleep(time, 3))
				{
					dj[i].s+= 2;
				}
			}
		
		}
	}
	
}
//飞机爆炸动画
void boom(DWORD time)
{
		if (plyer.s >= 2)
		{
			putimage(plyer.x, plyer.y, &img_player[plyer.s], NOTSRCERASE);
			putimage(plyer.x, plyer.y, &img_player[plyer.s+1], SRCINVERT);
				if (plyer.s < 5 && TimeSleep(time, 7))
				{
					mciSendString("close 爆炸1", 0, 0, 0);
					mciSendString("open ./music./爆炸1.mp3 alias 爆炸1", 0, 0, 0);
					mciSendString("play 爆炸1 ", 0, 0, 0);
					plyer.s += 2;
				}

		}
}
//绘图
void gameDraw()
{
	int a = 0;
	putimage(0, 0, &bk);
	if (plyer.live)
	{
		putimage(plyer.x, plyer.y, &img_player[0], NOTSRCERASE);
		putimage(plyer.x, plyer.y, &img_player[1], SRCINVERT);
	}
	else {
		boom(400);
	}
	for (int i = 0; i < zd1; i++)
	{
		
		if (bull[i].live)
		{
			putimage(bull[i].x, bull[i].y, &img_zidan[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_zidan[1], SRCINVERT);
		}
	}
	for (int i = 0; i < zd2; i++)
	{
		if (dbull[i].live)
		{
			putimage(dbull[i].x,dbull[i].y, &img_zidan[2], NOTSRCERASE);
			putimage(dbull[i].x,dbull[i].y, &img_zidan[3], SRCINVERT);
		}
	}
	for (int i = 0; i < dj_; i++)
	{
		if (dj[i].live)
		{
			if (dj[i].type == a1)
			{
				putimage(dj[i].x, dj[i].y, &img_dj[0][1], NOTSRCERASE);
				putimage(dj[i].x, dj[i].y, &img_dj[0][0], SRCINVERT);
			}
			else if (dj[i].type == b1)
			{
				putimage(dj[i].x, dj[i].y, &img_dj[1][1], NOTSRCERASE);
				putimage(dj[i].x, dj[i].y, &img_dj[1][0], SRCINVERT);
			}
		}
			
	}
	djboom(120);
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	settextstyle(20, 0, "楷体");
	char num[20];
	sprintf(num, "%d", scord);
	outtextxy(40, 0, num);
	outtextxy(0, 0,"分数:");
	setbkmode(TRANSPARENT);
	settextcolor(RGB(100,0,100));
	settextstyle(20, 0, "楷体");
	char num2[20];
	sprintf(num2, "%d",plyer.hp);
	outtextxy(40, 20, num2);
	outtextxy(0, 20, "血量:");
	if (plyer.s == 6)
	{
		setbkmode(TRANSPARENT);
		settextcolor(RGB(100,100,100));
		settextstyle(100, 0, "楷体");
	    outtextxy(100,400, "GAME OVER");
	}
}
//创建子弹
void creatzd(int n)
{
	if(n==zd1)
{
	for (int i = 0; i < n; i++)
	{
		if (!bull[i].live)
		{
			//mciSendString("close 子弹发射", 0, 0, 0);
			//mciSendString("open ./music./子弹发射.mp3 alias 子弹发射", 0, 0, 0);
			//mciSendString("play 子弹发射 ", 0, 0, 0);
			bull[i].x = plyer.x + 61;
			bull[i].y = plyer.y - 14;
			bull[i].live = true;
			break;
		}
	}
}
	
	if (n == zd2)
	{
		for (int i = 0; i < n; i++)
		{
			if (!dbull[i].live&&dj[i].live)
			{
				
				dbull[i].x = dj[i].x + dj[i].w/2;
				dbull[i].y = dj[i].y + dj[i].h/2;
				dbull[i].live = true;
				break;
			}
		}
	}
}
//子弹移动
void zdMove(int zd,int s)
{
	if (zd == zd1)
	{
		for (int i = 0; i < zd; i++)
		{
			if (bull[i].live)
			{
				bull[i].y -= s;
				if (bull[i].y < -21)
				{
					bull[i].live = false;
				}
			}
		}
	}
	if (zd == zd2)
	{
		for (int i = 0; i < zd; i++)
		{
			if (dbull[i].live)
			{
				dbull[i].y += s;
				if (dbull[i].y > HEIGHT+21&& dbull[i].y>0)
				{
					dbull[i].live = false;
				}
			}
		}
	}
}
//碰撞判定
void bzsw(int r)
{
	for (int i = 0; i < dj_; i++)
	{
		
		if (!plyer.live)
			break; 
		if (!dj[i].live)
			continue;
		if((int)(sqrt(pow((plyer.x + plyer.w / 2) - (dj[i].x + dj[i].w / 2), 2) + pow((plyer.y + plyer.h / 2) - (dj[i].y+ dj[i].h / 2), 2)))<=r)
		{   plyer.live = false;
		    plyer.hp = 0;
			dj[i].sw = 1;
			dj[i].live = false;
		}
	}

}
  //飞机控制
void playarMove(int speed)
{
	/*if (_kbhit())
	{
		char key = _getch();
		
		switch (key)
		{
		case'w':
		case'W':
			plyer.y -= speed;
			break;
		case's':
		case'S':
			plyer.y += speed;
			break;
		case'a':
		case'A':
			plyer.x -= speed;
			break;
		case'd':
		case'D':
			plyer.x += speed;
			break;
		}
}*/
	{
		if (GetAsyncKeyState('W') && plyer.y >= 0)
		{
			plyer.y -= speed;
		}
		if (GetAsyncKeyState('S') && plyer.y <= 860 - 100)
		{
			plyer.y += speed;

		}
		if (GetAsyncKeyState('A') && plyer.x >= -60)
		{
			plyer.x -= speed;

		}
		if (GetAsyncKeyState('D') && plyer.x <= 640 - 70)
		{
			plyer.x += speed;
		}
		if (GetAsyncKeyState(' '))
		{
			if (plyer.live && TimeSleep(100, 1))
			{
				creatzd(zd1);

			}

		}
	}
	
}
//敌机移动
void djMove(int speed)
{
	for (int i=0;i< dj_; i++)
	{
		if (dj[i].live)
		{
			dj[i].y+=speed;
			if (dj[i].y > HEIGHT)
			{
				dj[i].live = false;
			}
		}
	}
}
//敌机击落判定
void djsw()
{
	for (int i = 0; i < dj_; i++)
	{
		if (!plyer.live)
			break;
		if (!dj[i].live)
			continue;
		for (int j = 0; j < dj_; j++)
		{
			if (!bull[j].live)
				continue;
			if (bull[j].x > dj[i].x && bull[j].x <dj[i].x + dj[i].w && bull[j].y>dj[i].y && bull[j].y < dj[i].y + dj[i].h)
			{
				bull[j].live = false;
				dj[i].hp--;
			}
		}
		if (dj[i].hp <= 0)
		{
			dj[i].sw = 1;
			dj[i].live = false;
			if (dj[i].type == a1)
			{
				scord += 4;
			}else if(dj[i].type == b1)
			{
				scord += 1;
			}
		}
	}
}
//玩家击落判定
void plyersw()
{
	for (int j = 0; j < dj_; j++)
	{
		if (!dbull[j].live)
			continue;
		if (dbull[j].x > plyer.x &&dbull[j].x <plyer.x + plyer.w &&dbull[j].y>plyer.y && dbull[j].y < plyer.y + plyer.h)
		{
			dbull[j].live = false;
			plyer.hp--;
		}
	}
	if (plyer.hp <= 0)
	{
		plyer.live = false;
		plyer.hp = 0;
	}
}
//游戏开始
bool game_begin()
{
	HWND hnd = GetHWnd();
	int end = MessageBox(hnd, "GAME BEGIN", "是否开始游戏",MB_OKCANCEL);
	if (end == IDOK)
	{
		return 1;
	}
	else if (end == IDCANCEL)
	{
		return 0;
	}
}
//游戏结束
bool game_over()
{
	HWND hnd=GetHWnd();
	int end=MessageBox(hnd,"GAME OVER","是否结束游戏",MB_RETRYCANCEL);
	if (end == IDRETRY)
	{
		return 1;
	}
	else if(end==IDCANCEL)
	{
		return 0;
	}
}
int main() {
	HWND ck = initgraph(WIDTH, HEIGHT);
	MoveWindow(ck,500,25,WIDTH,HEIGHT,true);
	gameInit();
	BeginBatchDraw();
	music();
	bool end = false; ;
	gameDraw();
	FlushBatchDraw();
	if(game_begin())
	{
		end = 1;
		
	}
	else
	{
		end = false;
	}
	while(end==1)
	{
		
		if (TimeSleep(10, 5))
		{
			creatdj(500);
			plyersw();
			gameDraw();
			FlushBatchDraw();
			playarMove(5);
			zdMove(zd1, 10);
			djMove(2);
			if (TimeSleep(50, 4))
			{
				creatzd(zd2);
			}
			zdMove(zd2, 5);
			djsw();
			bzsw(50);
			
		}
		if (!plyer.live&&TimeSleep(15000,10))
		{
			if (game_over())
			{
				plyer.live = 1;
				plyer.hp = 10;
				plyer.s = 2;
				scord = 0;
			}
			else 
			{
				end = false;
			}
		}
	}
	EndBatchDraw();
	closegraph();
	return 0;
}