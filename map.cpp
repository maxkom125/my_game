#include <SFML/Graphics.hpp>
#include <iostream>

#include "map.h"

Map::Map(const std::string filename, const int mapWidth, const int mapHeight) {
    if (!this->mapImage.loadFromFile(filename))//загружаем файл для карты
        std::cout << "Error in map loading" << std::endl;

    this->texture.loadFromImage(this->mapImage);//заряжаем текстуру картинкой
    this->sprite.setTexture(this->texture); //заливаем текстуру спрайтом

    this->mapWidth  = mapWidth;
    this->mapHeight = mapHeight;

    //this->tileMap = new sf::String[this->mapWidth * this->mapHeight];
    mapInit();
}

void Map::mapRender(sf::RenderWindow& window) {
    /////////////////////////////Рисуем карту/////////////////////
    for (int i = 0; i < this->mapHeight; i++)
        for (int j = 0; j < this->mapWidth; j++)
        {
            if (tileMap[i][j] == ' ') this->sprite.setTextureRect(sf::IntRect(75, 8, this->tileWidth, this->tileHeight));//если встретили символ пробел, то рисуем 1й квадратик
            if (tileMap[i][j] == 's') this->sprite.setTextureRect(sf::IntRect(20, 8, this->tileWidth, this->tileHeight));//если встретили символ s, то рисуем 2й квадратик
            if (tileMap[i][j] == '0') this->sprite.setTextureRect(sf::IntRect(20, 8, this->tileWidth, this->tileHeight));//если встретили символ 0, то рисуем 3й квадратик

            this->sprite.setPosition(j * this->tileWidth, i * this->tileHeight);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
        
            window.draw(this->sprite);
        }
}

void Map::mapInit() {
    sf::String** map = new sf::String*[this->mapHeight];
    std::vector<sf::Vector2i> specialPoints = {sf::Vector2i(3, 3), sf::Vector2i(6, 5)};

    //top and bot bounds
    map[0] = new sf::String[this->mapWidth];
    map[this->mapHeight - 1] = new sf::String[this->mapWidth];
    for (int j = 0; j < this->mapWidth; j++){
        map[0][j] = '0';
        map[this->mapHeight - 1][j] = '0';
    }
    
    for (int i = 1; i < this->mapHeight - 1; i++) {
        map[i] = new sf::String[this->mapWidth];
        map[i][0]                  = '0'; //left bound
        map[i][this->mapWidth - 1] = '0'; //right bound

        for (int j = 1; j < this->mapWidth - 1; j++) {
            map[i][j] = ' ';
        }
    }

    for (sf::Vector2i specialPoint : specialPoints){
        map[specialPoint.x][specialPoint.y] = "s";
    }
    this->tileMap = map;
}
