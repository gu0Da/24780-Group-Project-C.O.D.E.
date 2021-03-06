/*
Move: arrows
Space: Shoot
change to standard missile: S
change to missile B: B
Simulate hit: H
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "fssimplewindow.h" 
#include <string>
#include <vector>
#include <time.h>

const int nMissiles=10;

using namespace std;

void DrawAllMissiles();
void DrawRect(int blockX,int blockY, int blockSizeX, int blockSizeY); //used for demo health bar
void DrawCircle(int cx,int cy,int rad,int fill); //used for demo force field

const int nParticlePerExplosion=100;
class Explosion
{
public:
	int state =0;
	int counter;
	int xy[nParticlePerExplosion*2];
	int vxvy[nParticlePerExplosion*2];
	void Initialize(void);
	bool Begin(int x,int y);
	void Move(void);
	void Draw(void);
	void Disappear(void);
};
void Explosion::Initialize(void)
{
	state=0;
}
bool Explosion::Begin(int x,int y)
{
	if(0==state)
	{
		state=1;
		counter=0;
		for(int i=0; i<nParticlePerExplosion; ++i)
		{
			xy  [i*2  ]=x;
			xy  [i*2+1]=y;
			vxvy[i*2  ]=rand()%11-5;
			vxvy[i*2+1]=rand()%11-5;
		}
		return true;
	}
	return false;
}
void Explosion::Move(void)
{
	for(int i=0; i<nParticlePerExplosion; ++i)
	{
		xy[i*2  ]+=vxvy[i*2  ];
		xy[i*2+1]+=vxvy[i*2+1];
	}

	++counter;
	if(50<counter)
	{
		Disappear();
	}
}
void Explosion::Draw(void)
{
	glPointSize(5);
	glColor3ub(255,0,0);
	glBegin(GL_POINTS);
	for(int i=0; i<nParticlePerExplosion; ++i)
	{
		glVertex2i(xy[i*2  ],xy[i*2+1]);
	}
	glEnd();
}
void Explosion::Disappear(void)
{
	state=0;
}

class Player{
public:
  int x;
  int y;
  int health=0;
  int state=0; //Whether ship is able to take damage: ie 0 if either dead or in the midst of respawning, 1 if in regular play
  bool respawn=true; //expression used to check if ships coordinates should be reset to respawn location
  bool spawning=true; //expersssion used if ship is in midst of respawning
  Explosion e;
  string missileType="Standard";
  int initializeCount=0;
  void Shoot();
  void ChangeMissile(string newType);
  void Draw();
  void Initialize();

};


//Just for demo: This class is just a sample missile class.
class MissileStandard
{
public:
	int x,y,state;
	bool IsFlying(void);
	void Initialize(void);
	//bool CheckCollision(Target t);
	bool GoneOutOfWindow(void);
	bool TryShoot(int x0,int y0);
	void Move(void);
	void Draw(void);
};

MissileStandard missileStandard[nMissiles];

bool MissileStandard::IsFlying(void)
{
	return 0!=state;
}
void MissileStandard::Initialize(void)
{
	state=0;
}
/*bool MissileStandard::CheckCollision(Target t)
{
	return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileStandard::GoneOutOfWindow(void)
{
	return y<0;
}
void MissileStandard::Move(void)
{
	if(0!=state)
	{
		y-=10;
		if(true==GoneOutOfWindow())
		{
			state=0;
		}
	}
}
bool MissileStandard::TryShoot(int x0,int y0)
{
	if(0==state)
	{
		state=1;
		x=x0;
		y=y0;
		return true;
	}
	return false;
}
void MissileStandard::Draw(void)
{
	glColor3ub(255,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-2,y);
	glVertex2i(x+2,y);
	glVertex2i(x+2,y+9);
	glVertex2i(x-2,y+9);
	glEnd();
}

//Just for demo: This class is just a second sample missile class
class MissileB
{
public:
	int x,y,state;
	bool IsFlying(void);
	void Initialize(void);
	//bool CheckCollision(Target t);
	bool GoneOutOfWindow(void);
	bool TryShoot(int x0,int y0);
	void Move(void);
	void Draw(void);
};

MissileB missileB[nMissiles];

bool MissileB::IsFlying(void)
{
	return 0!=state;
}
void MissileB::Initialize(void)
{
	state=0;
}
/*bool MissileB::CheckCollision(Target t)
{
	return ::CheckCollision(x,y,t.x,t.y,t.w,t.h);
}*/
bool MissileB::GoneOutOfWindow(void)
{
	return y<0;
}
void MissileB::Move(void)
{
	if(0!=state)
	{
		y-=10;
		if(true==GoneOutOfWindow())
		{
			state=0;
		}
	}
}
bool MissileB::TryShoot(int x0,int y0)
{
	if(0==state)
	{
		state=1;
		x=x0;
		y=y0;
		return true;
	}
	return false;
}
void MissileB::Draw(void)
{
	glColor3ub(0,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-2,y);
	glVertex2i(x+2,y);
	glVertex2i(x+2,y+9);
	glVertex2i(x-2,y+9);
	glEnd();
}



