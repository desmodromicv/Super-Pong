#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "paddle.h"
#include <cstdlib>

#define UP rect.move(Vector2f(0,-velocity));
#define DOWN rect.move(Vector2f(0,velocity));
using namespace sf;

SoundBuffer increaseStartB, increaseStopB;
Sound increaseStartS, increaseStopS;

Paddle::Paddle(int X,int Y,int vel)
{
    velocity = vel;

    rect.setFillColor(Color::Green);
    rect.setSize(Vector2f(25,100));
    rect.setPosition(Vector2f(X,Y));

    increaseStartB.loadFromFile("increaseStart.wav");
    increaseStopB.loadFromFile("increaseStop.wav");
    increaseStartS.setBuffer(increaseStartB);
    increaseStopS.setBuffer(increaseStopB);
    increaseStartS.setVolume(35.f);
    increaseStopS.setVolume(45.f);
}

void Paddle::movement(int control)///1-arrows | 2-awsd | 0-AI
{
    ///player control
    if(control == 1)///arrows
    {
        if((Keyboard::isKeyPressed(Keyboard::Up))&&(rect.getPosition().y > 100))UP
        else if((Keyboard::isKeyPressed(Keyboard::Down))&&(rect.getPosition().y + 100 < WINDOW_HEIGTH))DOWN
    }
    else if(control == 2)///awsd
    {
        if((Keyboard::isKeyPressed(Keyboard::W))&&(rect.getPosition().y > 100))UP
        else if((Keyboard::isKeyPressed(Keyboard::S))&&(rect.getPosition().y + 100 < WINDOW_HEIGTH))DOWN
    }
}

int Paddle::AI(int difficultyLevel,int AIballYposition,int AIballXposition,float velocityLeftRight,int ballYposition,int ballXposition,int controlIn1player,
                bool strategyIsSpeedUp, int AIpaddleSpeed, bool thereIsSpeedUp, bool thereIsIncrease, int speedUp, int increase, int velWithoutSpeedUp)
{
    int centerPos{50};
    if(thereIsIncrease)centerPos = increase * 10;

    ///control in 1 player = 1 (arrows)
    if((velocityLeftRight < 0)&&(AIballXposition < 900)&&(ballXposition > 200))
    {
        if(AIballYposition < rect.getPosition().y + centerPos)UP
        if(AIballYposition > rect.getPosition().y + centerPos)DOWN
    }
    else if((velocityLeftRight < 0)&&(ballXposition < 200))
    {
        if(ballYposition < rect.getPosition().y + centerPos)UP
        if(ballYposition > rect.getPosition().y + centerPos)DOWN
    }

    ///control in 1 player = 2 (AWSD)
    else if((velocityLeftRight > 0)&&(AIballXposition > 400)&&(ballXposition < 1100))
    {
        if(AIballYposition < rect.getPosition().y + centerPos)UP
        if(AIballYposition > rect.getPosition().y + centerPos)DOWN
    }
    else if((velocityLeftRight > 0)&&(ballXposition > 1100))
    {
        if(ballYposition < rect.getPosition().y + centerPos)UP
        if(ballYposition > rect.getPosition().y + centerPos)DOWN
    }

    ///speed up & increase
    if(difficultyLevel > 1)
    {
        int distanceBetweenPaddleAndAIBall;
        if(rect.getPosition().y > AIballYposition){
            if(ballXposition < 1100) distanceBetweenPaddleAndAIBall = rect.getPosition().y - AIballYposition - 40;
            else distanceBetweenPaddleAndAIBall = rect.getPosition().y - ballYposition - 40;
        }
        else if(rect.getPosition().y + 100 < AIballYposition){
            if(ballXposition > 200) distanceBetweenPaddleAndAIBall = AIballYposition - rect.getPosition().y - 100;
            else distanceBetweenPaddleAndAIBall = ballYposition - rect.getPosition().y - 100;
        }
        else distanceBetweenPaddleAndAIBall = 0;

        int paddleRunTime;
        int paddleRunTimePlus;
        paddleRunTime = abs(distanceBetweenPaddleAndAIBall / velWithoutSpeedUp);
        if(difficultyLevel == 3) paddleRunTimePlus = distanceBetweenPaddleAndAIBall / velWithoutSpeedUp + speedUp;

        int ballRunTime;
        if(controlIn1player == 1) ballRunTime = abs((ballXposition - 25) / velocityLeftRight);
        else ballRunTime = (1255 - ballXposition) / velocityLeftRight;

        if((((velocityLeftRight < 0)&&(AIballXposition < 20)&&(controlIn1player == 1))||((velocityLeftRight > 0)&&(AIballXposition > 1280)&&(controlIn1player == 2)))&&(paddleRunTime > ballRunTime))
        {
            if(AIballYposition == rect.getPosition().y + 30) return 0;

            if((difficultyLevel == 3)&&(paddleRunTimePlus > ballRunTime)) return 3;

            if(strategyIsSpeedUp)
            {
                if(thereIsSpeedUp) return 1;
                else if(thereIsIncrease) return 2;
                else return 0;
            }
            else
            {
                if(thereIsIncrease) return 2;
                else if(thereIsSpeedUp) return 1;
                else return 0;
            }
        }
        else return 0;
    }
    else return 0;
}

void Paddle::setVelocity(int x)
{
    velocity = x;
}

void Paddle::increase(int padNumber,float increaseSize)
{
    if(!isIncreasing)
    {
        isIncreasing = true;
        int y{rect.getPosition().y};
        if(padNumber == 1)rect.setPosition(Vector2f(10, y - increaseSize * 10));
        if(padNumber == 2)rect.setPosition(Vector2f(1265, y - increaseSize * 10));
        rect.setSize(Vector2f(25,100 + increaseSize * 20));

        increaseStartS.play();
    }
}

void Paddle::increaseStop(int padNumber,float increaseSize)
{
    if(isIncreasing)
    {
        isIncreasing = false;
        int y{rect.getPosition().y};
        if(padNumber == 1)rect.setPosition(Vector2f(10, y + increaseSize * 10));
        if(padNumber == 2)rect.setPosition(Vector2f(1265, y + increaseSize * 10));
        rect.setSize(Vector2f(25,100));

        increaseStopS.play();
    }
}

bool Paddle::getIncrease()
{
    return isIncreasing;
}

void Paddle::setSoundVolume(float volume){
    increaseStartS.setVolume(volume);
    increaseStopS.setVolume(volume);
}
