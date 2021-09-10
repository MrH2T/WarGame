#ifndef PREINFO_CPP

#define PREINFO_CPP

#include<iostream>
#include<algorithm>
#include<windows.h>

using std::string;

namespace CHAR_SET{
	
using namespace std;
	string tab[]={"\u2500","\u2502","\u250c","\u2510",
	    "\u2514","\u2518","\u251C","\u2524","\u252c",
		"\u2534","\u253c"};
	/*
tabs:
0-LR 1-UD 2-DR 3-DL 4-UR 5-UL 6-UDR 7-UDL 8-DLR 9-ULR 10-UDLR
*/
	string items[]={"\u25a0","\u25a1","\u25b2","\u25b3",
		"\u25b6","\u25b7","\u25c6","\u25c7","\u25cb","\u25cf",
		"\u2605","\u2606"};
/*
items:
0,1-FESquare
2,3-FETriangle
4,5-FEHTriangle
6,7-FEDiamond
8,9-FECircle
10,11-FEStar
*/
}

namespace BASIC_DATA{
	class Troop{
		private:
			string icon[2];
		public:
			int hp,atk,mov,sho,tm;
			int x,y;
			//mov=move sho=shoot tm=team
			Troop(int h,int a,int m,int s,int xx,int yy,int t):
				hp(h),atk(a),mov(m),sho(s),x(xx),y(yy),tm(t){}
	};
}

namespace WIN_CONTROL{
	#define c_BLACK 0x0
	#define c_DARKBLUE 0x1
	#define c_DBLUE 0x1
	#define c_DARKGREEN 0x2
	#define c_DGREEN 0x2
	#define c_SKYBLUE 0x3
	#define c_DARKRED 0x4
	#define c_DRED 0x4
	#define c_DARKPURPLE 0x5
	#define c_DPURPLE 0x5
	#define c_YELLOW 0x6
	#define c_GREY 0x7
	#define c_DARKGREY 0x8
	#define c_DGREY 0x8
	#define c_LIGHTBLUE 0x9
	#define c_LBLUE 0x9
	#define c_LIGHTGREEN 0xa
	#define c_LGREEN 0xa
	#define c_LIME 0xb
	#define c_RED 0xc
	#define c_PURPLE 0xd
	#define c_LIGHTYELLOW 0xe
	#define c_LYELLOW 0xe
	#define c_WHITE 0xf
	
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	
	void hideCursor(){
		GetConsoleCursorInfo(hOut,&cci);
		cci.bVisible=0;
		SetConsoleCursorInfo(hOut,&cci);
	}
	void setConsoleSize(){
		system("mode con lines=40 cols=80");
	}
	
	
	void setColor(short fCol,short bCol){
		SetConsoleTextAttribute(hOut,fCol|(bCol<<4));
	}
	
	void goxy(short x,short y){
		COORD cor={y,x};
		SetConsoleCursorPosition(hOut,cor);
	}
	
	void cls(){
		for(short i=0;i<80;i++){
			goxy(i,0);
			std::cout<<"\
			                                        \
													";
		}
	}
	
	void CONSOLE_INIT(){
		SetConsoleCP(65001);
		SetConsoleTitle("Unnamed Game");
		
		setConsoleSize();
		hideCursor();
		
	}
}




#endif
