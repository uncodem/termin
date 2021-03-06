#ifndef TERMWIN_H_
#define TERMWIN_H_

#include <string>
#include <vector>

#define splitCoord(c) c.x, c.y
#define operCoord(c, oper) c.x = c.x oper; c.y = c.y oper 

class Coord
{
public:
	int x, y;
	Coord(int x_, int y_);
	Coord();
	bool isnempt;
};

class Termwin
{
	std::string data;
	std::vector<std::string> topper;
	int cx, cy;
	int px, py;
    Coord dimen;
	int size;
public:
	Termwin(int width, int height, char bg);
	void print(const char *fmt, ...);
	char inch(int x, int y); 
	Coord getcoord();
	void draw();
	std::string splitToString();
};

Coord getScreenDimen();

#endif
