#include "mathLib2D.h"
#include <cmath>
#include <iostream>

Point2D::Point2D() {
    mX = 0.0;
    mY = 0.0;
}

Point2D::Point2D(float x, float y) {
    mX = x;
    mY = y;
}

float Point2D::distanceTo(Point2D otherPoint) {
    float distance, xComponent, yComponent;

    xComponent = pow(otherPoint.mX - mX, 2);
    yComponent = pow(otherPoint.mY - mY, 2);

    distance = sqrt(xComponent + yComponent);

    return distance;

}

float Point2D::fastDistanceTo(Point2D otherPoint) {
    float fastDistance, xComponent, yComponent;

    xComponent = pow(otherPoint.mX - mX, 2);
    yComponent = pow(otherPoint.mY - mY, 2);

    fastDistance = xComponent + yComponent;

    return fastDistance;
}

Vec2D::Vec2D() {
    mX = 0.0;
    mY = 0.0;
}

Vec2D::Vec2D(float x, float y) {
    mX = x;
    mY = y;
}

float Vec2D::length() {
    float length, xSquared, ySquared;

    xSquared = pow(mX, 2);
    ySquared = pow(mY, 2);

    length = sqrt(xSquared + ySquared);

    return length;
}

Vec2D Vec2D::normalize() {
    float xNormalized, yNormalized;

    if(length() == 0) {
        return Vec2D(0,0);
    }
    xNormalized = mX / length();
    yNormalized = mY / length();

    return Vec2D(xNormalized, yNormalized);
}

Vec2D Vec2D::multiply(float scalar) {
    return Vec2D (scalar * mX, scalar * mY);
}

Vec2D Vec2D::createVector(Point2D point1, Point2D point2) {
    float xComponent, yComponent;

    xComponent = point2.mX - point1.mX;
    yComponent = point2.mY - point1.mY;

    return Vec2D(xComponent, yComponent);
}

Point2D Vec2D::movePoint(Point2D source) {
    return Point2D(mX + source.mX, mY + source.mY);
}

Vec2D Vec2D::add(Vec2D source) {
    return Vec2D(source.mX + mX, source.mY + mY);
}

