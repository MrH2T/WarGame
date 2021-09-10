#ifndef UG_CPP
#define UG_CPP

#include"preinfo.cpp"

#define UNICODE

using namespace WIN_CONTROL;

void run_per_tick(){
	hideCursor();
	setConsoleSize();
	cls();
}


bool GAME_FLAG;

int main(){
	WIN_CONTROL::CONSOLE_INIT();
	
	GAME_FLAG = 1;
	
	
	while(GAME_FLAG){
		run_per_tick();
		Sleep(200);
		goxy(20,40);std::cout<<("FUCK YOU");
		Sleep(200);
	}
}

#endif
