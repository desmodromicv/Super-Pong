#include <SFML/Graphics.hpp>
#include "entity.h"
using namespace sf;

class Paddle : public Entity
{
    int velocity;
    bool isIncreasing{false};
public:
    Paddle(int=10, int=400, int=6);
    void movement(int control);
    bool AI(int difficultyLevel,int ballYposition,int ballXposition,float velocityLeftRight,int controlIn1player);
    void setVelocity(int);
    void increase(int padNumber,float increaseSize);
    void increaseStop(int padNumber,float increaseSize);
    bool getIncrease();
};
