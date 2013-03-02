#include <iostream>

#include "game.h"
#include "windowfactory.h"
#include "point.h"

using namespace std;

namespace edge
{

    Game::Game()
    {
        video = NULL;
        window = NULL;
    }

    void
    Game::init(const string& configFilePath) throw (Exception)
    {
        initVideo(configFilePath);
        initWindow(configFilePath);
        eva.init();
        
        WAVES_LEFT = eva.board.getWavesLeft();
        PAUSED=(false);
		FAST_FORWARD=(false);
		QUIT=(false);
		
		allFrameCount = -1;
        Timer::start();
    }

    void
    Game::loop() throw (Exception)
    {
        // 1. Atualizar time step
		allFrameCount++;

        // 2. Inputs
        SDL_Event event;
        bool quitGame = false;

        while (quitGame == false) {

            int tickCountBegin = Timer::get_ticks();

            while (SDL_PollEvent(&event)) {

                switch (event.type) {
                case SDL_QUIT:
                    quitGame = true;
                    break;		
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quitGame = true;
                        break;
					case SDLK_SPACE:
						togglePause();
						break;	
					case SDLK_f:
						toggleFastForward();
						break;	
					case SDLK_x:
						callNextWave();
						break;
                    default:
                        //eva.board.handleKeyboardEvent(&event.key);
                    	break;
                    }
                default:
                    break;
                }
            }
            // 3. Rodar IA
			eva.IA();
            // 4. Rodar simulações de física
            // 5. Atualizar entidades do jogo
			eva.update();

            if(Timer::get_ticks() > RESOURCES_COOLDOWN){
                eva.increaseResources();
                RESOURCES_COOLDOWN = Timer::get_ticks() + THREE_SECONDS;
            }
            // 6. Enviar/receber mensagens da rede
            // 7. Atualizar o estado do jogo (display)
            eva.draw(window->getCanvas());
            
            window->getCanvas()->update();

            int ticksElapsed = Timer::get_ticks() - tickCountBegin;
            int delayTime = ( 1000 / FRAMES_PER_SECOND ) - ticksElapsed;
            SDL_Delay( delayTime > 0 ? delayTime : 0);
        }


    }

    void
    Game::shutdown()
    {
        if (window) {
            delete window;
        }

        if (video && video->isUp()) {
            video->shutdown();
            delete video;
        }

        eva.shutdown();
    }

    void
    Game::initVideo(const string&) throw (Exception)
    {
        VideoSettings videoSettings;
        videoSettings.videoBufferLocation = VIDEO_MEMORY;
        videoSettings.machineArchitecture = MULTI_CORE;

        video = Video::getInstance();
        video->init();
        video->changeSettings(videoSettings);
    }

    void
    Game::initWindow(const string&) throw (Exception)
    {
        WindowSettings windowSettings;
        windowSettings.width = 1920;
        windowSettings.height= 1080;
        windowSettings.title = "EmperorVsAliens";
        windowSettings.presentationMode = WINDOW;
        
        window = WindowFactory::create(windowSettings);
    }
    
    void Game::togglePause()
	{
		PAUSED = !PAUSED;
		printf("Pressed: PAUSE!\n");
	}

	void Game::toggleFastForward()
	{
		FAST_FORWARD = !FAST_FORWARD;
		printf("Pressed: FAST_FORWARD!\n");
	}

	void Game::callNextWave()
	{
		if (WAVES_LEFT > 0) 
		{
			WAVES_LEFT--;
			eva.callNextWave();
		}
		printf("Pressed: CALL_NEXT_WAVE! %d waves left!\n",WAVES_LEFT);
		printf("Swarn Units alive: %d\n",(int)eva.swarmUnits.size());
		
	}
}