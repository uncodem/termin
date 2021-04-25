#include <iostream>
#include <windows.h>
#include "termwin.h"

int main()
{
	Coord scrSiz = getScreenDimen();
    // operCoord(scrSiz, / 2);
	// printf("%d %d\n", splitCoord(scrSiz));
	printf("\033[2J\033[1;1H"); // Clear the screen
	Termwin tWin(splitCoord(scrSiz), '`');
	Termwin tWin2(8, 8, '#');
	Termwin tWin3(5, 5, '-');
	tWin.print("lsms",Coord(0,0), tWin3.splitToString().c_str(), Coord(6,1), tWin2.splitToString().c_str());
	tWin.draw();
	getchar();
	return 0;
}

