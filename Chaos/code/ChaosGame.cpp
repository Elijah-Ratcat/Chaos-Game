// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include <SFML/Graphics/ConvexShape.hpp>
#include <random>

// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main()
{
    // constant variables
    const string name_of_the_game = "Chaos Game";
    const string font_name = "CloisterBlack.ttf";

    // Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, name_of_the_game, Style::Default);

    vector<Vector2f> vertices;
    vector<Vector2f> points;

    //For handling a right click and creation of shape
    bool create = false;
    bool outShape = false;

    int last_vertex = 0;

    // Loading font
    Font font;
    if (!font.loadFromFile(font_name))
    {
        cout << "Error laoding " << font_name << endl; // error...
    }

    // Setting text element
    Text text;
    text.setFont(font);
    text.setString("    Click any 3+ points to form a shape");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::White);

    // Setting triangle
    ConvexShape shape;
    shape.setOutlineColor(Color::White);
    shape.setOutlineThickness(1);
    shape.setFillColor(Color::Transparent);


	while (window.isOpen())
	{
        /*
		****************************************
		Handle the players input
		****************************************
		*/
        Event event;
		while (window.pollEvent(event))
		{
            if (event.type == Event::Closed)
            {
				// Quit the game when the window is closed
				window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the left button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

                    if(!create)
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    else if(points.size() == 0)
                    {
                        ///fourth click
                        ///push back to points vector
                        points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                if (vertices.size() >= 3) create = true;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
        /*
		****************************************
		Update
		****************************************
		*/

        if(vertices.size() >= 3 && !outShape) text.setString("continue adding points or right click to set shape");
        else if(outShape) text.setString("click any point inside the shape to start the Chaos Game");

        //create shape
        if (create && !outShape)
        {
            shape.setPointCount(vertices.size());
            for (int point = 0; point < vertices.size(); point++)
            {
                shape.setPoint(point, vertices[point]);
            }
            outShape = true;
        }

        if(points.size() > 0) //active first click after right click
        {
            for (int i = 0; i < 10; i++) // Generate points 10 times a frame for speed
            {
                ///generate more point(s)
                ///select random vertex
                ///calculate midpoint between random vertex and the last point in the vector
                ///push back the newly generated coord.
                
                int random_vertex = rand() % vertices.size();

                if (vertices.size() > 3) //adding no sequential repeat restriction to make interesting shapes on 4+ count shpaes
                {
                    while (random_vertex == last_vertex)    
                    {
                        random_vertex = rand() % vertices.size();
                    }
                    last_vertex = random_vertex;
                }

                int midpointX = (vertices[random_vertex].x + points.back().x) / 2;
                int midpointY = (vertices[random_vertex].y + points.back().y) / 2;
                points.push_back(Vector2f(midpointX, midpointY));
            }
        }

        /*
		****************************************
		Draw
		****************************************
		*/
        window.clear();
        srand(0);
        for(int i = 0; i < points.size(); i++)
        {
            RectangleShape rect(Vector2f(1,1));
            rect.setPosition(Vector2f(points[i].x, points[i].y));
            rect.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
            window.draw(rect);
        }

        for (int i = 0; i < vertices.size(); i++)
        {
            RectangleShape rect(Vector2f(5, 5));
            rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
            rect.setFillColor(Color::Red);
            window.draw(rect);
        }

        window.draw(text);
        if (outShape) window.draw(shape);
        window.display();
    }
}
