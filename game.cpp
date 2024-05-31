//============================================================================
// Name        : .cpp
// Author      : 
// Version     :
// Copyright   : (c) Reserved
// Description : 
//============================================================================

#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

class Grid;
class Brick;
class Ball;
class PowerUp;
class Paddle;
class Point;
class Level;

class Grid {
	protected:
	int width, height;
	int xOffset, yOffset;
	int rows, cols, lives, gameState=0, score=0;
	
	public:
	Grid(int w=800, int h=640, int r=15, int c=10, int x=100, int y=100, int l=3) {
		width = w;
		height = h;
		rows = r;
		cols = c;
		xOffset=x;
		yOffset=y;
		lives=l;
	}
	void setGrid(int w, int h, int r, int c) {
		width = w;
		height = h;
		rows = r;
		cols = c;
		xOffset=100;
		yOffset=100;
	}
	void draw() {
		srand(time(0)); 
		DrawLine(xOffset, yOffset, xOffset+width, yOffset, 6, colors[WHITE]);
		DrawLine(xOffset, yOffset, xOffset, yOffset+height, 6, colors[WHITE]);
		DrawLine(xOffset+width, yOffset, xOffset+width, yOffset+height, 6, colors[WHITE]);
		DrawLine(xOffset, yOffset+height, xOffset+width, yOffset+height, 6, colors[WHITE]);
		DrawSquare(xOffset-5, yOffset-5, 12, colors[WHITE]);
		DrawSquare(xOffset+width-5, yOffset-5, 12, colors[WHITE]);
		DrawSquare(xOffset-5, yOffset+height-5, 12, colors[WHITE]);
		DrawSquare(xOffset+width-5, yOffset+height-5, 12, colors[WHITE]);
	}
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int h) {
		height = h;
	}
};

class Point {
	public:
	float x, y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(float xtoo, float ytoo) {
		x = xtoo;
		y = ytoo;
	}
};

class Ball: public Grid {
	protected:
	float radius;
	Point coord;
	float speed;
	float* colour;
	bool alive;
	float dx=0, dy=-1;

	public:
	Ball(float r=5, int x=500, int y=340, float s=5, int l=3, float* c=colors[WHITE]) {
		radius = r;
		coord.x = x;
		coord.y = y;
		speed = s;
		lives=l;
		colour = c;
	}
	Ball (const Ball& b) { // copy constructor
		radius = b.radius;
		coord.x = b.coord.x;
		coord.y = b.coord.y;
		speed = b.speed;
		colour = b.colour;
		dx = b.dx;
		dy = b.dy;
	}
	void draw() {
		DrawCircle(coord.x, coord.y, radius, colour);
		DrawCircle(coord.x, coord.y, 4, colors[BLUE]);
	}
	void changeColor(float* c) {
		colour = c;
	}
	void changeSpeed(float s) {
		speed = s;
	}
	void setcoord(int x, int y) {
		coord.x = x;
		coord.y = y;
	}
	void setRadius(float r) {
		radius = r;
	}
	void setSpeed(float s) {
		speed = s;
	}
	void setColour(float* c) {
		colour = c;
	}
	bool checkCollision(int currLevel) {
		if (coord.x-radius<=xOffset || coord.x+radius>=xOffset+width)
			dx = -dx;
		else if (coord.y+radius>=yOffset+height) {
			if (currLevel==3) {
				lives--;
				alive=false;
				return false;
			}
			dy = -dy;
		}
		else if (coord.y-radius<=yOffset) {
			lives--;
			alive=false;
			return false;
		}
		return true;
	}
	float getDirectionX() {
		return dx;
	}
	float getDirectionY() {
		return dy;
	}
	float getSpeed() {
		return speed;
	}
	Point getCoord() {
        return coord;
    }
	int getRadius() {
		return radius;
	}
	void setDirection(float x, float y) {
		dx = x;
		dy = y;
	}
	void setCoord(float x, float y) {
		coord.x = x;
		coord.y = y;
	}
	void setAlive (bool a) {
		alive = a;
	}
	bool getAlive() {
		return alive;
	}
	bool move(int currLevel) {
		coord.x += dx;
		coord.y += dy;
		return checkCollision(currLevel);
	}
};

class PowerUp: public Grid {
	protected:
	Point coord;
	float* colour;
	float speed;
	float dx=0, dy=-1;
	int type, width=10, time=0;
	bool enabled =false, triggered=false;

