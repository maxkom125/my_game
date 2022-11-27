#include <SFML/Graphics.hpp>
#include <iostream>
//#ifndef MAP_H_
//#define MAP_H_

#define MAP_HEIGHT 25 //tiles
#define MAP_WIDTH 40 //tiles

#define TILE_WIDTH 50 //tiles
#define TILE_HEIGHT 68 //tiles

//TODO: Проверять пересечения обьектов на карте друг с другом и с картой (тут или в мэйне)
class Map {
	public:
		//Map(const std::string &filename);
		Map(const std::string filename);

        sf::Texture texture;
        sf::Sprite sprite;

		sf::String TileMap[MAP_HEIGHT] = {
        "0000000000000000000000000000000000000000",
        "0                                      0",
        "0  s                                   0",
        "0                                      0",
        "0     s                                0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0000000000000000000000000000000000000000",
    };

		void mapRender(sf::RenderWindow* window);

		//void mapRender(sf::RenderWindow* window);
};
//#endif