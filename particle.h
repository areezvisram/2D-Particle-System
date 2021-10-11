#ifndef PARTICLE_H
#define PARTICLE_H

#include "mathLib2D.h"
#include "colour.h"
#include <vector>

class Particle {
public:
    Particle();
    Particle(Point2D position, Colour colour, int size, Vec2D direction, int range, float speed);
    
    Point2D position;
    Colour colour;
    int size;
    int mass;
    Vec2D direction;
    int range;
    float speed;
    Vec2D acceleration;
    Vec2D mouseForce;    
    
    void update(float x, float y);
    Vec2D sumForces();

    Vec2D forceToMoveToPoint(Point2D source);
};

#endif