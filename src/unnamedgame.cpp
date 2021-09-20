#ifndef UG_CPP
#define UG_CPP

#include"preinfo.hpp"
#include<vector>

#define UNICODE

using namespace WIN_CONTROL;
using namespace WIN_CONTROL::MOUSE;

namespace GAME{
	using namespace CHAR_SET;
	using BASIC_DATA::Troop;
	using BASIC_DATA::TroopType;
	namespace Troops{
		TroopType SHIELD(3,2,1,1),
			HORSE(1,1,4,1),
			BOW(1,1,2,3),
			CROSSBOW(1,1,1,4),
			LANCE(2,1,3,1),
			SWORD(1,2,3,1);
		std::vector<Troop> troops;
		void troopImageInit(){
			SHIELD.setDefaultIcon(items[8],items[9]);
			HORSE.setDefaultIcon(items[10],items[11]);
			BOW.setDefaultIcon(items[2],items[3]);
			CROSSBOW.setDefaultIcon(items[4],items[5]);
			LANCE.setDefaultIcon(items[0],items[1]);
			SWORD.setDefaultIcon(items[6],items[7]);
		}
		void placeTroop(TroopType tp,int t,int x,int y){
			troops.push_back(Troop(tp,t,x,y));
		}
		void clearUsedTags(){
			for(auto now:troops){
				now.acted=now.moved=now.mvRiver=0;
			}
		}
	}
	
	#define MOUNT 0x11
	#define RIVER 0x12
	#define SCAMP 0x13
	#define CAMP_B 0x01
	#define CAMP_W 0x02
	#define TROOP_B 0x01
	#define TROOP_W 0x02
	
	#define BLACK 0
	#define WHITE 1
	
	int mapHeight,mapWidth;
	int map[30][30],tmap[30][30],cmap[30][30];
	int ctmap[30][30];
	int nowTurn;
	COORD bcPos,wcPos;
	int bcHp,wcHp;
	