	public:
	PowerUp(int x=500, int y=340, float s=10, int lives=1) {
		coord.x = x;
		coord.y = y;
		speed = s;
		type= (lives==1) ? 3: (lives==2) ? 2: (lives==3) ? 1: (lives==4) ? 0: 4;
		setColour();
	}
	void setCoord(int x, int y) {
		coord.x = x;
		coord.y = y;
		type= rand()%5;
		setColour();
	}
	void setTime(int t) {
		time = t;
	}
	int getTime() {
		return time;
	}
	Point getCoord() {
		return coord;
	}
	bool getEnabled() {
		return enabled;
	}
	bool getTriggered() {
		return triggered;
	}
	void draw() {
		if (time<=0 || triggered)
			return;
		if (type==0)
			DrawRectangle(coord.x, coord.y, width*2, width, colour);
		else if (type==1)
			DrawCircle(coord.x, coord.y, width/2, colour);
		else if (type==2)
			DrawSquare(coord.x, coord.y, width, colour);
		else if (type==3)
			DrawTriangle(coord.x, coord.y, coord.x+width, coord.y, coord.x+width/2, coord.y+width,  colour);
		else if (type==4)
			DrawSquare(coord.x, coord.y, width, colour);
	}
	void setColour() {
		if (type == 0)
			colour = colors[RED];
		else if (type == 1)
			colour = colors[BLUE];
		else if (type == 2)
			colour = colors[DEEP_PINK];
		else if (type == 3)
			colour = colors[GREEN];
		else if (type == 4)
			colour = colors[YELLOW];
	}
	void move(Point& pad, float &w, float &h, Ball &ball, int& numBalls) {
		coord.x += dx;
		coord.y += dy;
		if (coord.y==yOffset) {
			dy=0;
			triggered=true;
		}
		else {
			checkCollision(pad, w, h);
		}
		powerUpTrigger(pad, w, h, ball, numBalls);
	}
	void powerUpTrigger(Point& pad, float &w2, float &h2, Ball &ball, int& numBalls) {
		if (enabled) {
			if (!triggered) {
				if (type==0)// RED: spped ball (5 sec)
					ball.changeSpeed(ball.getSpeed()*2);
				else if (type==1) // BLUE: slow ball (5 sec)
					ball.changeSpeed(ball.getSpeed()/2);
				else if (type==2) // PINK: decrease paddle size 0.5 (5 sec)
					w2/=2;
				else if (type==3) // Green: increase paddle size 0.5 (5 sec)
					w2*=2;
				else if (type==4) // Yellow: two more bawls (5 sec)
					numBalls=3;
				triggered=true;
			}
			time++;
		}
		if ((time>=700 && type!=4) || (time>=3000 && type==4) ) {
			if (type==0)
				ball.changeSpeed(ball.getSpeed()/2);
			else if (type==1)
				ball.changeSpeed(ball.getSpeed()*2);
			else if (type==2)
				w2*=2;
			else if (type==3)
				w2/=2;
			else if (type==4)
				numBalls=1;
			enabled=false;
			time=0;
		}
	}	
	void checkCollision(Point& pad, float &w, float &h) {
		if (coord.x + width >= pad.x && coord.x - width <= pad.x + w && coord.y + width >= pad.y && coord.y - width <= pad.y + h) {
			enabled=true;
		}
	}
};

class Brick {
	protected:
	float width, height;
	Point coord;
	int lives, totalLives;
	float* colour;
	float borderThickness=5.0;
	PowerUp powerUp;

	public:
	Brick(float w=100.0, float h=25.0, int x=0, int y=0, int l=3) {
		width = w;
		height = h;
		coord.x = x;
		coord.y = y;
		lives = l;
		setColour();
	}
	void setBrick(float w, float h, int x, int y, int l) {
		width = w;
		height = h;
		coord.x = x;
		coord.y = y;
		lives = l;
		totalLives=l;
		setColour();
	}
	void draw() {
		DrawRectangle(coord.x, coord.y, width, height, colour);
		float lineY = coord.y + height * 0.35;
		DrawLine(coord.x+10, lineY, coord.x + width-10, lineY, 2, colors[BLACK]);
		DrawLine(coord.x, coord.y, coord.x+10, lineY,2, colors[BLACK]);
		DrawLine(coord.x + width, coord.y, coord.x + width-10, lineY, 2, colors[BLACK]);
		DrawLine(coord.x, coord.y + height, coord.x+10, lineY, 2, colors[BLACK]);
		DrawLine(coord.x + width, coord.y + height, coord.x + width-10, lineY, 3, colors[BLACK]);
	}
	void drawPowerUp() {
		powerUp.draw();
	}
	void movePowerUp(Point& pad, float &w, float &h, Ball &ball, int &numBalls) {
		if (powerUp.getTime()>0 )
			powerUp.move(pad, w, h, ball, numBalls);
	}
	void setColour() {
		if (lives == 5)
			colour = colors[GOLD];
		else if (lives == 4)
			colour = colors[RED];
		else if (lives == 3)
			colour = colors[BLUE];
		else if (lives == 2)
			colour = colors[DEEP_PINK];
		else if (lives == 1)
			colour = colors[GREEN];
	}
	void setWidth(float w) {
		width = w;
	}
	void setHeight(float h) {
		height = h;
	}
	void setCoord(int x, int y) {
		coord.x = x;
		coord.y = y;
	}
	int getLives() {
		return lives;
	}
	bool checkCollision(Ball &ball, Ball& ball2, Ball& ball3, Point& pad, float &w, float &h, int &numBalls, int ballNum) {
		bool collision = false;
		if (ball.getCoord().x + ball.getRadius() >= coord.x && ball.getCoord().x - ball.getRadius() <= coord.x + width) {
			if (ball.getCoord().y - ball.getRadius() <= coord.y && ball.getCoord().y + ball.getRadius() >= coord.y) {
				if(ball.getDirectionY() > 0) {
					ball.setDirection(ball.getDirectionX(), -ball.getDirectionY());
					collision = true;
				}
			}
			else if (ball.getCoord().y + ball.getRadius() >= coord.y + height && ball.getCoord().y - ball.getRadius() <= coord.y + height) {
				if(ball.getDirectionY() < 0) {
					ball.setDirection(ball.getDirectionX(), -ball.getDirectionY());
					collision = true;
				}
			}
		}
		if (!collision && ball.getCoord().y + ball.getRadius() >= coord.y && ball.getCoord().y - ball.getRadius() <= coord.y + height) {
			if (ball.getCoord().x - ball.getRadius() <= coord.x && ball.getCoord().x + ball.getRadius() >= coord.x) {
				if(ball.getDirectionX() > 0) {
					ball.setDirection(-ball.getDirectionX(), ball.getDirectionY());
					collision = true;
				}
			}
			else if (ball.getCoord().x + ball.getRadius() >= coord.x + width && ball.getCoord().x - ball.getRadius() <= coord.x + width) {
				if(ball.getDirectionX() < 0) {
					ball.setDirection(-ball.getDirectionX(), ball.getDirectionY());
					collision = true;
				}
			}
		}
		int num;
		if (collision) {
			num= 0;
			if (num==0 && lives==1 && numBalls==1) {
				powerUp = PowerUp(ball.getCoord().x, ball.getCoord().y, 10, totalLives);
				if (totalLives==5 && ballNum==1) {
					ball2.setCoord(400, 400);
					ball3.setCoord(600, 400);
					ball2.setDirection(0.3, -0.7);
					ball3.setDirection(-0.3, -0.7);
					ball2.setColour(colors[WHITE]);
					ball3.setColour(colors[WHITE]);
					ball2.setAlive(true);
					ball3.setAlive(true);
				}
				powerUp.setTime(1);
			}
			lives--;
			setColour();
		}
		return collision;
	}
	int getTotalLives() {
		return totalLives;
	}
};

