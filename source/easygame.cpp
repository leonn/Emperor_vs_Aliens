#include "easygame.h"

void EasyGame::init(GameConfig& config){
	EasyGame::init(config);
    timeToLive = timeForNextWave;
}


void EasyGame::gameBehaviour(){
  //  if(Timer::get_currentFrameTick() >= timeToLive)
    //    eva.killSwarm();
}

void EasyGame::callNextWave(){
	Game::callNextWave();
    timeToLive = timeForNextWave; 
}