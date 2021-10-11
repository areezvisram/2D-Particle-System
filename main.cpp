#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <random>
#include "particle.h"
#include "mathLib2D.h"
#include "colour.h"

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;

int GLOBAL_RANGE = 0;
int GLOBAL_SPEED = 5;
int MOUSE_X;
int MOUSE_Y;

bool IS_LEFT_PRESSED = false;
bool IS_RIGHT_PRESSED = false;
bool DISPLAY_RANGE_RADIUS = false;
bool DISPLAY_GLOBAL_RANGE_RADIUS = false;
bool ANIMATE = true;
bool SHOW_DIRECTION_VECTORS = false;

std::vector<Particle> PARTICLES(200);

// Function to draw a circle. Used for range displays and arranging particles in a circle
void drawCircle(float x, float y, float range, Colour circleColour) {    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLE_FAN);

    float degree = 360.0/64;
    float angle;
    for(int i = 0; i < 64; i++) {
        angle = i * degree * 3.14151629 / 180;    
        glColor4f(circleColour.r, circleColour.g, circleColour.b, 0.3);    
        glVertex2f(x + range * cos(angle), y + range * sin(angle));        
    }
        
    glEnd();    
}

// Function to generate random values in a given range
float generateRandomValue(int lowerBound, int upperBound, bool isFloat)
{
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_real_distribution<> distribution(lowerBound, upperBound);

    if(isFloat) {
        float randomNumber = std::round(10. * distribution(gen)) / 10;
        return randomNumber;
    } else {
        int randomNumber = std::round(10. * distribution(gen)) / 10;
        return randomNumber;
    }     
}

// Function to generate a particle with random stats
Particle generateParticle() {
    float x = generateRandomValue(0, GLOBAL_WIDTH - 5, true);
    float y = generateRandomValue(0, GLOBAL_HEIGHT - 5, true);
    float r = generateRandomValue(0, 1, true);
    float g = generateRandomValue(0, 1, true);
    float b = generateRandomValue(0, 1, true);
    int size = generateRandomValue(5, 10, false);
    float range = generateRandomValue(40, 60, false);
    float speed = generateRandomValue(1, 5, false);

    return Particle(Point2D(x, y), Colour(r, g, b), size, Vec2D(0,0), range, speed);   
}

// Overloaded function to generate a particle with random stats except for (x,y) values. Used when adding particles at mouse cursor
Particle generateParticle(float x, float y) {
    float r = generateRandomValue(0, 1, true);
    float g = generateRandomValue(0, 1, true);
    float b = generateRandomValue(0, 1, true);
    int size = generateRandomValue(5, 10, false);
    float range = generateRandomValue(40, 60, false);
    float speed = generateRandomValue(1, 5, false);

    return Particle(Point2D(x, y), Colour(r, g, b), size, Vec2D(0,0), range, speed);   
}

// Randomly generate the first 200 particles
void generateInitialParticles() {
    for(int i = 0; i < PARTICLES.size(); i++) {
        PARTICLES[i] = generateParticle();
    }
}

// Arrange the particles in a circle when 'b' is pressed
void arrangeInCircle(int xCenter, int yCenter, int radius) {    
	GLfloat twicePi = 2.0f * 3.14159265358979323846;
	
	glBegin(GL_LINE_LOOP);
    for(int i = 0; i < PARTICLES.size();i++) {
        Point2D source (xCenter + (radius * cos(i *  twicePi / 200)), yCenter + (radius * sin(i *  twicePi / 200)));
        PARTICLES.at(i).mouseForce = Vec2D(0,0);
        PARTICLES.at(i).direction = Vec2D(0,0);
        PARTICLES.at(i).speed = 0;
        Vec2D v = PARTICLES.at(i).forceToMoveToPoint(source);
        PARTICLES.at(i).direction = v.normalize();
        PARTICLES.at(i).speed = v.length();    
    }
	glEnd();    
    glFlush();
}

// Arrange the particles in a rose curve when z is pressed
void arrangeInFlower(int xCenter, int yCenter, int radius) {    
	GLfloat twicePi = 2.0f * 3.14159265358979323846 / 200;
    for(float theta = 0; theta < PARTICLES.size(); theta+= 1) {
        Point2D source (xCenter + 100*(2+(sin(6*theta)) * 2 * cos(twicePi * theta)), yCenter + 100*(2+(sin(6*theta)) * 2 * sin(twicePi * theta)));
        PARTICLES.at(theta).mouseForce = Vec2D(0,0);
        PARTICLES.at(theta).direction = Vec2D(0,0);
        PARTICLES.at(theta).speed = 0;
        Vec2D v = PARTICLES.at(theta).forceToMoveToPoint(source);
        PARTICLES.at(theta).direction = v.normalize();
        PARTICLES.at(theta).speed = v.length();        
    }

    glFlush();
}

