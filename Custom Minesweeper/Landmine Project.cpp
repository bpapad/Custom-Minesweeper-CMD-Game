#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <cstdlib>
//#pragma once
using namespace std;


//Player Class
class Player{
	private:
		int x,y;
	public:
		void setXY(int a, int b){
			setX(a);
			setY(b);
		}
		void setX(int a){
			x=a;
		}
		void setY(int b){
			y=b;
		}
		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
		
		//Constructors
		Player(){
			setXY(1,1);
		}
		Player(int a){
			setXY(a,a);
		}
		Player(int a, int b){
			setXY(a,b);
		}
};

//Global Variables
bool boom;
int width = 15;
int height = 9;
int score=0, stage=1, fruitpicked=0;
enum eDirection {STOP = 0,LEFT, RIGHT, UP, DOWN};
eDirection dir = STOP;
Player P(1,height/2);
Player Fruit(rand()%(width-2) +1 , rand()%(height-2)+1);
Player Fruit2(rand()%(width-2) +1 , rand()%(height-2)+1);


//Mine Generator
vector<vector<int> > minefield(){
	srand(time(NULL));
	int temp=0;
	int check=0;
 	vector<vector<int> > mines;
 	
 	//Filling vector with mines at random places (excluding the wall spots)
 	for(int i=0; i<height; i++){
 		vector<int> temp;
 		for(int j=0; j<width; j++){
 			if((i==0) || (j==0) || (i==height-1) || j==width-1)
 				temp.push_back(0);
 			else
 				temp.push_back(((rand()%5)==0)?1:0);
		}
		mines.push_back(temp);
	}
	
	//Clearing specific spots on the field from mines to avoid dead ends
	for(int i=0; i<mines.size(); i++){
		for(int j=0; j<mines[i].size(); j++){
			if(mines[i].at(j) == 1)
				check++;
		}		
		if((check >= mines.size()-2) || (check == mines.size()-4)){
			do{
			temp = (rand()%(mines[i].size()-2) + 1);
			}while(mines[i].at(temp) != 1);
			mines[i].at(temp) = 0;
		
		check=0;
		}
	}	
	mines[height/2].at(1)=0;
	mines[height/2].at(width-1)=0;
	mines[height/2].at(width-2)=0;	
	mines[Fruit.getY()].at(Fruit.getX())=0;
	mines[Fruit2.getY()].at(Fruit2.getX())=0;
	 
	return mines;
}

//Initializing Minefield
vector<vector<int> > mines = minefield();

//Minecounter
int minecounter(vector<vector<int> > m){
	int counter;
	for(int i=0; i<m.size(); i++){
		for(int j=0; j<m[i].size(); j++){
			if(m[i].at(j) == 1)
				counter++;
		}
	}
	return counter;
}


//Endscreen
void endscreen(vector<vector<int> > mines){	
	cout<<"Mine Positions: "<<endl;
	for(int i=0; i<mines.size(); i++){
		for(int j=0; j<mines[i].size(); j++){			
			if((i==0) || (j==0) || (i==height-1) || ((j==width-1) && (i!=height/2)))
				cout<<'#';
			else if(mines[i].at(j) == 1)
				cout<<'*';
			else
				cout<<' ';		
		}
		cout<<endl;
	}
}


//Mine Radar
void radar(){
	int counter = 0;
	if(mines[P.getY() + 1].at(P.getX()) == 1)
		counter++;
	if(mines[P.getY() - 1].at(P.getX()) == 1)
		counter++;
	if(mines[P.getY()].at(P.getX() + 1) == 1)
		counter++;
	if(mines[P.getY()].at(P.getX() - 1) == 1)
		counter++;
	
	cout<<counter<<" Mines around you!"<<endl;
}

//Cheat
void cheat(char d){
	if(d == 'y'){
		if(mines[P.getY() + 1].at(P.getX()) == 1){
			cout<<"1 on bottom"<<endl;
		}
			
		if(mines[P.getY() - 1].at(P.getX()) == 1){
			cout<<"1 on top"<<endl;
		}
		if(mines[P.getY()].at(P.getX() + 1) == 1){
			cout<<"1 on the right"<<endl;
		}
		if(mines[P.getY()].at(P.getX() - 1) == 1){
			cout<<"1 on the left"<<endl;
		}
		
		
		if(mines[P.getY()].at(P.getX()) == 1){
			cout<<"STEPPING ON A MINE!"<<endl;
		}
	}
	else
	cout<<endl;
	
}

//PLATFORM GENERATOR
void platform(char d){
	system("cls");
	cout<<"Make your way through the opening while avoiding all the mines! Collect the fruit for extra points!"<<endl;
	cout<<'\n'<<"Stage: "<<stage<<endl;
	cout<<"Score: "<<score<<'\n'<<endl;
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){			
				if((i==0) || (j==0) || ((j==width-1) && (i!=height/2)) || (i==height-1))
					cout<<"#";
				else if((i == Fruit.getY() && j == Fruit.getX()) || (i == Fruit2.getY() && j == Fruit2.getX()))
					cout<<"F";
				else if(i == P.getY() && j == P.getX())
					cout<<"P";
				else
					cout<<" ";
		
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<minecounter(mines)<<" Total Buried Mines"<<endl;
	radar();
	cheat(d);
}







