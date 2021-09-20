#ifndef UG_CPP
#define UG_CPP

#include"preinfo.hpp"
#include<vector>

#define UNICODE

using namespace WIN_CONTROL;
using WIN_CONTROL::MOUSE::lastClickedPos;
using WIN_CONTROL::MOUSE::mouseClicked;

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
		void clearUsedTag(){
			for(auto now:troops){
				now.used=0;
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
	
	int mapHeight,mapWidth;
	int map[30][30],tmap[30][30],cmap[30][30];
	int nowSide;
	
	bool GAME_FLAG;
	bool inBlock(COORD pos){
		swap(pos.X,pos.Y);
		if(pos.X%2==0||pos.Y%4==0||pos.Y>mapWidth*4+1||pos.X>mapHeight*2+1)return false;
		return true;
	}
	bool isTroopAt(short x,short y){
		return tmap[x][y];
	}
	Troop getTroopAt(short x,short y){
		if(!isTroopAt(x,y))return Troop();
		for(auto now:Troops::troops){
			if(now.x==x&&now.y==y)return now;
		}
	}
	void printItem(short x,short y){
		if(isTroopAt(x,y)){
			Troop tpa=getTroopAt(x,y);
			setColor(tpa.tm==0?c_BLACK:c_WHITE,cmap[x][y]);
			std::cout<<tpa.type.icon[tpa.tm]<<tpa.type.hp;
			setColor(c_DARKGREY,c_GREY);
		}
		else{
			if(map[x][y]==MOUNT){
				setColor(c_BLACK,cmap[x][y]);
				std::cout<<"MMM";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==RIVER){
				setColor(c_SKYBLUE,cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==SCAMP){
				setColor(c_YELLOW,cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==CAMP_B){
				setColor(c_BLACK,cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else if(map[x][y]==CAMP_W){
				setColor(c_WHITE,cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
			else{
				setColor(c_GREY,cmap[x][y]);
				std::cout<<"   ";
				setColor(c_DARKGREY,c_GREY);
			}
		}
	}
	void readMapFromFile(){
		FILE *fout=fopen("fccf","w");
		fprintf(fout," ");
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
	void gameRun(){
		WIN_CONTROL::hideCursor();
		WIN_CONTROL::MOUSE::getMouse();
		if(mouseClicked){
			mouseClicked=false;
			goxy(20,60);printf("%d %d      ",lastClickedPos.X,lastClickedPos.Y);
			if(inBlock(lastClickedPos)){
				goxy(22,60);printf("SBABS");
			}else goxy(22,60),printf("     ");
		}
		drawMap();
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