// Function to move particle to a given (x,y) coordinate
void moveParticle(float x, float y, bool isAttracting) {  
    for(long long unsigned int i = 0; i < PARTICLES.size(); i++) {
        if(PARTICLES.at(i).position.distanceTo(Point2D(x, y)) <= PARTICLES.at(i).range + GLOBAL_RANGE) {            
            PARTICLES.at(i).mouseForce = Vec2D::createVector(PARTICLES.at(i).position, Point2D(x, y)).multiply(isAttracting ? 1 : -1).normalize().multiply(GLOBAL_SPEED);
        }
        
    }      
}

// Toggle the display of direction vectors 
void displayDirectionVectors() {    
    for(Particle current_particle : PARTICLES) {
        if(current_particle.speed != 0) {
            glLineWidth(2);
            glBegin(GL_LINES);
            glColor3f(1,1,0);
            glVertex2f(current_particle.position.mX, current_particle.position.mY);
            Point2D directionVector = current_particle.direction.multiply(15).movePoint(current_particle.position);
            glVertex2f(directionVector.mX, directionVector.mY);
            glEnd();
        }        
    }    
}

// Display function
void display() { 
    glClear(GL_COLOR_BUFFER_BIT);
    for(Particle current_particle : PARTICLES) {
        glPointSize(current_particle.size);
        glBegin(GL_POINTS);
        glColor3f(current_particle.colour.r, current_particle.colour.g, current_particle.colour.b);
        glVertex2f(current_particle.position.mX, current_particle.position.mY);
        glEnd();
        if(DISPLAY_RANGE_RADIUS) {            
            drawCircle(current_particle.position.mX, current_particle.position.mY, current_particle.range, Colour(1,1,1));
        }
    }

    if(DISPLAY_GLOBAL_RANGE_RADIUS) {         
        drawCircle(MOUSE_X, MOUSE_Y, GLOBAL_RANGE, Colour(1,0,1));
    }

    if(SHOW_DIRECTION_VECTORS) {
        displayDirectionVectors();
    }
    
    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h) {    
    GLOBAL_WIDTH = w;
    GLOBAL_HEIGHT = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);    

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);    
}

// Function that adds particle at mouse position
void addNewParticleAtMousePos(float x, float y) {
    Particle new_particle = generateParticle(x, y);
    PARTICLES.push_back(new_particle);
    glutPostRedisplay();
}

// Function that deletes particle closest to mouse position
void deleteNearestParticle(float x, float y) {
    Point2D mousePos (x, y);
    float distance = MAXFLOAT;
    int index;
    for(long long unsigned int i = 0; i < PARTICLES.size(); i++) {
        float current_distance = PARTICLES.at(i).position.distanceTo(mousePos);
        if(current_distance < distance) {
            distance = current_distance;
            index = i;
        }        
    }      
    PARTICLES.erase(PARTICLES.begin() + index);
    glutPostRedisplay();
}

// Increase global speed
void increaseSpeed() {
    if(GLOBAL_SPEED <= 50) {
        GLOBAL_SPEED++;
    }    
}

// Decrease global speed
void decreaseSpeed() {    
    if(GLOBAL_SPEED > 0) {
        GLOBAL_SPEED--;
    }    
}

// Timer function for animation
void timer(int value) { 
    if(ANIMATE) {
        for(long long unsigned int i = 0; i < PARTICLES.size(); i++) {     
                PARTICLES.at(i).update(GLOBAL_WIDTH, GLOBAL_HEIGHT);   
        }
    }

    glutPostRedisplay();    
    glutTimerFunc(17, timer, 0);
}

// Update global mouse position
void updateMouse(int x, int y) {
    y = GLOBAL_HEIGHT - y;
    MOUSE_X = x;
    MOUSE_Y = y;
}

// Increase global range
void increaseGlobalRange() {
    if(GLOBAL_RANGE <= 95) {
        GLOBAL_RANGE = GLOBAL_RANGE + 5;
    }
}

