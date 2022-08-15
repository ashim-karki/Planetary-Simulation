#include <iostream>
#include <vector>
#include <math.h>
#include <engine.hpp>

#include <SFML/Graphics.hpp>
const float M_PI = PIlib;

using namespace std;
const double GravitationalConstant = 6.674 * pow(10.0, -11);

long double calculateGravitationalForce(double mass1, double mass2, double distance) {
    
    const double force = GravitationalConstant * (mass1 * mass2 / pow(distance, 2));
    return force;
}

movementVector vectorFromPlanets(planet planet1, planet planet2) {
    movementVector newVector;
    newVector.x = planet2.cordinates[0] - planet1.cordinates[0];
    newVector.y = planet2.cordinates[1] - planet1.cordinates[1];
    return newVector;
}

movementVector getVectorFromForce(double mass, long double force, double direction) {
    movementVector newVector;
    double magnitude = (force / mass) / 100000 * 86400;
    newVector.setVector(direction, magnitude);
    return newVector;
}

double massToRadius(double mass) {
    // 5514 is the Mean density (kg/m3) of earth.
    // formula for radius of sphere from volume
    double radius = cbrt(3 * (mass / (5514)) / (3 * M_PI));
    return radius;
}

int main() {
    srand(time(NULL));

    bool clearScreen = true;

    int screenDimensions[2] = { 1500, 1000 };
    const double pixelToSize = 3000000;
    bool mouseBtns[2];

    const int frameCap = 180;

    long int currentFrame = 0;
    long int currentPhysicsUpdate = 0;

    const int physicsUpdatesPerFrame = 60;

    int largestPlanetNum = 10;

    int planetAmt = 0;
    cout << "How many planets do you want? ";
    cin >> planetAmt;
    cout << "Solar System> " << endl;
    int flag1 = 0;
    cin >>   flag1;

    if (flag1 == 1)
    {
        sf::RenderWindow window(sf::VideoMode(screenDimensions[0], screenDimensions[1]), "Planetary Simulation");
        //sf::RenderWindow newd(sf::VideoMode(screenDimensions[0], screenDimensions[1]), "new window");

        window.setFramerateLimit(frameCap);
        sf::CircleShape planetShape(100.f);
        planetShape.setFillColor(sf::Color::Red);
        sf::CircleShape solarplanet(100.f);
        sf::CircleShape massRadius(100.f);
        massRadius.setFillColor(sf::Color::White);
        sf::CircleShape Sun(100.0f);
        sf::VertexArray vectorDraw(sf::LinesStrip, 2);
        sf::VertexArray orbit(sf::LineStrip, 2);
        sf::RectangleShape fadeRect;
        sf::Texture texture;
        sf::IntRect rectsource(0,0, 150, 150);
        sf::Transform scalefactor;
        sf::View view(sf::Vector2f(screenDimensions[0] / 2, screenDimensions[1] / 2), sf::Vector2f(0, 0));



        //texture.loadFromFile("include/new.png");
        if (!texture.loadFromFile("Sprites-01.png"))
        {
            cout << "error loading file";
        }
        sf::Sprite sprite(texture, rectsource);
        solarplanet.setTexture(&texture);
        solarplanet.setTextureRect(rectsource);
        //sprite.setColor(sf::Color::Green);
       //sprite.setScale(.5f, .5f);
        //sf::Window.setView;
        fadeRect.setSize(sf::Vector2f(screenDimensions[0], screenDimensions[1]));
        fadeRect.setFillColor(sf::Color(0, 0, 0, 10));
        fadeRect.setPosition(sf::Vector2f(0, 0));
        double mass_array[] = { 1989000, 0.330,	4.87,	5.97,	0.073	,0.642	,1898,	568,	86.8	,102 };
        double radius_array[] = { 69634.0, 2439.7,6051.8,6371.0,3389.5,69911,58232,25362,24622,1188.3 };
        double coordinates_array[] = { 0,879, 1382, 1796, 2579,7786,14335,28725,44951,59000 };
        double velocity_array[] = { 0,47.9, 35, 941.7, 24.1, 13.1, 9.7, 6.8, 5.4, 4.67 };
        /*vector <sun> suns;

        sun sun1;
        sun1.cordinates[0] = screenDimensions[0] * pixelToSize / 2.0;
        sun1.cordinates[1] = screenDimensions[1] * pixelToSize / 2.0;
        sun1.mass = 1.989 * pow(10, 30);
        suns.push_back(sun1);*/

        vector <planet>SolarSystem;
        for (int i = 0; i < 10; i++)
        {
            
            planet presetPlanet;
            presetPlanet.planetID = i;
            presetPlanet.cordinates[0] = 2.2 * pow(10, 9) + coordinates_array[i] * pow(10, 5);
            presetPlanet.cordinates[1] = 1.4 * pow(10, 9);
            cout << presetPlanet.cordinates[1] << endl;
            presetPlanet.customRadius = radius_array[i] * 1000;
            presetPlanet.mass = mass_array[i] * pow(10, 24);
            if (i != 0)
            {
                presetPlanet.vector.x = 0;
                presetPlanet.vector.y = sqrt((GravitationalConstant * mass_array[0] * pow(10, 24)) / (coordinates_array[i] * pow(10, 5))) * 0.1;
            }
            SolarSystem.push_back(presetPlanet);
        }
        vector <planet> planets;
        for (int i = 0; i < planetAmt; i++) {
            planet currentPlanet;
            largestPlanetNum++;
            currentPlanet.planetID = i;
            currentPlanet.cordinates[0] = rand() % screenDimensions[0] * pixelToSize;
            currentPlanet.cordinates[1] = rand() % screenDimensions[1] * pixelToSize;
            currentPlanet.mass = (rand() % 49 + 1) * pow(10, 24); // 1 to 100 * 10^24 | Earth is 5.9722 * 10^24
            planets.push_back(currentPlanet);
        }

        /*vector <planet> suns;
        {
            planet sun;
            sun.planetID = largestPlanetNum;
            largestPlanetNum++;
            sun.cordinates[0] = screenDimensions[0] * pixelToSize / 2;
            sun.cordinates[1] = screenDimensions[1] * pixelToSize / 2;
            sun.mass = pow(0.0130 * 1000000.0, 24);
            suns.push_back(sun);
        }*/

        cout << largestPlanetNum << endl;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                // catch the resize events
                if (event.type == sf::Event::Resized)
                {
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    screenDimensions[0] = event.size.width;
                    screenDimensions[1] = event.size.height;
                    fadeRect.setSize(sf::Vector2f(screenDimensions[0], screenDimensions[1]));
                    window.setView(sf::View(visibleArea));
                }
            }

            mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            if (mouseBtns[0]) {
                int mouseCord[2] = { sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y };
                if (mouseCord[0] >= 0 && mouseCord[0] <= screenDimensions[0] && mouseCord[1] >= 0 && mouseCord[1] <= screenDimensions[1]) {
                    planet newPlanet;
                    newPlanet.planetID = largestPlanetNum;
                    largestPlanetNum++;
                    //note: make it dynamic
                    newPlanet.mass = 1 * pow(10, 24);
                    newPlanet.cordinates[0] = mouseCord[0] * pixelToSize;
                    newPlanet.cordinates[1] = mouseCord[1] * pixelToSize;
                    while (mouseBtns[0]) {
                        window.clear();
                        //why is frameCap
                        newPlanet.mass += newPlanet.mass / frameCap;
                        double newPlanetRadius;
                        double actualRadius;
                        bool customRadius = false;
                        if (newPlanet.customRadius != 0) {
                            newPlanetRadius = newPlanet.customRadius;
                            customRadius = true;
                            //denisty dynamic
                            actualRadius = massToRadius(newPlanet.mass);
                        }
                        else {
                            newPlanetRadius = massToRadius(newPlanet.mass);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            newPlanet.customRadius = newPlanetRadius;
                        }

                        if (customRadius) {
                            massRadius.setRadius((float)(massToRadius(newPlanet.mass) / pixelToSize));
                            massRadius.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - actualRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - actualRadius / pixelToSize));
                            window.draw(massRadius);
                        }

                        // draw all other planets
                        for (auto& currentPlanet : planets) {
                            if (currentPlanet.isAlive) {
                                planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                                double planetRadius;
                                if (currentPlanet.customRadius != 0) {
                                    planetRadius = currentPlanet.customRadius;
                                }
                                else {
                                    planetRadius = massToRadius(currentPlanet.mass);
                                }
                                planetShape.setRadius((float)(planetRadius / pixelToSize));
                                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                                window.draw(planetShape);
                            }
                        }
                        if (flag1 == true)
                        {
                            for (auto presetPlanet : SolarSystem)
                            {
                                if (presetPlanet.isAlive) {
                                    planetShape.setFillColor(sf::Color(presetPlanet.colour[0], presetPlanet.colour[1], presetPlanet.colour[2]));
                                    double planetRadius;
                                    if (presetPlanet.customRadius != 0) {
                                        planetRadius = presetPlanet.customRadius;
                                    }
                                    else {
                                        planetRadius = massToRadius(presetPlanet.mass);
                                    }
                                    //planetShape.setRadius((float)(planetRadius / pixelToSize));
                                    planetShape.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                                    window.draw(planetShape);
                                }
                            }
                        }
                        /*for (auto sun : suns)
                        {
                            if (sun.isAlive)
                            {
                                planetShape.setFillColor(sf::Color::Yellow);
                                double sunradius;
                                if (sun.customRadius != 0)
                                {
                                    sunradius = sun.customRadius;

                                }
                                else
                                    sunradius = 1.229e7;

                                planetShape.setRadius((float)(sunradius / pixelToSize));
                                planetShape.setPosition(sf::Vector2f(sun.cordinates[0] / pixelToSize - sunradius / pixelToSize, sun.cordinates[1] / pixelToSize - sunradius / pixelToSize));
                                //cout << "sun radus" << (float)(sunradius / pixelToSize);

                                window.draw(planetShape);
                            }
                        }

                        planetShape.setFillColor(sf::Color(newPlanet.colour[0], newPlanet.colour[1], newPlanet.colour[2]));
                        planetShape.setRadius((float)(newPlanetRadius / pixelToSize));
                        planetShape.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - newPlanetRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - newPlanetRadius / pixelToSize));
                        cout << newPlanet.cordinates[0] << endl;
                        cout << newPlanet.cordinates[1] << endl;
                        window.draw(planetShape);*/

                        // Drawing vector to show direction of planet
                        mouseCord[0] = sf::Mouse::getPosition(window).x;
                        mouseCord[1] = sf::Mouse::getPosition(window).y;
                        vectorDraw[0].position = sf::Vector2f(mouseCord[0], mouseCord[1]);
                        vectorDraw[1].position = sf::Vector2f(newPlanet.cordinates[0] / pixelToSize, newPlanet.cordinates[1] / pixelToSize);
                        window.draw(vectorDraw);

                        window.display();

                        mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                    }
                    // on mouse release
                    mouseCord[0] = sf::Mouse::getPosition(window).x;
                    mouseCord[1] = sf::Mouse::getPosition(window).y;

                    newPlanet.vector.x = (mouseCord[0] * pixelToSize - newPlanet.cordinates[0]) / frameCap / physicsUpdatesPerFrame / 2; //move how far the mouse moved every 5 seconds
                    newPlanet.vector.y = (mouseCord[1] * pixelToSize - newPlanet.cordinates[1]) / frameCap / physicsUpdatesPerFrame / 2;

                    if (flag1 == 1)
                        SolarSystem.push_back(newPlanet);
                    else
                        planets.push_back(newPlanet);

                    window.clear();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                clearScreen = !clearScreen;

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {};
            }


            /* if (event.type = sf::Event::KeyReleased)
             {
                 if(event.key.code == sf::Keyboard::R)
                     window.setView(sf::View(sf::FloatRect(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, event.size.width, event.size.height)));
             }*/




            window.clear();

            if (clearScreen) {
                if (currentFrame % frameCap == 0 or currentFrame % frameCap == round(frameCap / 2)) {
                    window.draw(fadeRect);

                }
            }
            else {
                window.clear();

            }


            while (currentPhysicsUpdate < physicsUpdatesPerFrame) {
                for (auto& currentPlanet : planets) {
                    if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                    double currentPlanetRadius = 0;
                    if (currentPlanet.customRadius != 0) {
                        currentPlanetRadius = currentPlanet.customRadius;
                    }
                    else {
                        currentPlanetRadius = massToRadius(currentPlanet.mass);
                        //cout << "current planet radius: " << currentPlanetRadius;
                    }
                    for (auto& planetToCheck : planets) {
                        if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                        const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                        movementVector vectorOfPlanets = vectorFromPlanets(currentPlanet, planetToCheck);
                        const double gravitationalForce = calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                        movementVector vectorToAdd = getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                        currentPlanet.vector.x += vectorToAdd.x;
                        currentPlanet.vector.y += vectorToAdd.y;
                        /*std::cout << "PLANET ID : " << currentPlanet.planetID << std::endl;
                        std::cout << "VELOCITY X: " << vectorToAdd.x << std::endl;
                        std::cout << "VELOCITY Y: " << vectorToAdd.y << std::endl;
                        std::cout << "VELOCITY Added : " << currentPlanet.vector.y << std::endl;
                        */
                        if (currentPlanetRadius + planetToCheckRadius > vectorOfPlanets.getMagnitude() - vectorToAdd.getMagnitude()) {
                            if (currentPlanet.mass > planetToCheck.mass) {
                                currentPlanet.mass += planetToCheck.mass;
                                planetToCheck.isAlive = false;
                            }
                        }
                    }

                }




                for (auto& currentPlanet : SolarSystem) {
                    if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                    double currentPlanetRadius = 0;
                    if (currentPlanet.customRadius != 0) {
                        currentPlanetRadius = currentPlanet.customRadius;
                    }
                    else {
                        currentPlanetRadius = massToRadius(currentPlanet.mass);
                        //cout << "current planet radius: " << currentPlanetRadius;
                    }

                    for (auto& planetToCheck : SolarSystem) {
                        if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                        const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                        movementVector vectorOfPlanets = vectorFromPlanets(currentPlanet, planetToCheck);
                        const double gravitationalForce = calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                        movementVector vectorToAdd = getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                        //if (currentPlanet.planetID = 0)
                        {
                            currentPlanet.vector.x += vectorToAdd.x * 0.01;
                            //cout << currentPlanet.vector.x << endl;
                            currentPlanet.vector.y += vectorToAdd.y * 0.01;
                        }
                        //cout << currentPlanet.vector.y << endl;
                        /*std::cout << "PLANET ID : " << currentPlanet.planetID << std::endl;
                        std::cout << "VELOCITY X: " << vectorToAdd.x << std::endl;
                        std::cout << "VELOCITY Y: " << vectorToAdd.y << std::endl;
                        std::cout << "VELOCITY Added : " << currentPlanet.vector.y << std::endl;
                        */
                        if (currentPlanetRadius + planetToCheckRadius > vectorOfPlanets.getMagnitude() - vectorToAdd.getMagnitude()) {
                            if (currentPlanet.mass > planetToCheck.mass) {
                                currentPlanet.mass += planetToCheck.mass;
                                planetToCheck.isAlive = false;
                            }
                        }
                    }

                }

                for (auto& currentPlanet : planets)
                {

                    currentPlanet.move();
                }
                for (auto& currentplanet : SolarSystem)
                {
                    currentplanet.move();
                }

                currentPhysicsUpdate++;
            }

            currentPhysicsUpdate = 0;



            for (auto& currentPlanet : planets) {
                if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                double currentPlanetRadius = 0;
                if (currentPlanet.customRadius != 0) {
                    currentPlanetRadius = currentPlanet.customRadius;
                }
                else {
                    currentPlanetRadius = massToRadius(currentPlanet.mass);
                }
                //planetShape.setRadius((float)(currentPlanetRadius / pixelToSize));
                //planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - currentPlanetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - currentPlanetRadius / pixelToSize));
                window.draw(planetShape);
            }
            /*for (auto sun : suns)
            {
                if (sun.isAlive)
                {
                    planetShape.setFillColor(sf::Color::Yellow);
                    double sunradius;
                    if (sun.customRadius != 0)
                    {
                        sunradius = sun.customRadius;

                    }
                    else
                        sunradius = 1.229e7;

                    planetShape.setRadius((float)(sunradius / pixelToSize));
                    planetShape.setPosition(sf::Vector2f(sun.cordinates[0] / pixelToSize - sunradius / pixelToSize, sun.cordinates[1] / pixelToSize - sunradius / pixelToSize));
                    //cout << "sun radus" << (float)(sunradius / pixelToSize);

                    window.draw(planetShape);
                }
            }*/
            if (flag1 == true)
            {

                for (auto& presetPlanet : SolarSystem) {
                    if (presetPlanet.isAlive) {
                        planetShape.setFillColor(sf::Color(presetPlanet.colour[0], presetPlanet.colour[1], presetPlanet.colour[2]));
                        double planetRadius;
                        if (presetPlanet.customRadius != 0) {
                            planetRadius = presetPlanet.customRadius;
                        }
                        else {
                            planetRadius = massToRadius(presetPlanet.mass);
                        }
                        if (presetPlanet.planetID < 10)
                        {

                            solarplanet.setTextureRect(rectsource);
                            rectsource.top += 150;

                            solarplanet.setRadius((float)( planetRadius / pixelToSize));
                            solarplanet.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                            window.draw(solarplanet);
                        }//setrect()
                        else
                        {
                            planetShape.setRadius((float)(planetRadius / pixelToSize));
                            planetShape.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));

                            window.draw(planetShape);
                        }

                        //cout << "PRESET PLANET RADIUS:" << presetPlanet.mass << std::endl;
                    }
                }
                rectsource.top = 0;

            }

            window.display();

            currentFrame++;
        }
    }
    
    return 0;
}