#ifndef UG_CPP
#define UG_CPP

#include"preinfo.hpp"

#define UNICODE

using namespace WIN_CONTROL;
using WIN_CONTROL::MOUSE::lastPos;
using WIN_CONTROL::MOUSE::mouseClicked;

namespace GAME{
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
		void placeTroop(int x,int y){
			
		}
	}
	
	
	bool GAME_FLAG;
	void gameRun(){
		WIN_CONTROL::MOUSE::getMouse();
		if(mouseClicked){
			mouseClicked=false;
			goxy(20,10);
			printf("%d %d      ",lastPos.Y,lastPos.X);
		}
	}
}

using namespace GAME;

int main(){
	WIN_CONTROL::CONSOLE_INIT();
	
	GAME_FLAG = 1;
	
	while(GAME_FLAG){
		gameRun();
	}
}

#endif