// Enemy and enemy missiles


class Enemy
{
public:
	int x, y, xSize, ySize, state;
	//void Move();
};

class missileEnemy : public Enemy
{
public:
	void Move();
};

class standardEnemy : public Enemy
{
public:
	int pathPlace, state, fireMode;
	std::vector<int> movementPathX, movementPathY;
	std::vector<missileEnemy> missilesFired;
	standardEnemy();
	//~standardEnemy(); // may need to write destructor due to vector
	void generatePath1(const int startingYLocation);
	void generatePath2(const int startingYLocation);
	void generatePath3(const int startingYLocation);
	void generatePath4(const int startingYLocation);
	void generatePath5(const int startingyLocation);
	void generateRandomPath(const int startingYLocation);
	void Move();
	void Draw();
	void Fire();
	void resetPosition();
};

class magnetEnemy : public Enemy
{
	void Move(const int playerX, const int playerY); // could replace with a player object
	//void Draw();
};

void missileEnemy::Move()
{
	y += 20;
}

void standardEnemy::Fire()
{
	missileEnemy newMissile;
	newMissile.state = 1;
	newMissile.x = x + xSize / 2;
	newMissile.y = y;
	newMissile.xSize = 5;
	newMissile.ySize = 5;
	missilesFired.emplace_back(newMissile);
}

standardEnemy::standardEnemy()
{
	xSize = 20;
	ySize = 20;
	state = 1;
	pathPlace = 0;
	generateRandomPath(300); // generate some path
	//generatePath3(300);
}

void standardEnemy::generatePath1(const int startingYLocation)
{
	// Mapping some equation to the path
	for (float i = 0; i < 600; i++) { //assuming 100 fps (10ms sleep/frame)
		// equation used is sin(x)
		movementPathX.emplace_back(i); // need to change this to append to vector
		movementPathY.emplace_back((int)(startingYLocation + (float)100 * sin(i / 100.0)));
	}
}

void standardEnemy::generatePath2(const int startingYLocation)
{
	// Mapping some equation to the path
	for (float i = 600; i > 0; i--) { // 200 elements for a 4 second enemy on screen time, assuming 50 fps (25ms sleep/frame)
		// faster and larger than path 1
		movementPathX.emplace_back(i); // need to change this to append to vector
		movementPathY.emplace_back((int)(startingYLocation + (float)200 * sin(i / 50.0)));
	}
}

void standardEnemy::generatePath3(const int startingYLocation)
{
	for (float i = 0; i < 600; i++) {
		movementPathX.emplace_back(i);
		movementPathY.emplace_back((0.5 * i) + startingYLocation);
	}
}

void standardEnemy::generatePath4(const int startingYLocation)
{
	for (float i = 600; i > 0; i--) {
		movementPathX.emplace_back(i);
		movementPathY.emplace_back((0.5 * i) + startingYLocation);
	}
}

