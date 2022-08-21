#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <engine.hpp> 
#include "Menu.hpp" 

using namespace std;

const float M_PI = PIlib;
const double GravitationalConstant = 6.674 * pow(10.0, -11);

class Output
{
private:
    double mass;
    sf::Vector2f velocity;
    sf::Vector2f coordinates;
    sf::Font displayfont;
public:
    sf::Text dis, vel;
    sf::RectangleShape box;
    Output() {};
    Output(double m, sf::Vector2f v, sf::Vector2f coord) : mass(m), velocity(v), coordinates(coord) {};

    void printOutput(sf::RenderWindow& window)
    {
        displayfont.loadFromFile("Fonts/BRITANIC.TTF");
        dis.setFont(displayfont);
        vel.setFont(displayfont);
        box.setPosition(coordinates.x, coordinates.y);
        vel.setPosition(coordinates.x, coordinates.y + 20);
        box.setSize(sf::Vector2f(270, 140));

        box.setFillColor(sf::Color(255, 255, 255, 150));

        dis.setString(toConvert(mass));
        vel.setString(toConvert(velocity));
        dis.setOutlineColor(sf::Color::White);

        box.setScale(.5, .5);
        dis.setScale(.5, .5);
        vel.setScale(.4, .4);

        dis.setCharacterSize(40);
        vel.setCharacterSize(50);
        dis.setFillColor(sf::Color::Red);
        vel.setFillColor(sf::Color::Red);
        dis.setPosition(coordinates.x, coordinates.y);
        window.draw(box);
        window.draw(dis);
        window.draw(vel);
    }
    string toConvert(sf::Vector2f vel)
    {
        int powerx = 0, powery = 0;
        while (vel.x > 10 || vel.y > 10)
        {
            vel.x = vel.x / 10;
            powerx++;
            vel.y = vel.y / 10;
            powery++;
        }
        string velx = to_string(static_cast<int>(vel.x));

        string vely = to_string(static_cast<int>(vel.y));
        string total = "Vx: " + velx + "* 10 ^ " + to_string(powerx) + "\nVy: " + vely + "* 10 ^ " + to_string(powery);
        return total;
    }
    string toConvert(double mass)
    {
        int power = 0;
        while (mass >= 10)
        {
            mass = mass / 10;
            power++;
        }
        string value = to_string(static_cast<int> (mass));
        string index = to_string(power);
        string total = value + "* 10 ^ " + index + "kg";
        return total;

    }
    void getpos(double a, double b)
    {
        coordinates.x = a;
        coordinates.y = b;
    }
    void getmass(double M)
    {
        mass = M;
    }
    void getvelocity(double a, double b)
    {
        velocity.x = a;
        velocity.y = b;
    }
};

double massToRadius(double mass)
{
    double radius = cbrt(3 * (mass / (5514)) / (4 * M_PI));
    return radius;
}

class Calculation
{
public:
    long double calculateGravitationalForce(double mass1, double mass2, double distance) {

        const double force = GravitationalConstant * (mass1 * mass2 / pow(distance, 2));
        return force;
    }
    Move vectorFromPlanets(planet planet1, planet planet2) {
        Move newVector;
        newVector.x = planet2.cordinates[0] - planet1.cordinates[0];
        newVector.y = planet2.cordinates[1] - planet1.cordinates[1];
        return newVector;
    }
    Move getVectorFromForce(double mass, long double force, double direction) {
        Move newVector;
        double magnitude = (force / mass) / 100000 * 86400;
        newVector.setVector(direction, magnitude);
        return newVector;
    }
};


