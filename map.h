#include <SFML/Graphics.hpp>
#include <iostream>
//#ifndef MAP_H_
//#define MAP_H_


class Map {
	public:
		//Map(const std::string &filename);
		Map(const std::string filename, const int mapWidth, const int mapHeight);

        sf::Texture texture;
        sf::Sprite sprite;

        sf::Image mapImage;//объект изображения для карты

        int mapHeight = 25; //tiles
        int mapWidth = 40; //tiles

        int tileWidth = 50;
        int tileHeight = 68;

		sf::String** tileMap;
        
		void mapRender(sf::RenderWindow& window);
        void mapInit();
};
//#endif