class Level {
	public:
	virtual void createLevel(Brick **&bricks) {}
	virtual void drawBricks(Brick **&bricks) {}
	virtual void checkBrickCollision(Brick **&bricks, Ball &ball, Ball &ball2, Ball &ball3, Point& coord, float &width, float &height, int &numBalls, int& score) {}
	virtual bool checkWin(Brick **&bricks) {}
};
class Level1 : public Level, public Grid {
	public:
	void createLevel(Brick **&bricks) {
		float brickWidth = width/cols;
		float brickHeight = 25;
		bricks = new Brick*[rows];
		for (int i=0; i<10; i++) {
			bricks[i] = new Brick[cols];
			for (int j=0; j<cols; j++) {
				int lives=rand()%5+1;
				bricks[i][j].setBrick(brickWidth, brickHeight, xOffset+j*brickWidth, Grid::height -i*brickHeight, lives);
			}
		}
	}
	void drawBricks(Brick **&bricks) {
		for (int i=0; i<10; i++) {
			for (int j=0; j<cols; j++) {
				if (bricks[i][j].getLives()>0)
				bricks[i][j].draw();
				bricks[i][j].drawPowerUp();
			}
		}
	}
	void checkBrickCollision(Brick **&bricks, Ball &ball, Ball &ball2, Ball &ball3, Point& coord, float &width, float &height, int &numBalls, int& score) {
		bool collision =false, collision2=false, collision3= false;
		for (int i=0; i<10; i++) {
			for (int j=0; j<cols; j++) {
				if (bricks[i][j].getLives()>0 && !collision) {
					collision=bricks[i][j].checkCollision(ball, ball2, ball3, coord, width, height, numBalls, 1);
					if (collision) score+=2;
				}
				bricks[i][j].movePowerUp(coord, width, height, ball, numBalls);
				if (numBalls>1) {
					if (bricks[i][j].getLives()>0 && !collision2) {
						collision2=bricks[i][j].checkCollision(ball2, ball, ball3, coord, width, height, numBalls, 2);
						if (collision2) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball2,  numBalls);
					if (bricks[i][j].getLives()>0 && !collision) {
						collision3=bricks[i][j].checkCollision(ball3, ball, ball2, coord, width, height, numBalls, 3);
						if (collision3) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball3, numBalls);
				}
			}
		}
	}
	bool checkWin(Brick **&bricks) {
		for (int i=0; i<5; i++) {
			for (int j=0; j<cols; j++) {
				if (bricks[i][j].getLives()>0)
					return false;
			}
		}
		return true;
	}
};
class Level2 : public Level, public Grid {
	public:
	void createLevel(Brick **&bricks) {
		float brickOffset=0;
		float brickWidth = width/9;
		float brickHeight = 20;
		bricks = new Brick*[15];
		for (int i=0; i<15; i++) {
			bricks[i] = new Brick[3];
			for (int j=0; j<3; j++) {
				int lives=rand()%5+1;
				bricks[i][j].setBrick(brickWidth, brickHeight, brickOffset + xOffset+j*brickWidth, Grid::height -i*brickHeight, lives);
			}
			if (i==2 || i==5 )
				brickOffset+=3*brickWidth;
			else if (i==8 || i==11 )
				brickOffset-=3*brickWidth;
		}
	}
	void drawBricks(Brick **&bricks) {
		for (int i=0; i<15; i++) {
			for (int j=0; j<3; j++) {
				if (bricks[i][j].getLives()>0)
				bricks[i][j].draw();
				bricks[i][j].drawPowerUp();
			}
		}
	}
	void checkBrickCollision(Brick **&bricks, Ball &ball, Ball &ball2, Ball &ball3, Point& coord, float &width, float &height, int &numBalls, int& score) {
		bool collision = false, collision2=false, collision3=false;
		for (int i=0; i<15; i++) {
			for (int j=0; j<3; j++) {
				if (bricks[i][j].getLives()>0 && !collision) {
					collision=bricks[i][j].checkCollision(ball, ball2, ball3, coord, width, height, numBalls, 1);
					if (collision) score+=2;
				}
				bricks[i][j].movePowerUp(coord, width, height, ball, numBalls);
				if (numBalls>1) {
					if (bricks[i][j].getLives()>0 && !collision) {
						collision2=bricks[i][j].checkCollision(ball2, ball, ball3, coord, width, height, numBalls, 2);
						if (collision2) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball2,  numBalls);
					if (bricks[i][j].getLives()>0 && !collision) {
						collision3=bricks[i][j].checkCollision(ball3, ball, ball2, coord, width, height, numBalls, 3);
						if (collision3) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball3, numBalls);
				}
			}
		}
	
	}
	bool checkWin(Brick **&bricks) {
		for (int i=0; i<15; i++) {
			for (int j=0; j<3; j++) {
				if (bricks[i][j].getLives()>0)
					return false;
			}
		}
		return true;
	}
};
class Level3 : public Level, public Grid {
	public:
	void createBrickCols(Brick **&bricks, int n, float offest, int i, int j=0) {
		int lives=rand()%5+1;
		bricks[i][j].setBrick(Grid::width/9, 25, xOffset+ offest, Grid::height -i*25-100, lives);
		// cout<<xOffset+ offest<<" "<<Grid::height -i*25<<" , ";
		if (j<n-1)
			createBrickCols(bricks, n, offest+Grid::width/9, i, j+1);
	}
	void createBrickRows(Brick **&bricks, int n, float offset, int k=4, int i=1, int m=1) {
		bricks[m]= new Brick[n-i];
		createBrickCols(bricks, k, 0, m);
		createBrickCols(bricks, k*2, offset, m, k);//cout<<endl;
		offset+=Grid::width/n;
		if (m<n/2) {
			createBrickRows(bricks, n, offset, k-1, i+2, m+1);
		}
		if (m<n/2 && m>=1) {
			bricks[n-m-1]= new Brick[n-i];
			createBrickCols(bricks, k, 0, n-m-1);
			offset-=Grid::width/n;
			createBrickCols(bricks, k*2, offset, n-m-1, k);//cout<<endl;
		}
	}
	void createRecursiveLevel(Brick **&bricks, int n=9, int i=0) {
		bricks = new Brick*[n];
		bricks[0] = new Brick[n];
		createBrickCols(bricks, n, 0, 0);//cout<<endl;
		createBrickRows(bricks, n, Grid::width/n*(n+1)/2, n/2);
		bricks[n-1] = new Brick[n];
		createBrickCols(bricks, n, 0, n-1);//cout<<endl;
	}
	void createLevel(Brick **&bricks) {
		createRecursiveLevel(bricks);
	}
	void drawBricks(Brick **&bricks) {
		int n=9;
		for (int i=0; i<n; i++) {
			int numBricks;
			if (i==0 || i==n-1)
				numBricks=n;
			else if (i<=n/2)
				numBricks=(n+1)-i*2;
			else if (i<n-1)
				numBricks=2*(i-3);

			for (int j=0; j<numBricks; j++) {
				if (bricks[i][j].getLives()>0)
					bricks[i][j].draw();
				bricks[i][j].drawPowerUp();
			}
		}
	}
	void checkBrickCollision(Brick **&bricks, Ball &ball, Ball &ball2, Ball &ball3, Point& coord, float &width, float &height, int &numBalls, int& score) {
		int n=9;
	    bool collision = false, collision2=false, collision3=false;
		for (int i=0; i<n; i++) {
			int numBricks;
			if (i==0 || i==n-1)
				numBricks=n;
			else if (i<=n/2)
				numBricks=(n+1)-i*2;
			else if (i<n-1)
				numBricks=2*(i-3);
			for (int j=0; j<numBricks; j++) {
				if (bricks[i][j].getLives()>0 && !collision) {
					collision=bricks[i][j].checkCollision(ball, ball2, ball3, coord, width, height, numBalls, 1);
					if (collision) score+=2;
				}
				bricks[i][j].movePowerUp(coord, width, height, ball, numBalls);
				if (numBalls>1) {
					if (bricks[i][j].getLives()>0 && !collision) {
						collision2=bricks[i][j].checkCollision(ball2, ball, ball3, coord, width, height, numBalls, 2);
						if (collision2) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball2,  numBalls);
					if (bricks[i][j].getLives()>0 && !collision) {
						collision3=bricks[i][j].checkCollision(ball3, ball, ball2, coord, width, height, numBalls, 3);
						if (collision3) score+=2;
					}
					bricks[i][j].movePowerUp(coord, width, height, ball3, numBalls);
				}
			}
		}
	}
	bool checkWin(Brick **&bricks) {
		int n=9;
		for (int i=0; i<n; i++) {
			int numBricks;
			if (i==0 || i==n-1)
				numBricks=n;
			else if (i<=n/2)
				numBricks=(n+1)-i*2;
			else if (i<n-1)
				numBricks=2*(i-3);
			for (int j=0; j<numBricks; j++) {
				if (bricks[i][j].getLives()>0)
					return false;
			}
		}
		return true;
	}
};