int main()
{
    srand(time(NULL));

    bool clearScreen = true;

    int screenDimensions[2] = { 1500, 1000 };
    const double pixelToSize = 3000000;
    bool mouseBtns[2];


    const int frameCap = 180;

    long int currentFrame = 0;
    long int currentPhysicsUpdate = 0;

    const int physicsUpdatesPerFrame = 120;
    int largestPlanetNum = 10;
    Calculation test1;
    Output output;

    sf::RenderWindow window(sf::VideoMode(screenDimensions[0], screenDimensions[1]), "Planetary Simulation");
    Menu menu(window.getSize().x, window.getSize().y);
    // menudraw returns flag value as well as render menu window
    int flag = menu.menudraw();

    window.setFramerateLimit(frameCap);

    sf::CircleShape planetShape(100.f);
    planetShape.setFillColor(sf::Color::Red);
    sf::CircleShape solarplanet(100.f);
    sf::CircleShape massRadius(100.f);
    massRadius.setFillColor(sf::Color::White);

    sf::VertexArray vectorDraw(sf::LinesStrip, 2); //A vertex is the smallest graphical entity that you can manipulate.
    sf::VertexArray orbit(sf::LineStrip, 2);

    sf::RectangleShape fadeRect;
    sf::Texture texture;
    // bgtexture.loadFromFile("Bg.png");

    sf::IntRect rectsource(0, 0, 150, 150);
    // sf::Sprite backgrounnd(bgtexture);
     //backgrounnd.setOrigin(window.getSize().x/2, window.getSize().y/2);

    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(screenDimensions[0], screenDimensions[1]));
    view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    view.setSize(sf::Vector2f(screenDimensions[0], screenDimensions[1]));


    if (flag == 2)
    {

        if (!texture.loadFromFile("Sprites-01.png"))
        {
            cout << "error loading file";
        }
        //sf::Sprite sprite(texture, rectsource);
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

        vector <planet>SolarSystem;
        for (int i = 0; i < 10; i++)
        {

            planet presetPlanet;
            presetPlanet.planetID = i;
            presetPlanet.cordinates[0] = 2.2 * pow(10, 9) + coordinates_array[i] * pow(10, 5);
            presetPlanet.cordinates[1] = 1.4 * pow(10, 9);
            presetPlanet.Radius = radius_array[i] * 1000;
            presetPlanet.mass = mass_array[i] * pow(10, 24);
            if (i != 0)
            {
                presetPlanet.vector.x = 0;
                presetPlanet.vector.y = sqrt((GravitationalConstant * mass_array[0] * pow(10, 24)) / (coordinates_array[i] * pow(10, 5))) * 0.1;
            }
            SolarSystem.push_back(presetPlanet);
        }

        vector<Output> outputs;

        vector <planet> planets;
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

            //load backgorund image here
            //window.draw(backgrounnd);
            //backgrounnd.setOrigin(screenDimensions[0]/ 2, screenDimensions[1]/ 2);

            mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            if (mouseBtns[0]) {

                int mouseCord[2] = { sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y };
                output.getpos(mouseCord[0], mouseCord[1]);

                if (mouseCord[0] >= 0 && mouseCord[0] <= screenDimensions[0] && mouseCord[1] >= 0 && mouseCord[1] <= screenDimensions[1]) {
                    planet newPlanet;
                    newPlanet.planetID = largestPlanetNum;
                    largestPlanetNum++;

                    newPlanet.mass = 1 * pow(10, 26);
                    newPlanet.cordinates[0] = mouseCord[0] * pixelToSize;
                    newPlanet.cordinates[1] = mouseCord[1] * pixelToSize;
                    while (mouseBtns[0])
                    {
                        window.clear();
                        //output.printOutput(window);
                        newPlanet.mass += newPlanet.mass / frameCap;
                        //UPDATE THE MASS TO CLASS ON EVERY LOOP
                        output.getmass(newPlanet.mass);
                        double newPlanetRadius;
                        double actualRadius;
                        bool Radius = false;
                        if (newPlanet.Radius != 0) {
                            newPlanetRadius = newPlanet.Radius;
                            Radius = true;

                            actualRadius = massToRadius(newPlanet.mass);
                        }
                        else {
                            newPlanetRadius = massToRadius(newPlanet.mass);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            newPlanet.Radius = newPlanetRadius;
                        }

                        if (Radius) {
                            massRadius.setRadius((float)(massToRadius(newPlanet.mass) / pixelToSize));
                            massRadius.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - actualRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - actualRadius / pixelToSize));
                            window.draw(massRadius);
                        }
                        // draw all other planets
                        for (auto& currentPlanet : planets) {
                            if (currentPlanet.isAlive) {
                                planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                                double planetRadius;
                                if (currentPlanet.Radius != 0) {
                                    planetRadius = currentPlanet.Radius;
                                }
                                else
                                {
                                    planetRadius = massToRadius(currentPlanet.mass);
                                }
                                planetShape.setRadius((float)(planetRadius / pixelToSize));
                                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));

                                //window.draw(backgrounnd);
                                window.draw(planetShape);
                            }
                        }
                        //to draw growing planets as the radius increase
                        planetShape.setFillColor(sf::Color(newPlanet.colour[0], newPlanet.colour[1], newPlanet.colour[2]));
                        planetShape.setRadius((float)(newPlanetRadius / pixelToSize));
                        planetShape.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - newPlanetRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - newPlanetRadius / pixelToSize));
                        window.draw(planetShape);

                        for (auto& presetPlanet : SolarSystem)
                        {
                            if (presetPlanet.isAlive) {
                                planetShape.setFillColor(sf::Color(presetPlanet.colour[0], presetPlanet.colour[1], presetPlanet.colour[2]));
                                double planetRadius;
                                if (presetPlanet.Radius != 0) {
                                    planetRadius = presetPlanet.Radius;
                                }
                                else {
                                    planetRadius = massToRadius(presetPlanet.mass);
                                }
                                //solarplanet.setTextureRect(rectsource);
                                //rectsource.top += 150;
                                if (presetPlanet.planetID < 10)
                                {
                                    solarplanet.setTextureRect(rectsource);
                                    rectsource.top += 150;
                                    solarplanet.setRadius((float)(planetRadius / pixelToSize));
                                    solarplanet.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                                    window.draw(solarplanet);
                                }
                                else
                                {
                                    planetShape.setRadius((float)(planetRadius / pixelToSize));
                                    planetShape.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                                    window.draw(planetShape);
                                }
                                //window.draw(backgrounnd);
                            }
                        }
                        rectsource.top = 0;


                        // Drawing vector to show direction of planet
                        mouseCord[0] = sf::Mouse::getPosition(window).x;
                        mouseCord[1] = sf::Mouse::getPosition(window).y;
                        vectorDraw[0].position = sf::Vector2f(mouseCord[0], mouseCord[1]);
                        vectorDraw[1].position = sf::Vector2f(newPlanet.cordinates[0] / pixelToSize, newPlanet.cordinates[1] / pixelToSize);
                        output.printOutput(window);
                        window.draw(vectorDraw);
                        window.display();

                        newPlanet.vector.x = (mouseCord[0] * pixelToSize - newPlanet.cordinates[0]) / frameCap / physicsUpdatesPerFrame / 2; //move how far the mouse moved every 5 seconds
                        newPlanet.vector.y = (mouseCord[1] * pixelToSize - newPlanet.cordinates[1]) / frameCap / physicsUpdatesPerFrame / 2;
                        output.getvelocity(newPlanet.vector.x, newPlanet.vector.y);

                        mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                    }
                    // window.draw(output.display);
                    // on mouse release
                    mouseCord[0] = sf::Mouse::getPosition(window).x;
                    mouseCord[1] = sf::Mouse::getPosition(window).y;
                    SolarSystem.push_back(newPlanet); //add newly formed planets here into the solarsysetm vector
                    outputs.push_back(output);
                    window.clear();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                clearScreen = !clearScreen;

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {};
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {
                //backgrounnd.setPosition(window.getSize().x, window.getSize().y);
                view.zoom(1.009);
                window.setView(view);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                view.move(sf::Vector2f(-5.f, 0.f));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                view.move(sf::Vector2f(+5.f, 0.f));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                view.move(sf::Vector2f(0.f, 5.f));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                view.move(sf::Vector2f(0.f, -5.f));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                view.reset(sf::FloatRect(0, 0, screenDimensions[0], screenDimensions[1]));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                view.zoom(0.99);
                window.setView(view);
            }
            if (clearScreen) {
                if (currentFrame % frameCap == 0 or currentFrame % frameCap == round(frameCap / 2)) {
                    window.draw(fadeRect);

                }
            }
            else {
                window.clear();
            }
            float i = 0;
            while (currentPhysicsUpdate < physicsUpdatesPerFrame) {
                for (auto& currentPlanet : planets) {
                    if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                    double currentPlanetRadius = 0;
                    if (currentPlanet.Radius != 0) {
                        currentPlanetRadius = currentPlanet.Radius;
                    }
                    else {
                        currentPlanetRadius = massToRadius(currentPlanet.mass);

                    }
                    for (auto& planetToCheck : planets) {
                        if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                        const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                        Move vectorOfPlanets = test1.vectorFromPlanets(currentPlanet, planetToCheck);
                        const double gravitationalForce = test1.calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                        Move vectorToAdd = test1.getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                        currentPlanet.vector.x += vectorToAdd.x; //euler integration to update velocity
                        currentPlanet.vector.y += vectorToAdd.y;

                        if (currentPlanetRadius + planetToCheckRadius > vectorOfPlanets.getMagnitude() - vectorToAdd.getMagnitude()) { //collision detection
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
                    if (currentPlanet.Radius != 0) {
                        currentPlanetRadius = currentPlanet.Radius;
                    }
                    else {
                        currentPlanetRadius = massToRadius(currentPlanet.mass);
                        //cout << "current planet radius: " << currentPlanetRadius;
                    }

                    for (auto& planetToCheck : SolarSystem) {
                        if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                        const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                        Move vectorOfPlanets = test1.vectorFromPlanets(currentPlanet, planetToCheck);
                        const double gravitationalForce = test1.calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                        Move vectorToAdd = test1.getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                        //orbit[0].position = sf::Vector2f(planetToCheck.vector.x, planetToCheck.vector.y);
                        currentPlanet.vector.x += vectorToAdd.x * 0.010;
                        currentPlanet.vector.y += vectorToAdd.y * 0.010;
                        // orbit[1].position = sf::Vector2f(planetToCheck.vector.x, planetToCheck.vector.y);
                        // window.draw(orbit);
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
                if (currentPlanet.Radius != 0) {
                    currentPlanetRadius = currentPlanet.Radius;
                }
                else {
                    currentPlanetRadius = massToRadius(currentPlanet.mass);
                }

                planetShape.setRadius((float)(currentPlanetRadius / pixelToSize));
                planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - currentPlanetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - currentPlanetRadius / pixelToSize));
                window.draw(planetShape);
            }

            for (auto& presetPlanet : SolarSystem) {
                if (presetPlanet.isAlive)
                {
                    planetShape.setFillColor(sf::Color(presetPlanet.colour[0], presetPlanet.colour[1], presetPlanet.colour[2]));
                    double planetRadius;
                    if (presetPlanet.Radius != 0) {
                        planetRadius = presetPlanet.Radius;
                    }
                    else {
                        planetRadius = massToRadius(presetPlanet.mass);
                    }
                    if (presetPlanet.planetID < 10)
                    {
                        solarplanet.setTextureRect(rectsource);
                        rectsource.top += 150;

                        solarplanet.setRadius((float)(planetRadius / pixelToSize));
                        solarplanet.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                        window.draw(solarplanet);
                    }
                    else
                    {
                        planetShape.setRadius((float)(planetRadius / pixelToSize));
                        planetShape.setPosition(sf::Vector2f(presetPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, presetPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));

                        window.draw(planetShape);
                    }
                }
            }
            rectsource.top = 0;
            window.display();
            currentFrame++;
        }
    }
    if (flag == 1)
    {
        vector<planet> planets;
        vector<Output> outputs;

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

            mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left); //assignment of bool left click mouse button

            if (mouseBtns[0]) {
                int mouseCord[2] = { sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y }; //store mouse coordinates
                output.getpos(mouseCord[0], mouseCord[1]);
                if (mouseCord[0] >= 0 && mouseCord[0] <= screenDimensions[0] && mouseCord[1] >= 0 && mouseCord[1] <= screenDimensions[1]) { //if inside window
                    planet newPlanet;
                    newPlanet.planetID = largestPlanetNum;
                    largestPlanetNum++;
                    newPlanet.mass = 1 * pow(10, 25);
                    newPlanet.cordinates[0] = mouseCord[0] * pixelToSize;
                    newPlanet.cordinates[1] = mouseCord[1] * pixelToSize;
                    while (mouseBtns[0]) {
                        window.clear();
                        newPlanet.mass += newPlanet.mass / frameCap;
                        output.getmass(newPlanet.mass);
                        double newPlanetRadius;
                        double actualRadius;
                        bool Radius = false;
                        if (newPlanet.Radius != 0) {
                            newPlanetRadius = newPlanet.Radius;
                            Radius = true;
                            actualRadius = massToRadius(newPlanet.mass);
                        }
                        else {
                            newPlanetRadius = massToRadius(newPlanet.mass);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            newPlanet.Radius = newPlanetRadius;
                        }

                        if (Radius) {
                            massRadius.setRadius((float)(massToRadius(newPlanet.mass) / pixelToSize));
                            massRadius.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - actualRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - actualRadius / pixelToSize));
                            window.draw(massRadius);
                        }

                        // draw all other planets
                        for (auto currentPlanet : planets) {
                            if (currentPlanet.isAlive) {
                                planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                                double planetRadius;
                                if (currentPlanet.Radius != 0) {
                                    planetRadius = currentPlanet.Radius;
                                }
                                else {
                                    planetRadius = massToRadius(currentPlanet.mass);
                                }
                                planetShape.setRadius((float)(planetRadius / pixelToSize));
                                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                                window.draw(planetShape);
                            }
                        }

                        planetShape.setFillColor(sf::Color(newPlanet.colour[0], newPlanet.colour[1], newPlanet.colour[2]));
                        planetShape.setRadius((float)(newPlanetRadius / pixelToSize));
                        planetShape.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - newPlanetRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - newPlanetRadius / pixelToSize));
                        window.draw(planetShape);


                        // Drawing vector to show direction of planet
                        mouseCord[0] = sf::Mouse::getPosition(window).x;
                        mouseCord[1] = sf::Mouse::getPosition(window).y;
                        vectorDraw[0].position = sf::Vector2f(mouseCord[0], mouseCord[1]);
                        vectorDraw[1].position = sf::Vector2f(newPlanet.cordinates[0] / pixelToSize, newPlanet.cordinates[1] / pixelToSize);
                        window.draw(vectorDraw);
                        output.printOutput(window);
                        window.display();

                        newPlanet.vector.x = (mouseCord[0] * pixelToSize - newPlanet.cordinates[0]) / frameCap / physicsUpdatesPerFrame / 2; //move how far the mouse moved every 5 seconds
                        newPlanet.vector.y = (mouseCord[1] * pixelToSize - newPlanet.cordinates[1]) / frameCap / physicsUpdatesPerFrame / 2;
                        output.getvelocity(newPlanet.vector.x, newPlanet.vector.y);
                        mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                    }
                    // on mouse release

                    mouseCord[0] = sf::Mouse::getPosition(window).x;
                    mouseCord[1] = sf::Mouse::getPosition(window).y;
                    planets.push_back(newPlanet);
                    outputs.push_back(output);
                    window.clear();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                clearScreen = !clearScreen;

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {}; //
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {

                view.zoom(1.009);
                window.setView(view);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                view.move(sf::Vector2f(0.f, -10.f));
                window.setView(view);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                view.move(sf::Vector2f(-10.f, 0.f));
                window.setView(view);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                view.move(sf::Vector2f(0.f, 10.f));
                window.setView(view);

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                view.move(sf::Vector2f(+10.f, 0.f));
                window.setView(view);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                view.reset(sf::FloatRect(0, 0, screenDimensions[0], screenDimensions[1]));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            {
                view.zoom(0.99);
                window.setView(view);
            }
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
                    if (currentPlanet.Radius != 0) {
                        currentPlanetRadius = currentPlanet.Radius;
                    }
                    else {
                        currentPlanetRadius = massToRadius(currentPlanet.mass);
                    }
                    for (auto& planetToCheck : planets) {
                        if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                        const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                        Move vectorOfPlanets = test1.vectorFromPlanets(currentPlanet, planetToCheck);
                        const double gravitationalForce = test1.calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                        Move vectorToAdd = test1.getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                        currentPlanet.vector.x += vectorToAdd.x;
                        currentPlanet.vector.y += vectorToAdd.y;
                        if (currentPlanetRadius + planetToCheckRadius > vectorOfPlanets.getMagnitude() - vectorToAdd.getMagnitude()) {
                            if (currentPlanet.mass > planetToCheck.mass) {
                                currentPlanet.mass += planetToCheck.mass;
                                planetToCheck.isAlive = false;
                            }
                        }
                    }
                }
                for (auto& currentPlanet : planets) {
                    currentPlanet.move();
                }
                currentPhysicsUpdate++;
            }

            currentPhysicsUpdate = 0;

            for (auto& currentPlanet : planets) {
                if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                double currentPlanetRadius = 0;
                if (currentPlanet.Radius != 0) {
                    currentPlanetRadius = currentPlanet.Radius;
                }
                else {
                    currentPlanetRadius = massToRadius(currentPlanet.mass);
                }
                planetShape.setRadius((float)(currentPlanetRadius / pixelToSize));
                planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - currentPlanetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - currentPlanetRadius / pixelToSize));
                window.draw(planetShape);
            }
            window.display();
            currentFrame++;
        }
        return 0;
    }
    return 0;
}