// Decrease global range
void decreaseGlobalRange() {
    if(GLOBAL_RANGE > 0) {
        GLOBAL_RANGE = GLOBAL_RANGE - 5;
    }
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
    y = GLOBAL_HEIGHT - y;
    switch(key) {
        case 'a':
            addNewParticleAtMousePos(float(x), float(y));
            break;   
        case 'd':            
            deleteNearestParticle(float(x), float(y));
            break;
        case '+':
            increaseGlobalRange();
            break;
        case '-':
            decreaseGlobalRange();
            break;
        case 'c':            
            MOUSE_X = x;
            MOUSE_Y = y;
            DISPLAY_RANGE_RADIUS = !DISPLAY_RANGE_RADIUS;              
            break;
        case 'C':
            DISPLAY_GLOBAL_RANGE_RADIUS = !DISPLAY_GLOBAL_RANGE_RADIUS;
            break;
        case 'r':
            generateInitialParticles();
            break;
        case 'q':
        case 27:
            exit(0);
            break;    
        case 32:
            ANIMATE = !ANIMATE;
            break;
        case 'b':
            arrangeInCircle(GLOBAL_WIDTH/2, GLOBAL_HEIGHT/2, 350);
            break;
        case 'z':
            arrangeInFlower(GLOBAL_WIDTH/4, GLOBAL_HEIGHT/4, 350);
            break;
        case 'v':
            SHOW_DIRECTION_VECTORS = !SHOW_DIRECTION_VECTORS;
            break;
    }
}

// Special keyboard function
void specialKeyboard(int key, int x, int y) {
    y = GLOBAL_HEIGHT - y;
    switch(key) {
        case GLUT_KEY_UP:
            increaseSpeed();
            break;
        case GLUT_KEY_DOWN:
            decreaseSpeed();
            break;
    }
}

// Idle function
void idle() {    
    if(IS_LEFT_PRESSED) {
        moveParticle(MOUSE_X, MOUSE_Y, true);
    }
    if(IS_RIGHT_PRESSED) {
        moveParticle(MOUSE_X, MOUSE_Y, false);
    }
}

// Mouse drag function
void drag(int x, int y) {
    y = GLOBAL_HEIGHT - y;
    MOUSE_X = x;
    MOUSE_Y = y;
    if(IS_LEFT_PRESSED) {
        moveParticle(float(x), float(y), true);
    } else if(IS_RIGHT_PRESSED) {
        moveParticle(float(x), float(y), false);
    }
    
}

// Mouse function
void mouse(int button, int state, int x, int y) {
    y = GLOBAL_HEIGHT - y;
    MOUSE_X = x;
    MOUSE_Y = y;
    if(button == GLUT_LEFT_BUTTON) {        
        if(state == GLUT_DOWN) {
            IS_LEFT_PRESSED = true;
            moveParticle(float(x), float(y), true);
            glutMotionFunc(drag);              
        }
        if(state == GLUT_UP) {
            IS_LEFT_PRESSED = false;
            glutMotionFunc(nullptr);            

        }
    }
    if(button == GLUT_RIGHT_BUTTON) {        
        if(state == GLUT_DOWN) {
            moveParticle(float(x), float(y), false);
            IS_RIGHT_PRESSED = true;
            glutMotionFunc(drag);              
        }
        if(state == GLUT_UP) {
            IS_RIGHT_PRESSED = false;
            glutMotionFunc(nullptr);            

        }
    }
}

void printCommands () {
    std::cout << "MOUSE COMMANDS" << "\n";
    std::cout << "Left Mouse Button + Hold - Attract particles within range" << "\n";
    std::cout << "Right Mouse Button + Hold - Repel particles within range" << "\n";

    std::cout << "KEYBOARD COMMANDS (case sensitive)" << "\n";
    std::cout << "a - Add new particle at mouse position" << "\n";
    std::cout << "d - Delete particle nearest to mouse position" << "\n";
    std::cout << "+ - Increase global range that particles can attract/repel within" << "\n";
    std::cout << "- - Increase global range that particles can attract/repel within" << "\n";
    std::cout << "UP ARROW - Increase global speed that particles move with" << "\n";
    std::cout << "DOWN ARROW - Decrease global speed that particles move with" << "\n";
    std::cout << "Space bar - Pause the simulation" << "\n";
    std::cout << "r - Reset animation with brand new particles" << "\n";
    std::cout << "q - Quit Program" << "\n";
    std::cout << "ESC - Quit Program" << "\n";
    std::cout << "c - Toggle display of range of each individual particle" << "\n";
    std::cout << "C - Toggle display of global range of mouse cursor" << "\n";
    std::cout << "b - Arrange the particles in a circle shape" << "\n";
    std::cout << "z - Arrange the particles in a flower shape" << "\n";
    std::cout << "v - Toggle display of direction vectors of each particle" << "\n";
}

// main function - entry of the program
int main(int argc, char** argv)
{                
    printCommands();
    generateInitialParticles();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutCreateWindow("2D Particle System");    
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);    
    glutPassiveMotionFunc(updateMouse);    
    glutTimerFunc(17, timer, 0);
    glutIdleFunc(idle);
    glutDisplayFunc(display);   
    glutMainLoop();

    return (0);
}