class Paddle: public Grid, public Level {
	protected:
	int strokeWidth, numBalls=1, currLevel=1;
	Ball ball, ball2, ball3;
	Point coord;
	Point coord2;
	float width, height;
	int lastScore;
	float* stroke;
	float* fill;
	Level* level;
	Brick** bricks;
	string playerName="";

	public:
	Paddle(float w=100.0, float h=15.0, int x=400.0, int y=150.0, float* c=colors[WHITE], float* f=colors[BROWN], int sw=3) {
		width = w;
		height = h;
		this->coord.x = x;
		this->coord.y = y;
		stroke = c;
		fill = f;
		strokeWidth = sw;
		if (currLevel==1)
			level = new Level1();
		else if (currLevel==2)
			level = new Level2();
		else if (currLevel==3)
			level = new Level3();
		level->createLevel(bricks);
		lives=3;
	}
	void draw() {
		DrawRectangle(coord.x, coord.y, width, height, fill);
		DrawRectangle( coord.x + (width - width/3) / 2, coord.y + (height - height/2) / 2, width/3, height / 2, stroke);
		DrawLine(coord.x, coord.y, coord.x + width, coord.y, strokeWidth, stroke);
		DrawLine(coord.x, coord.y, coord.x, coord.y + height, strokeWidth, stroke);
		DrawLine(coord.x + width, coord.y, coord.x + width, coord.y + height, strokeWidth, stroke);
		DrawLine(coord.x, coord.y + height, coord.x + width, coord.y + height, strokeWidth, stroke);
		if (currLevel==3) {
			DrawRectangle(coord2.x, coord2.y, width, height, fill);
			DrawRectangle( coord2.x + (width - width/3) / 2, coord2.y + (height - height/2) / 2, width/3, height / 2, stroke);
			DrawLine(coord2.x, coord2.y, coord2.x + width, coord2.y, strokeWidth, stroke);
			DrawLine(coord2.x, coord2.y, coord2.x, coord2.y + height, strokeWidth, stroke);
			DrawLine(coord2.x + width, coord2.y, coord2.x + width, coord2.y + height, strokeWidth, stroke);
			DrawLine(coord2.x, coord2.y + height, coord2.x + width, coord2.y + height, strokeWidth, stroke);
		}
		ball.draw();
		if (numBalls>1) {
			if (ball2.getAlive())
				ball2.draw();
			if (ball3.getAlive())
				ball3.draw();
		}
	}
	void setStrokeWidth(int sw) {
		strokeWidth = sw;
	}
	void setStroke(float* c) {
		stroke = c;
	}
	void setFill(float* c) {
		fill = c;
	}
	void setCoord(float x, float y=150.0) {
		if (x-width/2>xOffset && x+width/2<xOffset+Grid::width) {
			coord.x = x - width/2;
			coord.y = y;
		}
	}
	void setCoord2(float x, float y=680.0) {
		if (x>xOffset && x+width<xOffset+Grid::width) {
			coord2.x = x;
			coord2.y = y;
		}
	}
	void setScore(int s) {
		score = s;
	}
	void setGameState(int s) {
		gameState = s;
	}
	void checkCollision() {
		int speed = ball.getSpeed();
		if (ball.getCoord().x +ball.getRadius() >= coord.x && ball.getCoord().x - ball.getRadius() <= coord.x + width && ball.getCoord().y + ball.getRadius() >= coord.y && ball.getCoord().y - ball.getRadius() <= coord.y + height) {
			float x, y, x2;
			x=  (ball.getCoord().x - (coord.x +width/2))/width;
			x2= (x<0)?-x:x;
			y=1-x2;
			x*=speed;
			y*=speed;
			ball.setDirection(x, y);
		}
		else if (currLevel==3) 
		if (ball.getCoord().x +ball.getRadius() >= coord2.x && ball.getCoord().x - ball.getRadius() <= coord2.x + width && ball.getCoord().y + ball.getRadius() >= coord2.y && ball.getCoord().y - ball.getRadius() <= coord2.y + height) {
			float x, y, x2;
			x=  (ball.getCoord().x - (coord2.x +width/2))/width;
			x2= (x<0)?-x:x;
			y=1-x2;
			x*=speed;
			y*=-speed;
			ball.setDirection(x, y);
		}
		if (numBalls >1) {
			if (ball2.getAlive()) {
				int speed2 = ball.getSpeed();
				if (ball2.getCoord().x +ball2.getRadius() >= coord.x && ball2.getCoord().x - ball2.getRadius() <= coord.x + width && ball2.getCoord().y + ball2.getRadius() >= coord.y && ball2.getCoord().y - ball2.getRadius() <= coord.y + height) {
					float x, y, x2;
					x=  (ball2.getCoord().x - (coord.x +width/2))/width;
					x2= (x<0)?-x:x;
					y=1-x2;
					x*=speed2;
					y*=speed2;
					ball2.setDirection(x, y);
				}
				else if (currLevel==3)
				if (ball2.getCoord().x +ball2.getRadius() >= coord2.x && ball2.getCoord().x - ball2.getRadius() <= coord2.x + width && ball2.getCoord().y + ball2.getRadius() >= coord2.y && ball2.getCoord().y - ball2.getRadius() <= coord2.y + height) {
					float x, y, x2;
					x=  (ball2.getCoord().x - (coord2.x +width/2))/width;
					x2= (x<0)?-x:x;
					y=1-x2;
					x*=speed2;
					y*=-speed2;
					ball2.setDirection(x, y);
				}
			}
			if (ball3.getAlive()) {
				int speed3 = ball.getSpeed();
				if (ball3.getCoord().x +ball3.getRadius() >= coord.x && ball3.getCoord().x - ball3.getRadius() <= coord.x + width && ball3.getCoord().y + ball3.getRadius() >= coord.y && ball3.getCoord().y - ball3.getRadius() <= coord.y + height) {
					float x, y, x2;
					x=  (ball3.getCoord().x - (coord.x +width/2))/width;
					x2= (x<0)?-x:x;
					y=1-x2;
					x*=speed3;
					y*=speed3;
					ball3.setDirection(x, y);
				}
				else if (currLevel==3)
				if (ball3.getCoord().x +ball3.getRadius() >= coord2.x && ball3.getCoord().x - ball3.getRadius() <= coord2.x + width && ball3.getCoord().y + ball3.getRadius() >= coord2.y && ball3.getCoord().y - ball3.getRadius() <= coord2.y + height) {
					float x, y, x2;
					x=  (ball3.getCoord().x - (coord2.x +width/2))/width;
					x2= (x<0)?-x:x;
					y=1-x2;
					x*=speed3;
					y*=-speed3;
					ball3.setDirection(x, y);
				}
			}
		}
	}
	float getSpeed() {
		return ball.getSpeed();
	}
	int getX() {
		return coord.x;
	}
	int getX2() {
		return coord2.x;
	}
	int getY() {
		return coord.y;
	}
	int getY2() {
		return coord2.y;
	}
	int getLives() {
		return lives;
	}
	int getScore() {
		return score;
	}
	int getCurrentLevel() {
		return currLevel;
	}
	int getLevel() {
		return currLevel;
	}
	Ball& getBall() {
		return ball;
	}
	string getPlayerName() {
		return playerName;
	}
	void setPlayerName(string name) {
		playerName = name;
	}
	void setLives(int l) {
		lives = l;
	}
	void setLastScore(int s) {
		lastScore = s;
	}
	int getLastScore() {
		return lastScore;
	}
	int getGameState() {
		return gameState;
	}
	void moveBall() {
		bool out1, out2, out3;
		out1=ball.move(currLevel);
		
		if (numBalls>1) {
			if (ball2.getAlive())
				out2=ball2.move(currLevel);
			if (ball3.getAlive())
				out3=ball3.move(currLevel);

			if (!out1 && (out2 || out3)) {
				lives++; Ball temp=ball;
				if (out2) { ball=ball2; ball2=temp; }
				else { ball=ball3; ball2=temp; }
			}
		} else if (!out1 && lives>0) {
			lives--;
			ball.setCoord(500, 340);
			ball.setDirection(0, -1);
		}
		
		checkCollision();
		level->checkBrickCollision(bricks, ball, ball2, ball3, coord, Paddle::width, Paddle::height, numBalls, score);
	}
	void drawBricks() {
		level->drawBricks(bricks);
	}
	void setCurrentLevel(int k) {
		if (currLevel==1)
			deleteLevel1();
		else if (currLevel==2)
			deleteLevel2();
		else if (currLevel==3)
			deleteLevel3();
		currLevel = k;
		if (currLevel==1)
			level = new Level1();
		else if (currLevel==2)
			level = new Level2();
		else if (currLevel==3) {
			level = new Level3();
			coord2.x = 400;
			coord2.y = Grid::height+yOffset-60;
		}
		level->createLevel(bricks);
		lives=3;
	}
	void deleteLevel1() {
		for (int i=0; i<10; i++) {
			delete[] bricks[i];
		}
		delete[] bricks;
	}
	void deleteLevel2() {
		for (int i=0; i<15; i++) {
			delete[] bricks[i];
		}
		delete[] bricks;
	}
	void deleteLevel3() {
		for (int i=0; i<9; i++) {
			delete[] bricks[i];
		}
		delete[] bricks;
	}
	bool checkWin() {
		return level->checkWin(bricks);
	}
	int stringToInt(string s) {
		int num=0;
		for (int i=0; i<s.length(); i++) {
			num*=10;
			num+=s[i]-'0';
		}
		return num;
	}
	~Paddle() {
		if (currLevel==1)
			deleteLevel1();
		if (currLevel==2)
			deleteLevel2();
		if (currLevel==3)
			deleteLevel3();
	}
};


