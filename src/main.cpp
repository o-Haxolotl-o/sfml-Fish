#include <complex>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"
using namespace std;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Fish");
    window.setFramerateLimit(300);
    //Init_____________________________________________________________________________________
    const int vertNum = 8; //how many points

    //sf::Texture gradient_texture("../../src/Images/Gradient.png");
    sf::CircleShape c[vertNum];
    for (int i = 0; i < vertNum; i++) { //vert properties
        //c[i].setTexture(gradient_texture);
        c[i].setPosition(sf::Vector2f(1000+i, 10));
        c[i].setRadius( 50/((i/2)+1) );
        c[i].setFillColor(sf::Color::White);

    }
    sf::Vector2f cd[vertNum];
    for (int i = 0; i < vertNum; i++) {
        //cd[i].x = i*5;
        //cd[i].y = i;
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        //forces
        for (int i = 0; i < vertNum; i++) {
            //cd[i].y+=.2; //gravity
            cd[i].x*=.95;//friction--
            cd[i].y*=.95;
        }


        //bounce
        for (int i = 0; i < vertNum; i++) {
            if(c[i].getPosition().y > window.getSize().y) {
                cd[i].y *=-.9;
                c[i].setPosition(sf::Vector2f(c[i].getPosition().x, window.getSize().y+1));
            }
            if(c[i].getPosition().x < 0) {
                cd[i].x *=-.9;
                c[i].setPosition(sf::Vector2f(0, c[i].getPosition().y));
            }
            if(c[i].getPosition().x > window.getSize().x) {
                cd[i].x *=-.9;
                c[i].setPosition(sf::Vector2f(window.getSize().x, c[i].getPosition().y));
            }
        }
        //distance constraint
        for (int i = 0; i < vertNum; i++) {
            if (i < vertNum - 1) {
                float desiredDistance = 64; // how far apart for each vertex
                sf::Vector2f vector = c[i].getPosition()-c[i+1].getPosition(); //direction to next vertex
                float A = vector.x;
                float B = vector.y;
                float C = sqrt(A*A+B*B);
                sf::Vector2f normVector = sf::Vector2f(vector.x/C,vector.y/C); // pythagorean theorem for dist
                if (C == 0) //dividing by zero is BAD
                    normVector = sf::Vector2f(0,0);
                sf::Vector2f midPoint = sf::Vector2f((c[i].getPosition().x + c[i+1].getPosition().x)/2 , (c[i].getPosition().y + c[i+1].getPosition().y)/2); //midpoint of verts

                //c[i].setPosition(sf::Vector2f(midPoint.x+normVector.x*desiredDistance, midPoint.y+normVector.y*desiredDistance));
                c[i+1].setPosition(sf::Vector2f(midPoint.x-normVector.x*desiredDistance, midPoint.y-normVector.y*desiredDistance)); //apply distance constraint
                //cd[i+1] += sf::Vector2f(normVector.x*desiredDistance, normVector.y*desiredDistance);
            }
            /*else {
                float desiredDistance = 64; // how far apart for each vertex
                sf::Vector2f vector = c[i].getPosition()-c[0].getPosition(); //direction to next vertex
                float A = vector.x;
                float B = vector.y;
                float C = sqrt(A*A+B*B);
                sf::Vector2f normVector = sf::Vector2f(vector.x/C,vector.y/C); // pythagorean theorem for dist
                if (C == 0) //dividing by zero is BAD
                    normVector = sf::Vector2f(0,0);
                sf::Vector2f midPoint = sf::Vector2f((c[i].getPosition().x + c[0].getPosition().x)/2 , (c[i].getPosition().y + c[0].getPosition().y)/2); //midpoint of verts

                //c[i].setPosition(sf::Vector2f(midPoint.x+normVector.x*desiredDistance, midPoint.y+normVector.y*desiredDistance));
                c[0].setPosition(sf::Vector2f(midPoint.x-normVector.x*desiredDistance, midPoint.y-normVector.y*desiredDistance)); //apply distance constraint
                //cd[i+1] += sf::Vector2f(normVector.x*desiredDistance, normVector.y*desiredDistance);
            }*/
        }

        //apply force
        for (int i = 0; i < vertNum; i++) {
            c[i].setPosition(c[i].getPosition()+cd[i]);
        }
        c[0].setPosition((sf::Vector2f)sf::priv::InputImpl::getMousePosition());
        //draw calls
        window.clear();
        for (int i = 0; i < vertNum; i++) {
            window.draw(c[i]);
        }
        window.display();
    }
}
