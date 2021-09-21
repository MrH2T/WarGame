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
		void clearUsedTags(){
			for(auto &now:troops){
				now.acted=now.moved=0;
			}
		}
	}
	using Troops::troops;
	
	typedef int TroopId;
	#define MOUNT 0x11
	#define RIVER 0x12
	#define SCAMP 0x13
	#define CAMP_B 0x01
	#define CAMP_W 0x02
	#define TROOP_B 0x01
	#define TROOP_W 0x02
	#define CSIDE_B 0x03
	#define CSIDE_W 0x04
	
	#define BLACK 0
	#define WHITE 1
	
	int mapHeight,mapWidth;
	int map[30][30],tmap[30][30],cmap[30][30];
	int ctmap[30][30];
	int nowTurn;
	COORD bcPos,wcPos;
	int bcHp,wcHp;
	bool onPlacingCamp;
	
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
		return map[x][y]==CAMP_B||map[x][y]==CAMP_W;
	}
	bool isRiverAt(short x,short y){
		return map[x][y]==RIVER;
	}
	bool isBlockedAt(short x,short y){
		return isMountAt(x,y)||isCampAt(x,y)||isRiverAt(x,y)||isScampAt(x,y);
	}
	bool isSideCampAt(short x,short y){
		return map[x][y]==CSIDE_B||map[x][y]==CSIDE_W;
	}
	int getSideCampAt(short x,short y){
		if(!isSideCampAt(x,y))return -1;
		if(map[x][y]==CSIDE_B)return BLACK;
		else return WHITE;
	}
	int getCampAt(short x,short y){
		if(!isCampAt(x,y))return -1;
		if(map[x][y]==CAMP_B)return BLACK;
		else return WHITE;
	}
	TroopId getTroopAt(short x,short y){
		for(int now=0;now<troops.size();now++){
			if(troops[now].x==x&&troops[now].y==y)return now;
		}
		return -1;
	}
	void clearTroopAt(short x,short y){
		troops.erase(troops.begin()+getTroopAt(x,y));
	}
	void printItem(short x,short y){
		if(isTroopAt(x,y)){
			TroopId tpa=getTroopAt(x,y);
			setColor(c_BLACK,ctmap[x][y]?ctmap[x][y]:cmap[x][y]);
			std::cout<<troops[tpa].type.icon[troops[tpa].tm]<<troops[tpa].type.hp;
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
				else if(chr=='W'){
					map[i][j]=CAMP_W;
					cmap[i][j]=c_WHITE;
				}
				else if(chr=='B'){
					map[i][j]=CAMP_B;
					cmap[i][j]=c_DGREY;
				}
				else if(chr=='w'){
					map[i][j]=CSIDE_W;
					cmap[i][j]=c_GREY;
				}
				else if(chr=='b'){
					map[i][j]=CSIDE_B;
					cmap[i][j]=c_GREY;
				}
				else {
					cmap[i][j]=c_GREY;
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
	
	void checkDie(TroopId tar){
		if(troops[tar].type.hp<=0){
			tmap[troops[tar].x][troops[tar].y]=0;
		 	troops.erase(troops.begin()+tar);
			
		}
	}
	
	const int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
	int bookForDfs[30][30];
	void drawMoveDfs(short x,short y,int moved,int movlim,int originTm){
		if(moved>movlim)return;
		if(map[x][y]!=RIVER)ctmap[x][y]=c_LIME;
		if(moved>bookForDfs[x][y])return;
		bookForDfs[x][y]=moved;
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||isTroopAt(dx,dy)||isMountAt(dx,dy)
				||isCampAt(dx,dy)&&getCampAt(dx,dy)!=originTm)continue;
			drawMoveDfs(dx,dy,moved+1,movlim,originTm);
		}
	}
	
	bool atkable;
	int atkOneTurn=0; 
	void drawAttackDfs(short x,short y,int moved,int movlim,int originTm,int met=0){
		if(moved>movlim)return;
		if(isTroopAt(x,y)&&troops[getTroopAt(x,y)].tm!=originTm||isCampAt(x,y)&&getCampAt(x,y)!=originTm)ctmap[x][y]=c_RED,atkable=1;
		if(moved>bookForDfs[x][y])return;
		bookForDfs[x][y]=moved;
		if(met)return;
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||isMountAt(dx,dy))continue;
			if(isTroopAt(dx,dy)&&troops[getTroopAt(dx,dy)].tm!=originTm&&!met){
				drawAttackDfs(dx,dy,moved+1,movlim,originTm,1);
			}
			else drawAttackDfs(dx,dy,moved+1,movlim,originTm,0);
		}
	}
	void checkAttackDfs(short x,short y,int moved,int movlim,int originTm,int met=0){
		if(moved>movlim)return;
		if(isTroopAt(x,y)&&troops[getTroopAt(x,y)].tm!=originTm||isCampAt(x,y)&&getCampAt(x,y)!=originTm)atkable=1;
		if(moved>bookForDfs[x][y])return;
		bookForDfs[x][y]=moved;
		if(met)return;
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||isMountAt(dx,dy))continue;
			if(isTroopAt(dx,dy)&&troops[getTroopAt(dx,dy)].tm!=originTm&&!met){
				checkAttackDfs(dx,dy,moved+1,movlim,originTm,1);
			}
			else checkAttackDfs(dx,dy,moved+1,movlim,originTm,0);
		}
	}
	
	void dfsClear(){memset(ctmap,0,sizeof(ctmap));memset(bookForDfs,0x3f,sizeof(bookForDfs));}
	
	bool enableToMove(TroopId tar){
		return !troops[tar].moved;
	}
	bool enableToAct(TroopId tar){
		if(troops[tar].acted)return false;
		if(map[troops[tar].x][troops[tar].y]==SCAMP)return true;
		memset(bookForDfs,0x3f,sizeof(bookForDfs));
		atkable=0;
		checkAttackDfs(troops[tar].x,troops[tar].y,0,troops[tar].type.sho,troops[tar].tm);
		if(atkable)return true;
		return false;
	}
	
	
	void selectMove(TroopId tar){
		short x=troops[tar].x,y=troops[tar].y;
		dfsClear();
		drawMoveDfs(x,y,0,troops[tar].type.mov,troops[tar].tm);
		drawMap();
		while(1){
			getMouse();
			if(mouseClicked){
				mouseClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(dx==x&&dy==y){
					memset(ctmap,0,sizeof(ctmap));
					drawMap();
					return;
				}
				else{
					if(ctmap[dx][dy]==c_LIME){//enable move to
						std::swap(tmap[dx][dy],tmap[x][y]);
						troops[tar].x=dx,troops[tar].y=dy;
						troops[tar].moved=1;
						memset(bookForDfs,0,sizeof(bookForDfs));
						memset(ctmap,0,sizeof(ctmap));
						drawMap();
				//		goxy(10,50);printf("FCCF");
						return;
					}else continue;
				}
			}
		}
	}
	
	void selectProduce(TroopId tar){
		short x=troops[tar].x,y=troops[tar].y;
		dfsClear();
		for(int p=0;p<4;p++){
			short dx=x+dir[p][0],dy=y+dir[p][1];
			if(dx<0||dy<0||dx>mapHeight||dy>mapWidth||isTroopAt(dx,dy)||isMountAt(dx,dy)||isCampAt(dx,dy)&&getCampAt(dx,dy)!=troops[tar].tm||isRiverAt(dx,dy))continue;
			ctmap[dx][dy]=c_PURPLE;
		}
		drawMap();
		while(1){
			if(!enableToAct(tar))return;
			getMouse();
			if(rightClicked){
				rightClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(x==troops[tar].x&&y==troops[tar].y){
					dfsClear();
					drawMap();
					return;
				}
			}
			if(mouseClicked){
				mouseClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(ctmap[dx][dy]==c_PURPLE){
					troops[tar].acted=1;
					Troop nt(troops[tar].type,troops[tar].tm,dx,dy);
					nt.type.hp=troops[tar].type.hp;
					nt.moved=nt.acted=1;
					troops.push_back(nt);
					tmap[dx][dy]=tmap[x][y];
					dfsClear();
					drawMap();
					return;
				}
			}
		}
	}
	void selectAttack(TroopId tar){
		short x=troops[tar].x,y=troops[tar].y;
		dfsClear();
		drawAttackDfs(troops[tar].x,troops[tar].y,0,troops[tar].type.sho,troops[tar].tm);
		drawMap();
		while(1){
			if(!enableToAct(tar))return;
			getMouse();
			if(rightClicked){
				rightClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(dx==troops[tar].x&&dy==troops[tar].y){
					dfsClear();
					drawMap();
					return;
					
				}
			}
			if(mouseClicked){
				mouseClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short dx=blockPos.Y,dy=blockPos.X;
				if(ctmap[dx][dy]==c_RED){
					if(isCampAt(dx,dy)){
						if(getCampAt(dx,dy)==0)bcHp-=troops[tar].type.atk;
						else wcHp-=troops[tar].type.atk;
						atkOneTurn+=troops[tar].type.atk;
						troops[tar].acted=1;
						dfsClear();
						drawMap();
						
						return;
					}
					
					TroopId target=getTroopAt(dx,dy);
					troops[tar].acted=1;
					troops[target].type.hp-=troops[tar].type.atk;
					checkDie(target);
					dfsClear();
					drawMap();
					return;
				}
			}
		}
	}
	
	bool won(){
		if(atkOneTurn>=3){
			if(nowTurn==1){
				SetConsoleTitle((string("Unnamed Game - WHITE Wins")).c_str());
				while(1);
				return true;
			}else{
				SetConsoleTitle((string("Unnamed Game - BLACK Wins")).c_str());
				while(1);
				return true;
			}
		}
		if(wcHp<=0){
			SetConsoleTitle((string("Unnamed Game - BLACK Wins")).c_str());
			while(1);
			return true;
		}
		else if(bcHp<=0){
			SetConsoleTitle((string("Unnamed Game - WHITE Wins")).c_str());
			while(1);
			return true;
		}
		else return false;
	}
	bool nowMovedAll(){
		for(int t=0;t<troops.size();t++){
			if(troops[t].tm==nowTurn&&(enableToAct(t)||enableToMove(t)))return false;
		}
		return true;
	}
	
	void turn(){
		dfsClear();
		Troops::clearUsedTags();
		atkOneTurn=0;
		drawMap();
		while(1) {
			
			if(won())exit(0);
			
			SetConsoleTitle((string("Unnamed Game - ")+(nowTurn?"WHITE":"BLACK")+"\'s Turn : "+ char(wcHp+'0') + " w : b "+ char(bcHp+'0')).c_str());
			Sleep(50);
			mouseClicked=rightClicked=spacePressed=0;
			drawMap();
			WIN_CONTROL::MOUSE::getMouse();
			if(onEndTurn()){
				return;
			}
			if(mouseClicked) {
				mouseClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(!isTroopAt(x,y))continue;
				TroopId tar=getTroopAt(x,y);
				if(enableToMove(tar)&&troops[tar].tm==nowTurn)selectMove(tar);
				if(nowMovedAll())return;
			}
			if(rightClicked){
				rightClicked=false;
				if(!inBlock(lastClickedPos))continue;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(!isTroopAt(x,y))continue;
				TroopId tar=getTroopAt(x,y);
//				goxy(10,50);
//				printf("%d",troops[tar].acted);
				if(enableToAct(tar)&&troops[tar].tm==nowTurn)
					if(!isScampAt(x,y))selectAttack(tar);
					else selectProduce(tar);
				if(nowMovedAll())return;
			}
		}
		
	}
	void gameRun(){
		WIN_CONTROL::hideCursor();
		Troops::clearUsedTags();
		
		turn();
		nowTurn^=1;
		Sleep(500);
		spacePressed=0;
	}
	
	
	
	void placeTroopAt(TroopType type,short x,short y,int tm){
		Troop nt(type,tm,x,y);
		Troops::troops.push_back(nt);
		tmap[x][y]=tm?TROOP_W:TROOP_B;
		
	}
	void printCertainItem(short x,short y,int tm,TroopType tp){
		goxy(2*x+1,4*y+1);
		std::cout<<tp.icon[tm]<<tp.hp;
	}
	using namespace Troops;
	int mctSize=6,sctSize=2;
	TroopType mainCampType[100];
	TroopType sideCampType[100];
	void initCampTypeImages(){
		//use after Troops::troopImageInit()
		mainCampType[0]=SHIELD;
		mainCampType[1]=HORSE;
		mainCampType[2]=BOW;
		mainCampType[3]=CROSSBOW;
		mainCampType[4]=LANCE;
		mainCampType[5]=SWORD;
		
		sideCampType[0]=SHIELD;
		sideCampType[1]=CROSSBOW;
	}
	
	void choosingTroop(short x,short y,int tm,TroopType types[],int tplen){
		int nowChoose=0;
		TroopType nowType=types[nowChoose];
		while(1){
			getMouse();
			printCertainItem(x,y,tm,nowType);
			if(mouseClicked){
				mouseClicked=false;
				placeTroopAt(nowType,x,y,tm);
				drawMap();
				return;
			}
			if(rightClicked){
				rightClicked=false;
				nowChoose=(nowChoose+1)%tplen;
				nowType=types[nowChoose];
		//		goxy(10,50);printf("%d  ",nowChoose);
				drawMap();
			}
		}
	}
	void placeTroop(int tm){
		int troopPlaced=0;
		SetConsoleTitle((string("Unnamed Game - ")+(tm?"WHITE":"BLACK")+" Place Troops").c_str());
		
		dfsClear();
		for(short x=0;x<mapHeight;x++){
			for(short y=0;y<mapWidth;y++){
				if(getSideCampAt(x,y)==tm)ctmap[x][y]=c_LYELLOW;
			}
		}
		drawMap();
		while(1){
			getMouse();
			if(onEndTurn()){
				return;
			}
			if(mouseClicked){
				mouseClicked=false;
				COORD blockPos=clickWhichBlock(lastClickedPos);
				short x=blockPos.Y,y=blockPos.X;
				if(!isCampAt(x,y)&&!isSideCampAt(x,y)
					||isCampAt(x,y)&&getCampAt(x,y)!=tm
					||isSideCampAt(x,y)&&getSideCampAt(x,y)!=tm)continue;
				
				if(troopPlaced==4&&!isTroopAt(x,y))continue;
				if(isCampAt(x,y)&&getCampAt(x,y)==tm){
					if(!isTroopAt(x,y))troopPlaced++;
					else tmap[x][y]=0,clearTroopAt(x,y);
					choosingTroop(x,y,tm,mainCampType,mctSize);
				}
				else if(isSideCampAt(x,y)&&getSideCampAt(x,y)==tm){
					if(!isTroopAt(x,y))troopPlaced++;
					else tmap[x][y]=0,clearTroopAt(x,y);
					choosingTroop(x,y,tm,sideCampType,sctSize);
				}
				
			}
		}
	}
	
	
	void gameInit(){
		bcHp=wcHp=6;
		bcPos=COORD{-10,-10};
		wcPos=COORD{-10,-10};
		drawMap();
//Choose Camp: Abandoned
//		chooseCamp(0);
//		chooseCamp(1);
		placeTroop(0);
		placeTroop(1);
		nowTurn=0;
		spacePressed=0;
	}
}

using namespace GAME;

int main(){
//	SetConsoleOutputCP(65001);
	WIN_CONTROL::CONSOLE_INIT();
	
	GAME::Troops::troopImageInit();
	GAME::initCampTypeImages();
	readMapFromFile();
	gameInit();
	
	GAME_FLAG = 1;
	SetConsoleTitle("Unnamed Game");
	while(GAME_FLAG){
		gameRun();
	}
}

#endif
