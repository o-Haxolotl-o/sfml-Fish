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

    const int finVert = 3;
    const int finSpace = 128;
    const float finSize = .8;

    sf::Texture gradientTexture("../../src/Images/Gradient.png"); //gradient to be used in the metaball
    std::vector<sf::Sprite> c(vertNum, sf::Sprite(gradientTexture)); //list of vectors for fish part sprite
    for (int i = 0; i < vertNum; i++) { //vert properties
        c[i].setOrigin( sf::Vector2f(gradientTexture.getSize().x * c[i].getScale().x / 2,gradientTexture.getSize().y * c[i].getScale().y / 2) );
        c[i].setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
        c[i].setScale(sf::Vector2f(baseSize * .1 / ((i*decreaseSize)+1.5), baseSize * .1 / ((i*decreaseSize)+1.5)));
        c[i].setColor(sf::Color((i+1)*15,(i+1)*5,(i+1)*15));
    }

    sf::Texture tailTexture("../../src/Images/Tail.png"); //tail sprite to be used in the metaball
    c[vertNum-1].setTexture(tailTexture);
    c[vertNum-1].scale(sf::Vector2f(baseSize*.5,baseSize*.5));

    sf::Shader metaballShader; //threshold shader to make metaballs
    if (!metaballShader.loadFromFile("../../src/Shaders/metaball.vert","../../src/Shaders/metaball.frag"))
        cerr << "Could not load metaball shader" << endl;
    sf::RenderTexture metaballTexture{{1920u,1080u}}; //render texture to apply shader to
    metaballTexture.setSmooth(true); //doesn't seem to do anything bc of the shader
    sf::Sprite metaballSprite(metaballTexture.getTexture());

    sf::Vector2f eyesVector; //fish eyes
    sf::CircleShape eyes{eyeRadius};
    eyes.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
    eyes.setFillColor(sf::Color(0, 92, 184,255));

    sf::Vector2f finsVector; //fish fins
    sf::Texture finTexture("../../src/Images/Fin.png");
    sf::Sprite fins(finTexture);
    fins.setColor(sf::Color::Cyan);
    fins.setOrigin(sf::Vector2f(finTexture.getSize().x/2, finTexture.getSize().y/2));
    fins.setPosition(sf::Vector2f(1000, 500));


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
        c[0].setPosition((sf::Vector2f)sf::priv::InputImpl::getMousePosition()); //move head to mouse position
        //distance constraint
        for (int i = 0; i < vertNum; i++) {
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
                    eyesVector = sf::Vector2f(normVector.x,normVector.y);
                if(i==finVert)
                    finsVector = sf::Vector2f(normVector.x,normVector.y);
                if(i==vertNum-1)
                    c[i].setRotation(sf::radians( atan2(normVector.y,normVector.x)+1.5));
        }
        


        //Draw__________________________________________________________________________________________________________________________________________________________________________
        window.clear(sf::Color(24, 38, 66,255)); //clear screen and metaball render texture
        metaballTexture.clear(sf::Color::Transparent); //reset metaballs

        for (int i = 0; i < vertNum; i++) { //draw fish to render texture
            metaballTexture.draw(c[i]);
        }

        //set fin to position for both sides & draw--
        fins.setRotation(sf::radians( atan2(finsVector.y,finsVector.x)+1) );

        fins.setScale(sf::Vector2f(finSize,finSize));
        fins.setPosition(sf::Vector2f(c[finVert].getPosition().x + finsVector.y * finSpace,c[finVert].getPosition().y + finsVector.x * -finSpace));
        metaballTexture.draw(fins);

        fins.setRotation(sf::radians( atan2(finsVector.y,finsVector.x)-4) );
        fins.setScale(sf::Vector2f(-finSize,finSize));
        fins.setPosition(sf::Vector2f(c[finVert].getPosition().x + finsVector.y * -finSpace,c[finVert].getPosition().y + finsVector.x * finSpace));
        metaballTexture.draw(fins);
        //------------------------------------

        metaballTexture.display(); //finish metaball render

        window.draw(metaballSprite, &metaballShader); //draw metaballs to screen

        //set eye to position for both sides--
        eyes.setPosition(sf::Vector2f(c[0].getPosition().x + eyesVector.y * eyeSpace,c[0].getPosition().y + eyesVector.x * -eyeSpace));
        window.draw(eyes);
        eyes.setPosition(sf::Vector2f(c[0].getPosition().x + eyesVector.y * -eyeSpace,c[0].getPosition().y + eyesVector.x * eyeSpace));
        window.draw(eyes);
        //------------------------------------


        window.display();
    }
}
