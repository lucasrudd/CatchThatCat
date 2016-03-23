#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <stdlib.h>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE 32
#define OBJECT_SIZEY 35

//add mine that follows you, and when it gets you it blows up
//make a screen that says "Man, you dun' fucked up."
//add music that laughs and mocks you for dying

int gameover;
int speed = 15;
SDL_Rect rcSrc, rcSprite;
int speedUp = 0;

void HandleEvent(SDL_Event event)
{
    switch (event.type) {
        case SDL_QUIT:
          gameover = 1;
          break;

        /* handle the keyboard */
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {

            case SDLK_ESCAPE:
            case SDLK_q:
            case SDLK_SPACE:
                gameover = 1;
                break;

            case SDLK_p:
                if(Mix_PausedMusic() == 1)
                {
                    Mix_ResumeMusic();
                }
                else if(Mix_PlayingMusic() == 1)
                {
                    Mix_PauseMusic();
                }
                break;

            /*case SDLK_LSHIFT:
            case SDLK_RSHIFT:
                if(speed == 10-speedUp)
                {
                    speed = 5+speedUp;
                }
                else
                {
                    speed = 10+speedUp;
                }
                break;*/

            case SDLK_LEFT:
                if(rcSrc.x == 192)
                {

                    rcSrc.x = 224;
                }
                else
                {

                    rcSrc.x = 192;
                }
                rcSprite.x -= speed;
                break;

            case SDLK_RIGHT:
                if(rcSrc.x == 64)
                {
                    rcSrc.x = 96;
                }
                else
                {
                    rcSrc.x = 64;
                }
                rcSprite.x += speed;
                break;

            case SDLK_UP:
                if(rcSrc.x == 0)
                {
                    rcSrc.x = 32;
                }
                else
                {
                    rcSrc.x = 0;
                }
                rcSprite.y -= speed;
                break;

            case SDLK_DOWN:
                if(rcSrc.x == 128)
                {
                    rcSrc.x = 160;
                }
                else
                {
                    rcSrc.x = 128;
                }
                rcSprite.y += speed;
                break;
          }
          break;
    }
}


