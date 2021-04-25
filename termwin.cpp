#include <cstdarg>
#include <cstdio>
#include "termwin.h"

#if defined(__WIN32) || defined(WIN32)

#include <windows.h>

Coord getScreenDimen()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return Coord(info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1);
}

#elif defined(__unix__) || defined(unix) || defined(__unix)

#include <unistd.h>
#include <sys/ioctl.h>

Coord getScreenDimen()
{
	struct winsize ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
	return Coord(ws.ws_col, ws.ws_row);
}

#else

Coord getScreenDimen()
{
	fprintf(stderr, "Sorry, Getting screen dimensions is not supported on this system.\n");
	return Coord();
}

#endif

Termwin::Termwin(int width, int height, char bg)
{
	px = 0;
	py = 0;
	cx = 0, cy = 0;
	dimen = Coord(width, height);
	size = width * height;
	for (int i = 0; i < size; i++)
		data += bg;
}

std::string Termwin::splitToString()
{
	std::string ret;
	for (int y = 0; y < dimen.y; y++)
	{
		for (int x = 0; x < dimen.x; x++)
		{
			ret += data[(y * dimen.x) + x];
		}
		ret += '\n';
	}
	return ret;
}

void Termwin::print(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	for (int i = 0; fmt[i] != '\0'; i++)
	{
		switch (fmt[i])
		{
			case 's': 
			{
				char* arg = va_arg(args, char*);
				if (arg == NULL) goto end;
				for (char ch : std::string(arg))
				{
					if (ch == '\n') { cx = px; cy++; }
					else if (ch == '\r') { cx = px; }
					else { data[(cy * dimen.x) + cx] = ch; cx++; if (cx == dimen.x) { cx = px; cy++; } if (cy == dimen.y) cy = 0; }
				}
				break;
			}
			case 'l':
			{
				Coord point = va_arg(args, Coord);
				if (!point.isnempt) goto end;
				cx = point.x;
				cy = point.y;
				px = point.x;
				py = point.y;
				break;
			}
			case 'c':
			{
				char ch = va_arg(args, int);
				if (ch == 0) goto end;
				if (ch == '\n') { cx = px; cy++; }
				else if (ch == '\r') { cx = px; }
				else { data[(cy * dimen.x) + cx++] = ch; } 
				break;
			}
			case 'm':
			{
				Coord coord = va_arg(args, Coord);
				if (!coord.isnempt) goto end;
				cx += coord.x;
				cy += coord.y;
				px = cx, py = cy;
				break;
			}
		}
	}
	end:
	va_end(args);
}

char Termwin::inch(int x, int y)
{
	return data[(dimen.x * y) + x];
}

Coord Termwin::getcoord()
{
	return Coord(cx, cy);
}

void Termwin::draw()
{
	for (int y = 0; y < dimen.y; y++)
	{
		for (int x = 0; x < dimen.x; x++)
		{
			printf("%c", data[(y * dimen.x) + x]);
		}
		printf("\n");
	}
	printf("\033[1;1H");
}

Coord::Coord(int x_, int y_)
{
	x = x_;
	y = y_;
	isnempt = true;
}

Coord::Coord()
{
	x = 0;
	y = 0;
	isnempt = true;
}