void standardEnemy::generatePath5(const int startingYLocation)
{
	for (float i = 0; i < 600; i++) {
		movementPathX.emplace_back(i);
		movementPathY.emplace_back(startingYLocation);
	}
}

void standardEnemy::generateRandomPath(const int startingYLocation)
{
	int randomNum = (rand() % 5) + 1; // 1 to 3
	switch (randomNum) {
	case 1:
		generatePath1(startingYLocation);
		break;
	case 2:
		generatePath2(startingYLocation);
		break;
	case 3:
		generatePath3(startingYLocation);
		break;
	case 4:
		generatePath4(startingYLocation);
		break;
	case 5:
		generatePath5(startingYLocation);
		break;
	default:
		generatePath2(startingYLocation);
		break;
	}
}

void standardEnemy::Move()
{
	// cycle through movement path
	if (state == 1) {
		pathPlace++;
		if (pathPlace < (movementPathX.size() - 1) && (pathPlace < movementPathY.size() - 1)) {
			x = movementPathX[pathPlace];
			y = movementPathY[pathPlace];
			if ((pathPlace % 100) == 0) {
				Fire();
			}
		}
		else {
			//state = 0; // kills enemy
			resetPosition(); // moves enemy back to other side of screen and keeps moving
		}
	}

	// update missiles fired
	for (auto& m : missilesFired) {
		if (m.y >= 800) {
			m.state = 0;
		}
		if (m.state == 1) {
			m.y += 3;
		}
	}
}

void standardEnemy::Draw()
{
	for (auto& m : missilesFired) {
		if (m.state == 1) {
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex2i(m.x, m.y);
			glVertex2i(m.x + m.xSize, m.y);
			glVertex2i(m.x + m.xSize, m.y + m.ySize);
			glVertex2i(m.x, m.y + m.ySize);
			glEnd();
		}
	}

	if (state == 1) {
		// just drawing a basic shape for now
		glColor3ub(255, 0, 0);
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + xSize, y);
		glVertex2i(x + xSize, y + ySize);
		glVertex2i(x, y + ySize);
		glEnd();
	}

}

void standardEnemy::resetPosition()
{
	pathPlace = 0;
}

void magnetEnemy::Move(const int playerX, const int playerY)
{
	// enemy gravitates toward player constantly
	if (y >= playerY) {
		if (x < playerX - 3)
		{
			x += 3;
		}
		else if (playerX + 3 < x)
		{
			x -= 3;
		}
	}
	y += 5; // enemy always travels down
}

// end of enemy and enemy missiles class code