bool collision(SDL_Rect main, SDL_Rect other)
{
    int mainLeft, mainRight, mainTop, mainBottom, otherLeft, otherRight, otherTop, otherBottom;

    mainLeft = main.x;
    mainRight = main.x + SPRITE_SIZE;
    mainTop = main.y;
    mainBottom = main.y + SPRITE_SIZE;

    otherLeft = other.x;
    otherRight = other.x + SPRITE_SIZE;
    otherTop = other.y;
    otherBottom = other.y + SPRITE_SIZE;

    if(!((mainBottom <= otherTop) || (mainTop >= otherBottom) || (mainRight <= otherLeft) || (mainLeft >= otherRight)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main ( int argc, char *argv[] )
{
    SDL_Surface *screen, *temp, *sprite, *object, /**mine,*/ *gameoverScreen, *highScoreScreen, /*highScoreScreenName,*/ *fence, *fenceVerticle, *grass;
    SDL_Surface *message[10], *nameScoreScreen;
    SDL_Rect rcGrass, rcObject, rcFence, rcMessage, rcHighScore, rcMine, /*rcHighScoreName,*/ /*rcNameScoreScreen,*/ rcGameOver;
    TTF_Font *font, *fontLarge;
    Mix_Music *music;
    Mix_Chunk *pickUp;
    Mix_Chunk *applause;
    Mix_Chunk *winSFX;
    SDL_Color textColor = {255, 255, 255};
    int colorkey, score;
    int scoreArray[10];
    double HighScoreDouble;
    int songFrequency = 22050;
    score = 0;
    srand (std::time(NULL));
    int xSpeed, ySpeed;
    xSpeed = ySpeed =  1;

    SDL_Init(SDL_INIT_EVERYTHING);

    if(TTF_Init() == -1)
    {
        return 1;
    }

    if(Mix_OpenAudio(songFrequency, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return 2;
    }

    std::ifstream HighScoreFile ("highscore.txt");
    std::string line, HighScoreName, HighScore;

    if(HighScoreFile.is_open())
    {
        //std::getline(HighScoreFile, HighScoreName, ':');
        std::getline(HighScoreFile, HighScore);
        HighScoreFile.close();
    }


    SDL_WM_SetCaption("Catch The Cat 11/19/2014", "Lucas Rudd");


    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0 | SDL_FULLSCREEN);
    SDL_EnableKeyRepeat(70, 70);

    font = TTF_OpenFont("font.ttf", 28);
    fontLarge = TTF_OpenFont("font.ttf", 35);
    pickUp = Mix_LoadWAV("SFX_Pickup_32.wav");
    applause = Mix_LoadWAV("Applause.wav");
    winSFX = Mix_LoadWAV("Round End.wav");
    music = Mix_LoadMUS("Cat in Space.wav");
    Mix_PlayMusic(music, -1);

    if(font == NULL)
    {
        return 3;
    }

    if(music == NULL)
    {
        return 4;
    }

    if(pickUp == NULL)
    {
        return 5;
    }

    std::stringstream tempStream;
    std::stringstream s[10];
    std::stringstream gameoverText;
    std::stringstream HighScoreText, HighScoreNameText, nameInputText;



    for(int i = 0; i < 10; i++)
    {
        scoreArray[i] = i;
        s[i] << "Catch That Cat!     Score: " << scoreArray[i];
        message[i] = TTF_RenderText_Solid(font, s[i].str().c_str(), textColor);
    }




    temp = SDL_LoadBMP("spriteAnimated.bmp");
    sprite = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    temp = SDL_LoadBMP("cheshire-cat.bmp");
    object = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    temp = SDL_LoadBMP("fence.bmp");
    fence = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    temp = SDL_LoadBMP("fenceVerticle.bmp");
    fenceVerticle = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    temp = SDL_LoadBMP("grass.bmp");
    grass = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /*temp = SDL_LoadBMP("mine.bmp");
    mine = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);*/

    HighScoreDouble = atof(HighScore.c_str());
    HighScoreText << "The HighScore is: " << HighScoreDouble << " seconds.";

    clock_t start;
    double duration;
    start = std::clock();

    colorkey = SDL_MapRGB(screen->format, 255, 0, 255);

    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    SDL_SetColorKey(object, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    SDL_SetColorKey(fence, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    //SDL_SetColorKey(mine, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    SDL_SetColorKey(fenceVerticle, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


    rcSprite.x = 150;
    rcSprite.y = 150;

    rcMine.x = SPRITE_SIZE/2;
    rcMine.y = SPRITE_SIZE/2;

    rcGameOver.x = SPRITE_SIZE;
    rcGameOver.y = SCREEN_HEIGHT/2;
    rcHighScore.x = SCREEN_WIDTH/2;
    rcHighScore.y = 0;
    //rcHighScoreName.x = rcGameOver.x + SPRITE_SIZE;
    //rcHighScoreName.y = rcHighScore.y + SPRITE_SIZE;
    //rcNameScoreScreen.x = rcGameOver.x;
    //rcNameScoreScreen.y = rcHighScore.y + SPRITE_SIZE;

    rcObject.x = rand() % (SCREEN_WIDTH-(2*SPRITE_SIZE)) + SPRITE_SIZE;;
    rcObject.y = rand() % (SCREEN_HEIGHT-(2*OBJECT_SIZEY)) + OBJECT_SIZEY;

    rcSrc.x = 128;
    rcSrc.y = 0;
    rcSrc.w = SPRITE_SIZE;
    rcSrc.h = SPRITE_SIZE;

    rcMessage.x = 0;
    rcMessage.y = 0;

    gameover = 0;
    int stepNum = 0;
    double Vel, xVel, yVel;
    Vel = 10;
    highScoreScreen = TTF_RenderText_Solid(font, HighScoreText.str().c_str(), textColor);

  /* message pump */
  while (!gameover)
  {
    /* look for an event */
    SDL_Event event;

    if (SDL_PollEvent(&event))
    {
        HandleEvent(event);
    }

    /*if(rcSprite.x > rcMine.x)
    {
        xVel = ((rcMine.x - rcSprite.x)/(rcMine.y-rcSprite.y));
        if((rcSprite.y - rcMine.y) == 0)
        {
            xVel = rcMine.x - rcSprite.x;
        }
    }
    else if(rcSprite.x < rcMine.x)
    {
        xVel = -((rcMine.x - rcSprite.x)/(rcMine.y-rcSprite.y));
        if((rcSprite.y - rcMine.y) == 0)
        {
            xVel = -(rcMine.x - rcSprite.x);
        }

    }


    if(rcSprite.y > rcMine.y)
    {
        yVel = ((rcMine.y-rcSprite.y)/(rcMine.x-rcSprite.x ));
        if((rcSprite.x - rcMine.x) == 0)
        {
            yVel = (rcMine.y - rcSprite.y);
        }
    }
    else if(rcSprite.y < rcMine.y)
    {
        yVel = ((rcMine.y-rcSprite.y)/(rcMine.x-rcSprite.x ));
        if((rcSprite.x - rcMine.x) == 0)
        {
            yVel = -(rcMine.y - rcSprite.y);
        }
    }*/


    if(xVel >= 2)
    {
        xVel = 2;
    }
    else if(xVel <= -2)
    {
        xVel = -2;
    }
    else if(yVel >= 2)
    {
        yVel = 2;
    }
    else if(yVel <= -2)
    {
        yVel = -2;
    }

    /*rcMine.x += xVel;
    rcMine.y += yVel;*/
    rcObject.x += xSpeed;
    rcObject.y += ySpeed;
    if((rcObject.x < SPRITE_SIZE) || (rcObject.x > SCREEN_WIDTH-2*(SPRITE_SIZE)))
    {
        xSpeed = -xSpeed;
    }

    if((rcObject.y < SPRITE_SIZE) || (rcObject.y > SCREEN_HEIGHT-2*(SPRITE_SIZE)))
    {
        ySpeed = -ySpeed;
    }

    if(rcSprite.x < SPRITE_SIZE)
    {
        rcSprite.x = SPRITE_SIZE;
    }
    else if(rcSprite.x > SCREEN_WIDTH-2*(SPRITE_SIZE))
    {
        rcSprite.x = SCREEN_WIDTH-2*(SPRITE_SIZE);
    }
    if(rcSprite.y < SPRITE_SIZE)
    {
        rcSprite.y = SPRITE_SIZE;
    }
    else if(rcSprite.y > SCREEN_HEIGHT-2*(SPRITE_SIZE))
    {
        rcSprite.y = SCREEN_HEIGHT-2*(SPRITE_SIZE);
    }

    if(collision(rcSprite, rcObject) && (score != 10))
    {
        if(Mix_PlayChannel(-1, pickUp, 0) == -1)
        {
            return 6;
        }

        do
        {
            rcObject.x = rand() % (SCREEN_WIDTH-(2*SPRITE_SIZE)) + SPRITE_SIZE;
            rcObject.y = rand() % (SCREEN_HEIGHT-(2*SPRITE_SIZE)) + SPRITE_SIZE;
        }while(!((rcObject.x != rcSprite.x) && (rcObject.y != rcSprite.y)));

        score++;
        speed++;
        speedUp++;
    }

    if(score == 10)
    {
        if(Mix_PlayChannel(-1, winSFX, 0) == -1)
        {
            return 7;
        }
        object = NULL;
        sprite = NULL;
        //mine = NULL;
        highScoreScreen = NULL;
        rcObject.x = 0;
        rcObject.y = 0;
        duration = (std::clock() - start)/((double)CLOCKS_PER_SEC);
        gameoverText << "CONGRADULATIONS! YOU WASTED " << duration << " SECONDS OF YOUR LIFE :D";

        if(HighScoreDouble < duration)
        {
            //HighScoreNameText << HighScoreName;
            rcHighScore.x = rcGameOver.x;
            rcHighScore.y = rcGameOver.y + SPRITE_SIZE;
            highScoreScreen = TTF_RenderText_Solid(fontLarge, HighScoreText.str().c_str(), textColor);
            //highScoreScreenName = TTF_RenderText_Solid(fontLarge, HighScoreNameText.str().c_str(), textColor);
        }

        else if(HighScoreDouble >= duration)
        {
            if(Mix_PlayChannel(-1, applause, 0) == -1)
            {
                return 8;
            }
            rcHighScore.x = rcGameOver.x;
            rcHighScore.y = rcGameOver.y + SPRITE_SIZE;
            HighScoreText.str("");
            HighScoreText.clear();
            HighScoreText << "BUT, HOLY SHIT! YOU SET THE HIGH SCORE!!!!";
            //nameInputText << "TYPE IN YOUR NAME TO BE IMMORTALIZED...";
            highScoreScreen = TTF_RenderText_Solid(fontLarge, HighScoreText.str().c_str(), textColor);
            //nameScoreScreen = TTF_RenderText_Solid(fontLarge, nameInputText.str().c_str(), textColor);
            tempStream << duration;
            HighScore = tempStream.str();
            std::ofstream newHighScore;
            newHighScore.open("highscore.txt");
            if(newHighScore.is_open())
            {
                newHighScore << HighScore;
                newHighScore.close();
            }
            else
            {
                return 55;
            }
        }
        gameoverScreen = TTF_RenderText_Solid(font, gameoverText.str().c_str(), textColor);
        score++;
    }


    for(int x = 0; x < SCREEN_WIDTH/SPRITE_SIZE; x++)
    {
        for(int y = 0; y < SCREEN_HEIGHT/SPRITE_SIZE; y++)
        {
            rcGrass.x = x*SPRITE_SIZE;
            rcGrass.y = y*SPRITE_SIZE;
            SDL_BlitSurface(grass, NULL, screen, &rcGrass);
            if((y ==0) || (y == (SCREEN_HEIGHT/SPRITE_SIZE)-1))
            {
                rcFence.x = x*SPRITE_SIZE;
                rcFence.y = y*SPRITE_SIZE;
                SDL_BlitSurface(fence, NULL, screen, &rcFence);
            }

            if((x==0) || (x == (SCREEN_WIDTH/SPRITE_SIZE)-1))
            {
                if((y!= 0) && (y != (SCREEN_HEIGHT/SPRITE_SIZE)-1))
                {
                    rcFence.x = x*SPRITE_SIZE;
                    rcFence.y = y*SPRITE_SIZE;
                    SDL_BlitSurface(fenceVerticle, NULL, screen, &rcFence);
                }
            }
        }
    }

    if(score < 10)
    {
        SDL_BlitSurface(message[score], NULL, screen, &rcMessage);
    }

    SDL_BlitSurface(object, NULL, screen, &rcObject);
    SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);
    //SDL_BlitSurface(mine, NULL, screen, &rcMine);
    SDL_BlitSurface(gameoverScreen, NULL, screen, &rcGameOver);
    SDL_BlitSurface(highScoreScreen, NULL, screen, &rcHighScore);
    //SDL_BlitSurface(highScoreScreenName, NULL, screen, &rcHighScoreName);
    //SDL_BlitSurface(nameScoreScreen, NULL, screen, &rcNameScoreScreen);
    /* update the screen */
    stepNum++;
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

  /* free the background surface */
  TTF_CloseFont(font);
  TTF_CloseFont(fontLarge);
  Mix_FreeChunk(pickUp);
  Mix_FreeChunk(applause);
  Mix_FreeMusic(music);
  SDL_FreeSurface(highScoreScreen);
  //SDL_FreeSurface(highScoreScreenName);
  //SDL_FreeSurface(nameScoreScreen);
  SDL_FreeSurface(gameoverScreen);
  SDL_FreeSurface(*message);
  SDL_FreeSurface(fence);
  SDL_FreeSurface(fenceVerticle);
  SDL_FreeSurface(object);
  //SDL_FreeSurface(mine);
  SDL_FreeSurface(sprite);
  SDL_FreeSurface(grass);

  /* cleanup SDL */
  Mix_CloseAudio();
  TTF_Quit();
  SDL_Quit();

  return 0;
}


