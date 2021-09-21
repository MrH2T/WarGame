#ifndef PREINFO_HPP

#define PREINFO_HPP

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
		"\u25b6","\u25b7","\u25c6","\u25c7","\u25cf","\u25cb",
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
	class TroopType{
		public:
			string icon[2];
			int hp,atk,mov,sho;
			void setDefaultIcon(string s0,string s1){
				icon[0]=s0,icon[1]=s1;
			}
			//mov=move sho=shoot tm=team
			TroopType(){}
			TroopType(int h,int a,int m,int s):
				hp(h),atk(a),mov(m),sho(s){}
			bool operator==(const TroopType bbb)const{
				return hp==bbb.hp&&atk==bbb.atk&&mov==bbb.mov&&sho==bbb.sho;
			}
	};
	
	class Troop{
		public:
			TroopType type;
			int tm,x,y;
			int moved,acted;
			Troop(){}
			Troop(TroopType tp,int t,int xx,int yy): type(tp),tm(t),x(xx),y(yy){
			}
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
	
	HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	
	void goxy(short x,short y){
		COORD cor={y,x};
		SetConsoleCursorPosition(hOut,cor);
	}
	
	#define isKeydown()
	namespace MOUSE{
		CONSOLE_SCREEN_BUFFER_INFO binfo;
		INPUT_RECORD ms_rec;
		DWORD ms_res;
		COORD cr_pos,cr_home={0,0};
		
		COORD lastClickedPos,mouseNowPos;
		bool mouseClicked,rightClicked;
		
		bool spacePressed;
		
		void getMouse(){
			ReadConsoleInput(hIn,&ms_rec,1,&ms_res);
			if(ms_rec.EventType==MOUSE_EVENT){
				cr_pos=ms_rec.Event.MouseEvent.dwMousePosition;
//				GetConsoleScreenBufferInfo(hOut,&binfo);
//				goxy(20,40);
//				printf("%d %d          \n",cr_pos.X,cr_pos.Y);
				SetConsoleCursorPosition(hOut,binfo.dwCursorPosition);
				mouseNowPos=cr_pos;
//				goxy(5,50);
//				printf("%d %d %d",ms_rec.Event.MouseEvent.dwButtonState,FROM_LEFT_1ST_BUTTON_PRESSED,RIGHTMOST_BUTTON_PRESSED);
				if(ms_rec.Event.MouseEvent.dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED){
			//		if(mouseClicked)return;
					mouseClicked=true;rightClicked=false;
					lastClickedPos=cr_pos;
				}
				if(ms_rec.Event.MouseEvent.dwButtonState==RIGHTMOST_BUTTON_PRESSED){
					rightClicked=true;mouseClicked=false;
//					goxy(10,50);
//					printf("FCCF");
					lastClickedPos=cr_pos;
				}
			}
			else if(ms_rec.EventType==KEY_EVENT){
				if(ms_rec.Event.KeyEvent.wVirtualKeyCode==VK_SPACE
				&&ms_rec.Event.KeyEvent.bKeyDown){
					spacePressed=true;
				}
			}
		}
	}
	
	
	void hideCursor(){
		GetConsoleCursorInfo(hOut,&cci);
		cci.bVisible=0;
		SetConsoleCursorInfo(hOut,&cci);
	}
	
	void setColor(short fCol,short bCol){
		SetConsoleTextAttribute(hOut,fCol|(bCol<<4));
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
		DWORD consoleMode;
		GetConsoleMode(hIn,&consoleMode);
		SetConsoleMode(hIn,consoleMode|ENABLE_MOUSE_INPUT);
		
		SetConsoleOutputCP(65001);
		SetConsoleTitle("War Game");
		
		hideCursor();
		
	}
}


#endif
