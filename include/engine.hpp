#include <math.h>

const double pi = 3.141592653589793238463;

class movementVector {
public:
    double x = 0;
    double y = 0;

    double getDistance() {
        const double distance = sqrt(pow(x, 2) + pow(y, 2));
        return distance;
    }

    double getDirection() {
        double direction = atan(x / y);
        if (y == 0) {
            if (x > 0) {
                direction = pi / 2;
            }
            else {
                direction = -pi / 2;
            }
        }
        else if (y < 0) {
            if (x > 0) {
                direction = pi / 2 + (pi / 2 + direction);
            }
            else {
                direction = -(pi / 2) - (pi / 2 - direction);
            }
        }
        return direction;
    }

    void updatePos(double direction, double distance) {
        x = distance * sin(direction);
        y = distance * cos(direction);
    }
};

class planet {
public:
    movementVector vector;
    int colour[3] = { rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55 };
    int planetID;
    bool isAlive = true;
    double mass; // in kg
    double customRadius = 0;
    double cordinates[2] = { 0, 69 }; // in km, will be divided by 3000000 to get pixels
    void move() { // moves the planet by the vector
        cordinates[0] += vector.x;
        cordinates[1] += vector.y;
    }
};