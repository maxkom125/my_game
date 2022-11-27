#include <SFML/Graphics.hpp>
#include <iostream>

#include "map.h"

Map::Map(const std::string filename) {
    sf::Image map_image;//объект изображения для карты
    if (!map_image.loadFromFile(filename))//загружаем файл для карты
        std::cout << "Error in map loading" << std::endl;

    this->texture.loadFromImage(map_image);//заряжаем текстуру картинкой
    this->sprite.setTexture(this->texture); //заливаем текстуру спрайтом
}

void Map::mapRender(sf::RenderWindow* window) {
    /////////////////////////////Рисуем карту/////////////////////
    for (int i = 0; i < MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (TileMap[i][j] == ' ') this->sprite.setTextureRect(sf::IntRect(75, 8, TILE_WIDTH, TILE_HEIGHT)); //если встретили символ пробел, то рисуем 1й квадратик
            if (TileMap[i][j] == 's') this->sprite.setTextureRect(sf::IntRect(20, 8, TILE_WIDTH, TILE_HEIGHT));//если встретили символ s, то рисуем 2й квадратик
            if (TileMap[i][j] == '0') this->sprite.setTextureRect(sf::IntRect(20, 8, TILE_WIDTH, TILE_HEIGHT));//если встретили символ 0, то рисуем 3й квадратик


            this->sprite.setPosition(j * 50, i * 68);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
        
        (*window).draw(this->sprite);
        }
}