/*Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)*/
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

Paddle pad;
//gameState:: 0=menu, 1=level select, 2=pause/resume, 4=game over, 5=won, 6=playing, 7=entering name
void GameDisplay() {
	// set the background color using function glClearColor.
	glClearColor(0/*Red*/, 0/*Green*/, 0.0/*Blue */, 0 /*Alpha*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	
	if (pad.getGameState()==0) {
		DrawString( 230, 640, "Press 's' or click \"Start\" below to start the game", colors[MISTY_ROSE]);
		DrawString( 230, 570, "Please Enter Your Name by clicking the Box Below", colors[MISTY_ROSE]);
		DrawRectangle( 230, 500, 200, 40, colors[WHITE]);
		DrawString( 235, 510, pad.getPlayerName(), colors[BLACK]);
		DrawRectangle( 400, 350, 100, 40, colors[WHITE]);
		DrawString( 425, 360, "Start", colors[BLACK]);
		DrawRectangle(385, 420, 135, 40, colors[WHITE]);
		DrawString( 395, 430, "High Score", colors[BLACK]);
	}
	else if (pad.getGameState()==1) {
		DrawString( 350, 420, "Select a level", colors[MISTY_ROSE]);
		DrawSquare( 290, 360, 25, colors[WHITE]);
		DrawSquare( 390, 360, 25, colors[WHITE]);
		DrawSquare( 490, 360, 25, colors[WHITE]);
		DrawString( 295, 365, "1", colors[BLACK]);
		DrawString( 395, 365, "2", colors[BLACK]);
		DrawString( 495, 365, "3", colors[BLACK]);
	}
	else if (pad.getGameState()==2) {
		DrawString( 240, 420, "Press 'r' or click \"Resume\" below to resume the game", colors[MISTY_ROSE]);
		DrawRectangle( 400, 350, 135, 40, colors[WHITE]);
		DrawString( 425, 360, "Resume", colors[BLACK]);
		DrawRectangle (350, 300, 240, 40, colors[WHITE]);
		DrawString( 375, 310, "Exit to Main Menu", colors[RED]);
	}
	else if (pad.getGameState()==3) {
		fstream file("highscore.txt", ios::in);
		string input1, input2, input3, name1="Player Unknown", name2="Player Unknown", name3="Player Unknown";
		int highscore1=0, highscore2=0, highscore3=0;
		if (file.is_open()) {
			while (getline(file, input1, ',') && getline(file, input2, ',') && getline(file, input3) ) {
				int score=pad.stringToInt(input1);
				if ( input3=="level1" && score> highscore1) {
					highscore1= pad.stringToInt(input1);
					name1 = input2;
				}
				else if (input3=="level2" && score> highscore2) {
					highscore2= pad.stringToInt(input1);
					name2 = input2;
				}
				else if (input3=="level3" && score> highscore3) {
					highscore3= pad.stringToInt(input1);
					name3 = input2;
				}
			}
		}
		file.close();
		string hs=to_string(highscore1); string hs2=to_string(highscore2); string hs3=to_string(highscore3);
		DrawString( 280, 450, "Scores to beat: ", colors[MISTY_ROSE]);
		DrawString( 280, 420, "Level 1:     "+name1+": "+hs, colors[MISTY_ROSE]);
		DrawString( 280, 390, "Level 2:     "+name2+": "+hs2, colors[MISTY_ROSE]);
		DrawString( 280, 360, "Level 3:     "+name3+": "+hs3, colors[MISTY_ROSE]);
	}
	else if (pad.getGameState()==4) {
		DrawString( 230, 420, "Game Over", colors[MISTY_ROSE]);
		DrawString ( 230, 380, "Score="+to_string(pad.getLastScore()), colors[MISTY_ROSE]);
		DrawRectangle( 230, 330, 240, 40, colors[WHITE]);
		DrawString( 255, 340, "Exit to Main Menu", colors[RED]);
	}
	else if (pad.getGameState()==5) {
		DrawString (230, 460, "Score="+to_string(pad.getLastScore()), colors[MISTY_ROSE]);
		DrawString( 230, 420, "You Won", colors[MISTY_ROSE]);
		DrawRectangle( 230, 350, 240, 40, colors[WHITE]);
		DrawString( 255, 360, "Exit to Main Menu", colors[RED]);	
	}
	else if (pad.getGameState()==6) {
		string score=to_string(pad.getScore());
		string lives=to_string(pad.getLives());
		DrawString( 100, 780, "Score="+score, colors[MISTY_ROSE]);
		DrawString( 210, 780, "Lives="+lives, colors[MISTY_ROSE]);
		DrawRectangle(805, 768, 95, 40, colors[WHITE]);
		DrawString( 825, 780, "Pause", colors[BLACK]);
		pad.Grid::draw();
		pad.drawBricks();
		pad.draw();
	}
	else if (pad.getGameState()==7) {
		DrawString( 230, 450, "Enter Your Name", colors[MISTY_ROSE]);
		DrawRectangle( 230, 400, 200, 40, colors[WHITE]);
		DrawString( 235, 410, pad.getPlayerName(), colors[BLACK]);
	}
	
	glutSwapBuffers(); // do not modify this line..
}

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		if (pad.getLevel()==3)
			pad.setCoord2(pad.getX2()-10);
	}
	else if (key == GLUT_KEY_RIGHT) {
		if (pad.getLevel()==3)
			pad.setCoord2(pad.getX2()+10);
	}
	else if (key == GLUT_KEY_UP) {}
	else if (key == GLUT_KEY_DOWN) {}

	glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27) { /* Escape key ASCII*/
		// exit(1);
		if (pad.getGameState()==1) pad.setGameState(0);
		else if (pad.getGameState()==2) {
			pad.setGameState(1);
			fstream file("highscore.txt", ios::app);
			file<<pad.getScore()<<","<<pad.getPlayerName()<<",level"<<pad.getCurrentLevel()<<endl;
			file.close();
			pad.setLives(3);
			pad.setScore(0);
			pad.getBall().setCoord(500, 200);
		}
		else if (pad.getGameState()==6) pad.setGameState(2);
		else if (pad.getGameState()==4 || pad.getGameState()==5) pad.setGameState(1);
		else if (pad.getGameState()==7) pad.setGameState(0);
		else if (pad.getGameState()==3) pad.setGameState(0);
	}

	if (pad.getGameState()==0) {
		if (key == 's' || key == 'S') { pad.setGameState(1); }
	}
	else if (pad.getGameState()==6) {
		if (key == 'r' || key == 'R') { pad.setGameState(2); }
	}
	else if (pad.getGameState()==7) {
		if (key==13) {
			pad.setGameState(0);
		}
		pad.setPlayerName(pad.getPlayerName()+(char)key);
	}
	glutPostRedisplay();
}