//Game Logic
void move(char d){
	srand(time(NULL));
	switch(dir){
		case LEFT:
			P.setX(P.getX()-1);
			break;
		case RIGHT:
			P.setX(P.getX()+1);
			break;
		case UP:
			P.setY(P.getY()-1);
			break;
		case DOWN:
			P.setY(P.getY()+1);
			break;
		default:
			break;
	}
	dir=STOP;
	
	//Wall Collision
	if(P.getY() <= 0)
		P.setY(P.getY()+1);
	else if(P.getX() <= 0)
		P.setX(P.getX()+1);
	else if(P.getY() >= height-1)
		P.setY(P.getY()-1);
	else if(P.getX() >= width-1 && P.getY()!=height/2)
		P.setX(P.getX()-1);
	
	//Fruit Points
	if((P.getX() == Fruit.getX() && P.getY() == Fruit.getY())){
		score+=10;
		fruitpicked++;
		Fruit.setXY(0,0);
	}
	if((P.getX() == Fruit2.getX() && P.getY() == Fruit2.getY())){
		score+=10;
		fruitpicked++;
		Fruit2.setXY(0,0);
	}
	
	
	//Stage Progression
	if(P.getX() >= width-1 && P.getY() == height/2){
		score+=20;
		++stage;
		++width;
		++height;
		P.setXY(1,height/2);
		Fruit.setXY(rand()%(width-2) +1 , rand()%(height-2)+1);
		Fruit2.setXY(rand()%(width-2) +1 , rand()%(height-2)+1);
		if((Fruit.getX() == 1) && (Fruit.getY() == height/2)){
			Fruit.setXY(Fruit.getX()+1, Fruit.getY()+1);
		}
		if(((Fruit2.getX() == Fruit.getX()) && (Fruit2.getY() == Fruit.getY())) || ((Fruit2.getX() == 1) && (Fruit2.getY() == height/2))){
			Fruit2.setXY(Fruit.getX()+1, Fruit.getY()+1);
		}
		mines = minefield();
		platform(d);
	}
}

//Player Movement
void logic(char d){
do{
	if(_kbhit()){
		switch(_getch()){
			case 75:
				dir = LEFT;
				break;
			case 77:
				dir = RIGHT;
				break;
			case 72:
				dir = UP;
				break;
			case 80:
				dir = DOWN;
				break;
			case 'a':
				dir = LEFT;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 'w':
				dir = UP;
				break;
			case 's':
				dir = DOWN;
				break;
			case 'x':
				boom = true;
				break;		
		}
		move(d);
		platform(d);
	}
	}while(mines[P.getY()].at(P.getX()) != 1);
	boom=true;
}




//MAIN
int main(){
	vector<string> names;
	string name;
	vector<int> scores, fruits, stages;
	vector<char> difficulties;
	char nextplayer;
	int pcount=0;

	vector<string>::iterator strit;
	
	do{
		++pcount;
		cout<<"Player "<<pcount<<", enter your name : ";
		cin>>name;
		names.push_back(name);
		
			char dif;
			cout<<"ENABLE MINE DETECTOR? ('y' or 'n'): ";
			cin>>dif;
			platform(dif);
			while(!boom){
				logic(dif);		
			}
			system("cls");
			cout<<'\n'<<"BOOM!  GAME OVER!"<<endl;
			cout<<endl;
			endscreen(mines);
			cout<<'\n'<<"Stage: "<<stage<<"  Fruit collected: "<<fruitpicked<<"  Total Score: "<<score;
			if(dif == 'y')
				cout<<"     Mine Detector: ON  ..shame on you!"<<endl;
			else
				cout<<"     Mine Detector: OFF  ..at least you tried!"<<endl;
			
			stages.push_back(stage);
			scores.push_back(score);
			fruits.push_back(fruitpicked);
			difficulties.push_back(dif);
			boom=false;
			P.setXY(1,height/2);
			
			cout<<'\n'<<"Continue for Player "<<pcount+1<<"? ('y' or 'n') "<<endl;
			cin>>nextplayer;
			system("cls");
			
	}while(nextplayer == 'y');
	
	int k=0;
	string winner;
	for(strit = names.begin(); strit < names.end(); strit++){
		k++;
	}
	
	for(int l=0; l<k; l++){
		cout<<names.at(l)<<":  "<<"  Stage: "<<stages.at(l)<<"  Radar: "<<difficulties.at(l)<<"  Fruit collected: "<<fruits.at(l)<<" Score: "<<scores.at(l)<<endl;
	}
	
	
	
	
	return 0;
}