// for testing
int main(){


  srand(time(NULL));
  const int nEnemy = 4;

  standardEnemy enemies[nEnemy];


  FsOpenWindow(16,16,600,800,1);

  Player player;
  int terminate=0;

  player.e.Initialize();



  while(terminate==0){

    FsPollDevice();
    int key=FsInkey();

    //only can control player when not spawning
    //The controls are just for demoing okay to change
      switch(key)
      {
      case FSKEY_ESC:
        terminate=1;
        break;
      case FSKEY_SPACE:
        player.Shoot();
    		break;
      case FSKEY_S://switch missile type
        player.ChangeMissile("Standard");
        break;
      case FSKEY_B://Switch missile type
        player.ChangeMissile("B");
        break;
      case FSKEY_H://Simulate a hit
        player.health=player.health-1;
        break;
      }
      if (0 != FsGetKeyState(FSKEY_LEFT))
      {
        player.x -= 10;
        if (player.x <= 0)
        {
          player.x = 0;
        }
      }
      if (0 != FsGetKeyState(FSKEY_RIGHT))
      {
        player.x += 10;
        if (player.x >= 600)
        {
          player.x = 600;
        }
      }
      if (0 != FsGetKeyState(FSKEY_UP))
      {
        player.y -= 10;
        if (player.y <= 0)
        {
          player.y = 0;
        }
      }
      if (0 != FsGetKeyState(FSKEY_DOWN))
      {
        player.y += 10;
        if (player.y >= 800)
        {
          player.y = 800;
        }
      }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    DrawAllMissiles(); //Ensures all missiles the are shot are drawn independent of what weapon type player is currently holding IE: will draw black missiles still in motion even if player switches to red missiles

    player.Initialize(); //does nothing if player state is 1 or not spawning, else will perform respawn behavior and animation

    //Health bar for demoing
    if(player.health>0) DrawRect(40,120,10*player.health,20);

    //draw player always when not mid spawning or dead
		if(player.state==1) player.Draw();

    //Just for demo: force field around player while spawning
    if(player.spawning){
      DrawCircle(player.x,player.y-5,30,0);
    }

    //If player health reaches 0 and player is not mid spawn, the player explodes
    if(player.health<=0 && !player.spawning){
			player.state=0;
			if(true==player.e.Begin(player.x,player.y)){
			}

    }
    //function calls to conitnue explosion
    if(player.e.state==1){
      player.e.Move();
      player.e.Draw();
      //Once explosion reaches a certain point player will begin to respawn
      if(player.e.counter>50){
        player.respawn=true;
        player.spawning=true;
      }
    }

	for (auto& e : enemies)
	{
		e.Move();
		e.Draw();
	}

    glFlush();
    FsSwapBuffers();
    FsSleep(25);
  }

}

//Ensures all missiles are drawn.
void DrawAllMissiles(){

    for(auto &m : missileStandard)
    {
      if(m.IsFlying())
      {
        m.Draw();
      }
    }
    for(auto &m : missileStandard)
    {
      m.Move();
    }

    for(auto &m : missileB)
    {
      if(m.IsFlying())
      {
        m.Draw();
      }
    }
    for(auto &m : missileB)
    {
      m.Move();
    }
}

void DrawRect(int blockX,int blockY, int blockSizeX, int blockSizeY)
{

  glColor3ub(0,0,255);
  glBegin(GL_QUADS);
  glVertex2i(blockX,blockY+blockSizeY);
  glVertex2i(blockX,blockY-blockSizeY);
  glVertex2i(blockX+blockSizeX,blockY-blockSizeY);
  glVertex2i(blockX+blockSizeX,blockY+blockSizeY);
  glEnd();
}

void DrawCircle(int cx,int cy,int rad,int fill)
{

    const double YS_PI=3.1415927;

    if(0!=fill)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    int i;
    for(i=0; i<64; i++)
    {
        double angle=(double)i*YS_PI/32.0;
        double x=(double)cx+cos(angle)*(double)rad;
        double y=(double)cy+sin(angle)*(double)rad;
        glVertex2d(x,y);
    }

    glEnd();
}


void Player::Initialize(){

  if(state==0 && spawning){
    if(respawn){//Only start ship here when it is first respawning
      x=300;
      y=1000;
      respawn=false;
    }
    if(y>700) y=y-10;
    else{
      initializeCount++;
      if(initializeCount>70)
      {
        state=1;
        health=5;
        initializeCount=0;
        spawning=false;
      }
      }
    Draw();
  }

  else return;
}

void Player::Draw()
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

void Player::Shoot()
{
  if(missileType=="Standard"){
    for(auto &m : missileStandard)
    {
      if(true==m.TryShoot(x,y))
      {
        //player.PlayOneShot(program.missileSE);
        //++nUsed;
        break;
      }
    }
  }

  if(missileType=="B"){
    for(auto &m : missileB)
    {
      if(true==m.TryShoot(x,y))
      {
        //player.PlayOneShot(program.missileSE);
        //++nUsed;
        break;
      }
    }
  }

}

void Player::ChangeMissile(string newType){

  missileType=newType;

  if(missileType=="Standard"){

    for(auto &m : missileStandard)
    {
        m.Initialize();
      }

  }

  if(missileType=="B"){

    for(auto &m : missileB)
    {
        m.Initialize();
      }

  }

}
