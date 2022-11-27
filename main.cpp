#include <SFML/Graphics.hpp>
#include <iostream>

#include "unit.h"
#include "map.h" //подключили код с картой
void runIntoWall(const float& diff, float& velocity, float& pos, Unit*& unit, const int& wallPos) {
    if (velocity > 0)
        pos -= diff;
    else if (velocity < 0)
        pos += diff;
    
    // стена слева, а скорость направо => не обнуляем скорость
    if (wallPos * velocity > 0)
        velocity = 0;

    unit->freeMovingTime = 0;
    std::cout << "errr 0" << std::endl;
}

void interactionWithMap(Unit* unit, Map* map)//ф-ция взаимодействия с картой
	{
         sf::Vector2f unitTopLeftPoint = unit->pos;
         sf::Vector2f unitBotRightPoint = unit->pos + unit->size;

         sf::Vector2f tileTopLeftPoint, tileBotRightPoint, diff;
         sf::Vector2f tilePoint, unitPoint, intersLen;

         int wallPos;
        // Массив2д в классе, 
        // Get all tiles that intersects with input) area
        //проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам. про условия читайте ниже.
        //std::cout << unitBotRightPoint.y / TILE_HEIGHT + 1 - (unitTopLeftPoint.y / TILE_HEIGHT - 1) << " " << 
        //unitBotRightPoint.x / TILE_WIDTH + 1 - unitTopLeftPoint.x / TILE_WIDTH << std::endl;
		for (int i = unitTopLeftPoint.y / TILE_HEIGHT; i <= unitBotRightPoint.y / TILE_HEIGHT; i++) // TODO: CHECK (float->int, +-1)
        	for (int j = unitTopLeftPoint.x / TILE_WIDTH; j <= unitBotRightPoint.x / TILE_WIDTH; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
                tileTopLeftPoint  = sf::Vector2f(j * TILE_WIDTH, i * TILE_HEIGHT);
                tileBotRightPoint = sf::Vector2f((j + 1) * TILE_WIDTH - 1, (i + 1) * TILE_HEIGHT - 1);
                //std::cout << j << " " << i << std::endl;
                //std::cout << unitTopLeftPoint.x << " " << unitTopLeftPoint.y << std::endl;
                //std::cout << tileTopLeftPoint.x << " " << tileTopLeftPoint.y << std::endl;
				if (map->TileMap[i][j] == '0' || map->TileMap[i][j] == 's')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
                    // if (unit->velocity.y > 0) check   bot point else check  top point
                    // if (unit->velocity.x > 0) check right point else check left point
                    unitPoint = unit->pos;

                    // find intersection length (in case of equal bounds. if < 0 => no intersection)
                    intersLen.x = std::min(tileBotRightPoint.x - unitPoint.x, (unitPoint.x + unit->size.x) - tileTopLeftPoint.x);
                    intersLen.y = std::min(tileBotRightPoint.y - unitPoint.y, (unitPoint.y + unit->size.y) - tileTopLeftPoint.y);

                    //std::cout << intersLen.x << " intersect " << intersLen.y << std::endl;
                    // firstly check larger intersection. Right moving -> diff x < diff y
                    if (intersLen.y > intersLen.x && intersLen.y > 0) {// the wall is on the right (left)
                        if (intersLen.x == tileBotRightPoint.x - unitPoint.x) // the wall is on the left
                            wallPos = -1;
                        else // the wall is on the right
                            wallPos = 1;

                        if (unit->velocity.x != 0) // TODO: approx equal
                            runIntoWall(intersLen.x, unit->velocity.x, unit->pos.x, unit, wallPos);  
                    }
                    else if (intersLen.x > 0) {// the wall is on top (bot)
                        if (intersLen.y == tileBotRightPoint.y - unitPoint.y) // the wall is on top
                            wallPos = -1;
                        else // the wall is on bot
                            wallPos = 1;

                        if (unit->velocity.y != 0)
                            runIntoWall(intersLen.y, unit->velocity.y, unit->pos.y, unit, wallPos);  
                    }
				}
 
				if (0 && map->TileMap[i][j] == 's') { //если символ равен 's' (камень)
					unit->pos = sf::Vector2f(450, 450); //какое то действие... например телепортация героя
                    unit->freeMovingTime = 0;
					map->TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}
			}
	}
int main()
{
    
    Unit my_unit(100, TILE_WIDTH * 4, TILE_HEIGHT * 3);

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!"); //, Style::Fullscreen
    window.setFramerateLimit(144);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    float dx, dy;

    sf::Clock clock;

    Map map("img/map.png");

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
 
		clock.restart();
		time = time / 3000;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);//забираем коорд курсора
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
        //std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
        //std::cout << pos.x << "\n";//смотрим на Х,которая преобразовалась в мировые координаты
        if (event.type == sf::Event::MouseButtonPressed)//если нажата клавиша мыши
            if (event.key.code == sf::Mouse::Left)//а именно левая
                if (my_unit.contains(mousePos.x, mousePos.y)) { //и при этом координата курсора попадает в спрайт
                    my_unit.leftClicked();
                }

        if (event.type == sf::Event::MouseButtonPressed)//если отпустили клавишу
			if (event.key.code == sf::Mouse::Right) { //а именно левую
                sf::Vector2f spriteTopLeftPixelDest = sf::Vector2f(mousePos.x - my_unit.getSize().x / 2, mousePos.y - my_unit.getSize().y / 2);
                if (my_unit.isSelected) {
                    my_unit.isSelected = false;
                    my_unit.sprite.setColor(sf::Color::White);//красим спрайт обратно

                    my_unit.startMove(spriteTopLeftPixelDest);
                }
            }
        interactionWithMap(&my_unit, &map);

        my_unit.update(window, time);

        window.clear();

        map.mapRender(&window);//рисуем квадратики на экран
        
        my_unit.draw(window);
        window.display();
    }

    return 0;
}

/*
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // если (Мышка::нажата клавиша(ЛКМ))
            {
            //if (IntRect(x, y, ширина x, высота y).contains(Mouse::getPosition(window))) { num = 1 }
            }
*/