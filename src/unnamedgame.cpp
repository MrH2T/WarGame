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
	namespace Troops{
		using BASIC_DATA::Troop;
		using BASIC_DATA::TroopType;
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
	
	int map[30][30],mapHeight,mapWidth;
	
	int nowSide;
	
	bool GAME_FLAG;
	bool inBlock(COORD pos){
		swap(pos.X,pos.Y);
		if(pos.X%2==0||pos.Y%4==0||pos.Y>mapWidth*4+1||pos.X>mapHeight*2+1)return false;
		return true;
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
				std::cout<<"   ";
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
			std::cout<<Troops::CROSSBOW.icon[0]<<"5";
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
	mapHeight=10;
	mapWidth=10;
	while(GAME_FLAG){
		gameRun();
	}
}

#endif
