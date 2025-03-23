#include <complex>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../cmake-build-debug/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"
#include "../cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"
using namespace std;

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Fish");
    window.setFramerateLimit(300);
    //Init__________________________________________________________________________________________________________________________________________________________________________
    const int vertNum = 16; //how many points
    const int vertSpace = 18; //distance between points
    const int baseSize = 6; //head size
    const float decreaseSize = .2; //how fast to decrease vert size

    const int eyeSpace = 90; //how far apart
    const int eyeRadius = 20; //eye size

    sf::Texture gradientTexture("../../src/Images/Gradient.png"); //gradient to be used in the metaball
    std::vector<sf::Sprite> c(vertNum, sf::Sprite(gradientTexture)); //list of vectors for fish part sprite
    for (int i = 0; i < vertNum; i++) { //vert properties
        c[i].setOrigin( sf::Vector2f(1024 * c[i].getScale().x / 2,1024 * c[i].getScale().y / 2) );
        c[i].setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));

        c[i].setScale(sf::Vector2f(baseSize * .1 / ((i*decreaseSize)+1.5), baseSize * .1 / ((i*decreaseSize)+1.5)));
        c[i].setColor(sf::Color::White);
    }

    sf::Vector2f eyesPosition; //fish eyes
    sf::CircleShape eyes{eyeRadius};
    eyes.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
    eyes.setFillColor(sf::Color(0, 92, 184,255));


    sf::Shader metaballShader; //threshold shader to make metaballs
    if (!metaballShader.loadFromFile("../../src/Shaders/metaball.vert","../../src/Shaders/metaball.frag"))
        cerr << "Could not load metaball shader" << endl;
    sf::RenderTexture metaballTexture{{1920u,1080u}}; //render texture to apply shader to
    metaballTexture.setSmooth(true);
    sf::Sprite metaballSprite(metaballTexture.getTexture());

    //Update__________________________________________________________________________________________________________________________________________________________________________
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        //distance constraint
        for (int i = 0; i < vertNum; i++) {
            if (i < vertNum - 1) {
                float desiredDistance = vertSpace; // how far apart for each vertex
                sf::Vector2f vector = c[i].getPosition()-c[i+1].getPosition(); //direction to next vertex
                float A = vector.x;
                float B = vector.y;
                float C = sqrt(A*A+B*B);
                sf::Vector2f normVector = sf::Vector2f(vector.x/C,vector.y/C); // pythagorean theorem for dist
                if (C == 0) //dividing by zero is BAD
                    normVector = sf::Vector2f(0,0);
                sf::Vector2f midPoint = sf::Vector2f((c[i].getPosition().x + c[i+1].getPosition().x)/2,
                                                    ( c[i].getPosition().y + c[i+1].getPosition().y)/2); //midpoint of verts

                c[i+1].setPosition(sf::Vector2f(midPoint.x-normVector.x*desiredDistance,
                                                midPoint.y-normVector.y*desiredDistance)); //apply distance constraint


                if(i==0)
                    eyesPosition = sf::Vector2f(normVector.x,normVector.y);
            }
        }
        

        c[0].setPosition((sf::Vector2f)sf::priv::InputImpl::getMousePosition()); //move head to mouse position

        //Draw__________________________________________________________________________________________________________________________________________________________________________
        window.clear(sf::Color(24, 38, 66,255)); //clear screen and metaball render texture
        metaballTexture.clear(sf::Color::Transparent); //reset metaballs

        for (int i = 0; i < vertNum; i++) { //draw fish to render texture
                metaballTexture.draw(c[i]);
        }
        metaballTexture.display(); //finish metaball render

        window.draw(metaballSprite, &metaballShader); //draw metaballs to screen

        //set eye to position for both sides--
        eyes.setPosition(sf::Vector2f(c[0].getPosition().x + eyesPosition.y * eyeSpace,c[0].getPosition().y + eyesPosition.x * -eyeSpace));
        window.draw(eyes);
        eyes.setPosition(sf::Vector2f(c[0].getPosition().x + eyesPosition.y * -eyeSpace,c[0].getPosition().y + eyesPosition.x * eyeSpace));
        window.draw(eyes);

        window.display();
    }
}
