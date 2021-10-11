#include "particle.h"
#include <iostream>
#include <vector>
#include <math.h>

Particle::Particle() {
    position = Point2D();
    colour = Colour();
    size = 0;
    direction = Vec2D();
    range = 0;
    speed = 0;
}

Particle::Particle(Point2D mPosition, Colour mColour, int mSize, Vec2D mDirection, int mRange, float mSpeed) {
    position = mPosition;
    colour = mColour;
    size = mSize;
    direction = mDirection;
    range = mRange;
    speed = mSpeed;
    mass = mSize;
    acceleration = Vec2D(0,0);
    mouseForce = Vec2D(0,0);
}

Vec2D Particle::sumForces() {
    Vec2D friction = direction.multiply(-0.08);
    Vec2D allForces = mouseForce.add(friction);
    return allForces;
}

void Particle::update(float x, float y) {
    Vec2D allForce = sumForces();      
    acceleration = allForce.multiply(1.0/mass);    
    mouseForce = Vec2D(0,0);    
    Vec2D velocity = direction.multiply(speed).add(acceleration);
    direction = velocity.normalize();
    speed = velocity.length();
    if(speed <= 0.05) {
        speed = 0;
    }
    position = velocity.movePoint(position);  
    float halfSize = size/2.0;
    if((position.mX - halfSize) < 0) {
        direction.mX = -direction.mX;
        position.mX = halfSize;
    }
    else if((position.mX + halfSize) > x) {
        direction.mX = -direction.mX;
        position.mX = x - halfSize;
    }
    else if((position.mY - halfSize) < 0 ) {
        direction.mY = -direction.mY;
        position.mY = halfSize;
    }
    else if((position.mY + halfSize) > y) {
        direction.mY = -direction.mY;
        position.mY = y - halfSize;
    }
}

Vec2D Particle::forceToMoveToPoint(Point2D source) {
    float s = sqrt((2.0 * 0.08 * position.distanceTo(source)) / mass);
    return Vec2D::createVector(position, source).normalize().multiply(s);

}