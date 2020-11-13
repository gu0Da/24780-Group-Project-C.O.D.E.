// 24780 HW 5
// Wuzhou Zu (wuzhouz@andrew.cmu.edu)
// 10.05.2020

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "fssimplewindow.h"

const int nTargets = 20;
const int nMissiles = 2;

// Shuffle function
void SwapInt(int& a, int& b)
{
	int c;
	c = b;
	b = a;
	a = c;
}

void DrawCraft(int x, int y)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_QUADS);

	glVertex2i(x - 5, y);
	glVertex2i(x - 5, y - 19);
	glVertex2i(x + 4, y - 19);
	glVertex2i(x + 4, y);

	glVertex2i(x - 9, y + 4);
	glVertex2i(x - 9, y - 5);
	glVertex2i(x + 9, y - 5);
	glVertex2i(x + 9, y + 4);

	glEnd();
}

void DrawMissile(int x, int y)
{
	glBegin(GL_TRIANGLES);
	glVertex2i(x - 3, y + 6);
	glVertex2i(x + 3, y + 6);
	glVertex2i(x, y - 6);
	glEnd();
}

//Color gradation on enemies
void DrawEnemy(int x, int y, int w, int h)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_QUADS);
	glColor3ub(255, 0, 0);
	glVertex2i(x, y);
	glColor3ub(0, 255, 0);
	glVertex2i(x + w, y);
	glColor3ub(0, 0, 255);
	glVertex2i(x + w, y + h);
	glColor3ub(255, 255, 0);
	glVertex2i(x, y + h);
	glEnd();
}

bool CheckTargetCollision(int mx, int my, int tx, int ty, int tw, int th)
{
	int dx = mx - tx;
	int dy = my - ty;
	if (0 <= dx && dx < tw && 0 <= dy && dy < th)
	{
		return true;
	}
	return false;
}

class Target
{
public:
	int x, y, state, w, h, v;
	void Initialize(void);
};

// Create falling targets with random size and speed
void Target::Initialize(void)
{
	y = 0;
	x = 100 + rand() % 600;
	w = 60 + rand() % 50;
	h = 50 + rand() % 20;
	v = 1 + rand() % 4;
	state = 1;
}

class Missile
{
public:
	int x, y, state;
	void Initialize(void);
};

void Missile::Initialize(void)
{
	state = 0;
}

int main(void)
{	
	// Create Math Quiz
	srand(time(nullptr));
	int q1[20];
	for (int i = 0; i < 20; i++)
	{
		q1[i] = i;
	}
	for (int i = 0; i < 20; i++)
	{
		int j = rand() % 20;
		SwapInt(q1[i], q1[j]);
	}
	int qState = 0;
	// Upgrade weapon
	for (int i = 0; i < 3; i++) {
		int k = rand() % 20;
		int g = rand() % 20;
		int ans;
		int a = q1[k];
		int b = q1[g];
		int cans = a * b;
		std::cout << "What is the product of " << a << " and " << b << "?\n";
		std::cin >> ans;

		if (ans == cans)
		{
			qState++;
			std::cout << "Congrats! Your missile flight speed is upgraded to level " << qState << " !\n";
		}
		else
		{
			qState += 0;
			std::cout << "Wrong answer! Your missile did not get the flight speed upgrade.\n";
			std::cout << "Your current flight speed level is " << qState << " .\n";
		}
	}
	int score = 0;
	int missileUsed = 0;
	int x = 400, y = 575;

	Missile missile[nMissiles];
	Target target[nTargets];

	for (auto& t : target)
	{
		t.Initialize();
	}
	for (auto& m : missile)
	{
		m.Initialize();
	}

	FsOpenWindow(0, 0, 800, 600, 1);
	for (;;)
	{
		FsPollDevice();
		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}
		switch (key)
		{
		case FSKEY_SPACE:
			for (auto& m : missile)
			{
				if (0 == m.state)
				{
					m.state = 1;
					m.x = x;
					m.y = y;
					++missileUsed;
					break;
				}
			}
			break;
		}
		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			x -= 10;
			if (x <= 0)
			{
				x = 795;
			}
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			x += 10;
			if (x >= 800)
			{
				x = 0;
			}
		}
		// assign missiles different flight speeds by number of questions answered correctly
		switch (qState)
		{
		case 0:
			for (auto& m : missile)
			{
				if (0 != m.state)
				{
					m.y -= 3;
					if (m.y < 0)
					{
						m.state = 0;
					}
				}
			}
			break;
		case 1:
			for (auto& m : missile)
			{
				if (0 != m.state)
				{
					m.y -= 5;
					if (m.y < 0)
					{
						m.state = 0;
					}
				}
			}
			break;
		case 2:
			for (auto& m : missile)
			{
				if (0 != m.state)
				{
					m.y -= 7;
					if (m.y < 0)
					{
						m.state = 0;
					}
				}
			}
			break;
		case 3:
			for (auto& m : missile)
			{
				if (0 != m.state)
				{
					m.y -= 10;
					if (m.y < 0)
					{
						m.state = 0;
					}
				}
			}
			break;
		default:
			for (auto& m : missile)
			{
				if (0 != m.state)
				{
					m.y -= 5;
					if (m.y < 0)
					{
						m.state = 0;
					}
				}
			}
			break;
		}
		for (auto& t : target)
		{
			if (0 != t.state)
			{
				t.y += t.v;
				if (600 <= t.y)
				{					
					t.y = 0;
				}
			}
		}
		for (auto& m : missile)
		{
			if (0 != m.state)
			{
				bool shotDown = false;
				int nTargetsAlive = 0;
				for (auto& t : target)
				{
					if (0 != t.state && 0 != m.state && true == CheckTargetCollision(m.x, m.y, t.x, t.y, t.w, t.h))
					{
						m.state = 0;
						t.state = 0;
						shotDown = true;
						score += 100;
					}
					nTargetsAlive += t.state;
				}
				if (true == shotDown && 0 == nTargetsAlive)
				{
					std::cout << "You Win!";
					std::cout << "You used " << missileUsed << " to shoot down all " << nTargets << " targets.\n";
					std::cout << "You score is " << score << ".\n";
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glShadeModel(GL_SMOOTH);
		DrawCraft(x, y);
		for (auto& m : missile)
		{
			if (0 != m.state)
			{
				DrawMissile(m.x, m.y);
			}
		}
		for (auto& t : target)
		{
			if (0 != t.state)
			{
				DrawEnemy(t.x, t.y, t.w, t.h);
			}
		}

		glFlush();
		FsSwapBuffers();
		FsSleep(10);
	}
	return 0;
}