void Timer(int m) {
	if (pad.getGameState()==6) {
		pad.moveBall();
		glutPostRedisplay();
		if (pad.getLives()==0) {
			pad.setGameState(4);
			pad.setLastScore(pad.getScore());
			fstream file("highscore.txt", ios::app);
			file<<pad.getScore()<<","<<pad.getPlayerName()<<",level"<<pad.getCurrentLevel()<<endl;
			file.close();
			pad.setLives(3);
			pad.setScore(0);
			pad.getBall().setCoord(500, 200);
		}
		else if (pad.checkWin()) {
			pad.setGameState(5);
			pad.setLastScore(pad.getScore());
			fstream file("highscore.txt", ios::app);
			file<<pad.getScore()<<","<<pad.getPlayerName()<<",level"<<pad.getCurrentLevel()<<endl;
			file.close();
			pad.setLives(3);
			pad.setScore(0);
			pad.getBall().setCoord(500, 200);
		}
	}
	glutTimerFunc(1.0, Timer, 0);
}

void MousePressedAndMoved(int x, int y) {
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	if (pad.getGameState()==6) {
		pad.setCoord(x);
	}
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window */
void MouseClicked(int button, int state, int x, int y) { // GLUT_DOWN, GLUT_UP represents the mouse button state
	if (button == GLUT_LEFT_BUTTON) {
        y = 840 - y;
		if (pad.getGameState()==0) {
			if (x>=400 && x<=500 && y>=350 && y<=390)
				pad.setGameState(1);
			else if (x>=230 && x<=430 && y>=485 && y<=525)
				pad.setGameState(7);
			else if (x>=385 && x<=520 && y>=420 && y<=460)
				pad.setGameState(3);
		}
		else if (pad.getGameState()==1) {
			if (x>=290 && x<=315 && y>=360 && y<=385) {
				pad.setCurrentLevel(1);
				pad.setGameState(6);
			}
			else if (x>=390 && x<=415 && y>=360 && y<=385) {
				pad.setCurrentLevel(2);
				pad.setGameState(6);
			}
			else if (x>=490 && x<=515 && y>=360 && y<=385) {
				pad.setCurrentLevel(3);
				pad.setGameState(6);
			}
		}
		else if (pad.getGameState()==6) {
			if (x>=805 && x<=900 && y>=770 && y<=810) {
				pad.setGameState(2);
			}
		}
		else if (pad.getGameState()==2) {
			if (x>=400 && x<=535 && y>=350 && y<=390) {
				pad.setGameState(6);
			}
			else if (x>=350 && x<=590 && y>=300 && y<=340) {
				pad.setGameState(0);
				fstream file("highscore.txt", ios::app);
				file<<pad.getScore()<<","<<pad.getPlayerName()<<",level"<<pad.getCurrentLevel()<<endl;
				file.close();
				pad.setLives(3);
				pad.setScore(0);
				pad.getBall().setCoord(500, 200);
			}
		}
		else if (pad.getGameState()==5) {
			if (x>=230 && x<=470 && y>=350 && y<=390) {
				pad.setGameState(0);
			}
		}
		else if (pad.getGameState()==4) {
			if (x>=230 && x<=470 && y>=330 && y<=370) {
				pad.setGameState(0);
			}
		}
	} 
	else if (button == GLUT_RIGHT_BUTTON) {}
	glutPostRedisplay();
}

int main(int argc, char*argv[]) {
	srand(time(0));
	int width = 1020, height = 840;

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Brick Breaker"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library, you are telling the library names of function to call for different tasks.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutTimerFunc(1.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved);
	glutMotionFunc(MousePressedAndMoved);

	// now handle the control to library and it will call our registered functions when it deems necessary...
	glutMainLoop();
	return 1;
}
#endif