	bool GAME_FLAG;
	bool inBlock(COORD pos){
		swap(pos.X,pos.Y);
		if(pos.X%2==0||pos.Y%4==0||pos.Y>mapWidth*4+1||pos.X>mapHeight*2+1)return false;
		return true;
	}
	COORD clickWhichBlock(COORD pos){
		if(!inBlock(pos))return {-1,-1};
		return COORD{(SHORT)pos.X/4,(SHORT)pos.Y/2};
	}
	bool isTroopAt(short x,short y){
		return tmap[x][y];
	}
	bool isScampAt(short x,short y){
		return map[x][y]==SCAMP;
	}
	bool isMountAt(short x,short y){
		return map[x][y]==MOUNT;
	}
	bool isCampAt(short x,short y){
		return (x-bcPos.X==1||x-bcPos.X==0&&y-bcPos.Y==1||y-bcPos.Y==0)||
			   (x-wcPos.X==1||x-wcPos.X==0&&y-wcPos.Y==1||y-wcPos.Y==0);
	}
	bool isRiverAt(short x,short y){
		return map[x][y]==RIVER;
	}
	int getCampAt(short x,short y){
		if((x-bcPos.X==1||x-bcPos.X==0&&y-bcPos.Y==1||y-bcPos.Y==0))return BLACK;
		if((x-wcPos.X==1||x-wcPos.X==0&&y-wcPos.Y==1||y-wcPos.Y==0))return WHITE;
		else return -1;
	}
	Troop& getTroopAt(short x,short y){
		Troop tn=Troop();
		if(!isTroopAt(x,y))return tn;
		for(auto &now:Troops::troops){
			if(now.x==x&&now.y==y)return now;
		}
		return tn;
	}
	void printItem(short x,short y){
		if(isTroopAt(x,y)){
			Troop tpa=getTroopAt(x,y);
			setColor(tpa.tm==0?c_BLACK:c_WHITE,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
			std::cout<<tpa.type.icon[tpa.tm]<<tpa.type.hp;
			setColor(c_DARKGREY,c_GREY);
		}
		else{
			if(map[x][y]==MOUNT){
				setColor(c_BLACK,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"MMM";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==RIVER){
				setColor(c_SKYBLUE,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==SCAMP){
				setColor(c_YELLOW,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==CAMP_B){
				setColor(c_BLACK,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==CAMP_W){
				setColor(c_WHITE,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else{
				setColor(c_GREY,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
		}
	}
	void readMapFromFile(){
	//	FILE *fout=fopen("fccf","w");
	//	fprintf(fout," ");
		FILE *fin=fopen("map.txt","r");
		fscanf(fin,"%d %d\n",&mapHeight,&mapWidth);
		for(short i=0;i<mapHeight;i++){
			for(short j=0;j<mapWidth;j++){
				char chr;
				fscanf(fin,"%c",&chr);
				if(chr=='M'){
					cmap[i][j]=c_GREY;
					map[i][j]=MOUNT;
				}
				else if(chr=='R'){
					cmap[i][j]=c_SKYBLUE;
					map[i][j]=RIVER;
				}
				else if(chr=='.'){
					cmap[i][j]=c_GREY;
				}
				else if(chr=='C'){
					cmap[i][j]=c_YELLOW;
					map[i][j]=SCAMP;
				}
			}
			fscanf(fin,"\n");
		}
	}
	void drawMap(){
		setColor(c_DGREY,c_GREY);
		goxy(0,0);
		std::cout<<tab[2];
		for(short y=0;y<mapWidth-1;y++){
			std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[8];
		}
		std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[3];
		for(short x=0;x<mapHeight-1;x++){
			goxy(x*2+1,0);
			std::cout<<tab[1];
			for(short y=0;y<mapWidth;y++){
				
				printItem(x,y);
				std::cout<<tab[1];
			}
			goxy(x*2+2,0);
			std::cout<<tab[6];
			for(short y=0;y<mapWidth-1;y++)
				std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[10];
			std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[7];
		}
		goxy(mapHeight*2-1,0);
		std::cout<<tab[1];
		for(short y=0;y<mapWidth;y++){
			
			printItem(mapHeight-1,y);
			std::cout<<tab[1];
		}
		goxy(mapHeight*2,0);
		std::cout<<tab[4];
		for(short y=0;y<mapWidth-1;y++)
			std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[9];
		std::cout<<tab[0]<<tab[0]<<tab[0]<<tab[5];
	}
	
	bool onEndTurn(){
		if(spacePressed){spacePressed=false;return true;}
		else return false;
	}
	
	vector<COORD> stayWaterPos;
	void warnStayRiver(){
		memset(ctmap,0,sizeof(ctmap));
		for(auto pos:stayWaterPos){
			ctmap[pos.X][pos.Y]=c_RED;
		}
		drawMap();
		Sleep(500);
		memset(ctmap,0,sizeof(ctmap));
		drawMap();
		stayWaterPos.clear();
	}
	void checkDie(Troop& tar){
		if(tar.type.hp<=0){
			for(int i=0;i<Troops::troops.size();i++){
				if(Troops::troops[i].x==tar.x&&Troops::troops[i].y==tar.y){
					Troops::troops.erase(Troops::troops.begin()+i);
					break;
				}
			}
			tmap[tar.x][tar.y]=0;
			
		}
	}
	
	const int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
	bool bookForDfs[30][30];
	void drawMoveDfs(short x,short y,int moved,int movlim,int originTm){
		if(moved>movlim)return;
		ctmap[x][y]=c_LIME;
		bookForDfs[x][y]=1;
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||bookForDfs[dx][dy]||isTroopAt(dx,dy)||isMountAt(dx,dy)
				||isCampAt(dx,dy)&&getCampAt(dx,dy)!=originTm)continue;
			drawMoveDfs(dx,dy,moved+1,movlim,originTm);
		}
	}
	void drawAttackDfs(short x,short y,int moved,int movlim,int originTm,int met=0){
		if(moved>movlim)return;
		if(isTroopAt(x,y)&&getTroopAt(x,y).tm!=originTm||isCampAt(x,y)&&getCampAt(x,y)!=originTm)ctmap[x][y]=c_RED;
		bookForDfs[x][y]=1;
		if(met)return;
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||bookForDfs[dx][dy]||isMountAt(dx,dy))continue;
			if(isTroopAt(dx,dy)&&getTroopAt(dx,dy).tm!=originTm&&!met){
				drawAttackDfs(dx,dy,moved+1,movlim,originTm,1);
			}
			else drawAttackDfs(dx,dy,moved+1,movlim,originTm,0);
		}
	}
	
	void dfsClear(){memset(ctmap,0,sizeof(ctmap));memset(bookForDfs,0,sizeof(bookForDfs));}
	
	void selectMove(Troop& tar){
		short x=tar.x,y=tar.y;
		memset(ctmap,0,sizeof(ctmap));
		memset(bookForDfs,0,sizeof(bookForDfs));
		drawMoveDfs(x,y,0,tar.type.mov,tar.tm);
		drawMap();
		while(1){
			getMouse();
			if(mouseClicked){
				COORD blockPos=clickWhichBlock(lastClickedPos);
				int dx=blockPos.Y,dy=blockPos.X;
				if(dx==dy){
					memset(ctmap,0,sizeof(ctmap));
					drawMap();
					return;
				}
				else{
					if(bookForDfs[dx][dy]){//enable move to
						std::swap(tmap[dx][dy],tmap[x][y]);
						Troop& zzzlalala=getTroopAt(x,y);
						zzzlalala.x=dx,zzzlalala.y=dy;
						memset(bookForDfs,0,sizeof(bookForDfs));
						memset(ctmap,0,sizeof(ctmap));
						drawMap();
						return;
					}else continue;
				}
			}
		}
	}
	
	void selectProduce(Troop& tar){
		short x=tar.x,y=tar.y;
		dfsClear();
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||isTroopAt(dx,dy)||isMountAt(dx,dy)||isCampAt(dx,dy)&&getCampAt(dx,dy)!=tar.tm||isRiverAt(dx,dy))continue;
			ctmap[dx][dy]=c_PURPLE;
		}
		drawMap();
		while(1){
			getMouse();
			if(rightClicked){
				rightClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(x==tar.x&&y==tar.y){
					dfsClear();
					return;
				}
			}
			if(mouseClicked){
				mouseClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(ctmap[dx][dy]==c_PURPLE){
					Troop nt;
					nt.type.hp=tar.type.hp;
					nt.type.sho=tar.type.sho;
					nt.type.atk=tar.type.atk;
					nt.type.mov=tar.type.mov;
					nt.tm=tar.tm;
					nt.x=dx,nt.y=dy;
					nt.acted=nt.moved=1;
					nt.mvRiver=0;
					tmap[dx][dy]=tmap[x][y];
					Troops::troops.push_back(nt);
					dfsClear();
					return;
				}
			}
		}
	}
	void selectAttack(Troop& tar){
		short x=tar.x,y=tar.y;
		dfsClear();
		drawAttackDfs(tar.x,tar.y,0,tar.type.sho,tar.tm);
		drawMap();
		while(1){
			getMouse();
			if(rightClicked){
				rightClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(dx==tar.x&&dy==tar.y){
					if(isScampAt(dx,dy)){
						return selectProduce(tar);
					}
					else {
						dfsClear();
						drawMap();
						return;
					}
				}
			}
			if(mouseClicked){
				mouseClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(ctmap[dx][dy]==c_RED){
					if(isCampAt(dx,dy)){
						if(getCampAt(dx,dy)==0)bcHp-=tar.type.atk;
						else wcHp-=tar.type.atk;
						dfsClear();
						drawMap();
						return;
					}
					
					Troop target=getTroopAt(dx,dy);
					target.type.hp-=tar.type.atk;
					checkDie(target);
					tar.acted=1;
					dfsClear();
					drawMap();
					return;
				}
			}
		}
	}
	
	void turn(){
		dfsClear();
		drawMap();
		while(1) {
			WIN_CONTROL::MOUSE::getMouse();
			if(onEndTurn()){
				bool stayRiver=0;
				for(auto now:Troops::troops){
					if(now.tm==nowTurn){
						if(map[now.x][now.y]==RIVER&&!now.mvRiver){
							stayWaterPos.push_back(COORD{now.x,now.y});
							stayRiver=1;
						}
					}
				}
				if(stayRiver){
					warnStayRiver();
				}else return;
			}
			if(mouseClicked) {
				mouseClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(!isTroopAt(x,y))continue;
				Troop &tar=getTroopAt(x,y);
				if(!tar.moved)selectMove(tar);
				
			}
			if(rightClicked){
				rightClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(!isTroopAt(x,y))continue;
				Troop &tar=getTroopAt(x,y);
				if(!tar.acted)selectAttack(tar);
			}
		}
		nowTurn^=1;
	}
	void gameRun(){
		WIN_CONTROL::hideCursor();
		Troops::clearUsedTags();
		turn();
	}
	
	
	
	
	void chooseCamp(int tm){
		while(1){
			
		}
	}
	
	void gameInit(){
		bcHp=wcHp=6;
		
		chooseCamp(0);
	}
}

using namespace GAME;

int main(){
	WIN_CONTROL::CONSOLE_INIT();
//	SetConsoleOutputCP(65001);
	GAME::Troops::troopImageInit();
	
	GAME_FLAG = 1;
//	mapHeight=10;
//	mapWidth=10;
	readMapFromFile();
	while(GAME_FLAG){
		gameRun();
	}
}

#endif
