#include <iostream>
#include <cmath>

#include "unit.h"
#include "map.h"

#define DEFAULT_VELOCITY 0.5
#define DEST_EPS 2
#define FREE_MOVING_TIME_PERCENT 0.7
#define DEST_LINE_WIDTH 10

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

bool approximatelyEqual(float a, float b, float epsilon) {
    return fabs(a - b) <= epsilon;
}

bool EqualVector2f(sf::Vector2f v1, sf::Vector2f v2) {
    float eps = DEST_EPS;
    return approximatelyEqual(v1.x, v2.x, eps) && approximatelyEqual(v1.y, v2.y, eps);
}

float vector2fLen(const sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float getAngle(const sf::Vector2f v) { // angle coord in radians of the vector
    float angle; // angle in radians        
    if (!approximatelyEqual(v.x, 0, 1)) //approx
        angle = std::atan2(v.y, v.x);
    else 
        angle = v.y > 0 ? 90 * 3.14159265 / 180 : 270 * 3.14159265 / 180;
    
    return angle;
}

Unit::Unit(float size, float pos_x, float pos_y) {
    this->size = sf::Vector2f(size, size);
    this->pos = sf::Vector2f(pos_x, pos_y);

    this->velocity = sf::Vector2f(pos_x, pos_y);

    this->isMoving = 0;
    this->isSelected = 0;

    this->freeMovingTime = 0;

    this->lineToDest = new sf::RectangleShape(sf::Vector2f(1, DEST_LINE_WIDTH));

    if (!_texture.loadFromFile("img/rect.png")) {
        std::cout << "Error in img loading" << std::endl;
    }

    sprite.setTexture(_texture);

    sf::FloatRect initial_fig = sprite.getGlobalBounds();
    sprite.scale(this->size.x / initial_fig.width, this->size.y / initial_fig.height);

    sprite.setPosition(pos_x, pos_y); 
    //_sprite.setColor(sf::Color::Blue); //setFillColor(sf::Color::Green);
}

Unit::~Unit() {        
    delete lineToDest;
}

void Unit::reSize(sf::Vector2f new_size) {
    sprite.scale(new_size.x / this->size.x, new_size.y / this->size.y);
    this->size = new_size;
}

sf::Vector2f Unit::getSize() {
    return this->size;
}

bool Unit::contains(float x, float y) {
    return sprite.getGlobalBounds().contains(x, y);
}

void Unit::setPos(float new_pos_x, float new_pos_y) {
    this->pos.x = new_pos_x;
    this->pos.y = new_pos_y;
}

void Unit::startMove(sf::Vector2f destPos) {//dest pos for top left pixel of sprite
    this->lineToDest->setFillColor(sf::Color::Red);
    this->dest_pos = destPos;
    this->reCalcMove();
}

void Unit::reCalcMove() {
    //dest pos -> top left pixel of sprite
    sf::Vector2f destVector = this->dest_pos - this->sprite.getPosition();
    float distance = vector2fLen(destVector);

    float angle = getAngle(destVector);

    this->velocity.x = DEFAULT_VELOCITY * std::cos(angle);
    this->velocity.y = DEFAULT_VELOCITY * std::sin(angle);

    this->freeMovingTime = distance / DEFAULT_VELOCITY * FREE_MOVING_TIME_PERCENT;

    this->isMoving = true; //можем двигать спрайт
    this->lineToDest->setRotation(angle * 180 / 3.14159265);	
    //std::cout << "isMoving!" << std::endl;//выводим в консоль сообщение об этом	
}

void Unit::stopMove() {
    this->isMoving = false; //не можем двигать спрайт	
    //this->sprite.setColor(sf::Color::White);//и даем ему прежний цвет
    this->velocity.x = 0;
    this->velocity.y = 0;

    this->dest_pos = sf::Vector2f();

    //std::cout << "Stopped!" << std::endl;//выводим в консоль сообщение об этом	
}

void Unit::updatePos(float time) {
    this->pos.x += this->velocity.x * time;
    this->pos.y += this->velocity.y * time;
    sprite.setPosition(this->pos.x, this->pos.y);

    if (EqualVector2f(this->pos, this->dest_pos))
        this->stopMove();
    
    sf::Vector2f spriteCenter = this->pos + sf::Vector2f(this->size.x / 2, this->size.y / 2);
    sf::Vector2f initial_fig = this->lineToDest->getSize();

    this->lineToDest->setPosition(spriteCenter);
    this->lineToDest->setScale(vector2fLen(this->dest_pos - pos) / initial_fig.x, 1); // От центра до курсора, стандартная ширина
    
    this->freeMovingTime--;
    if (this->freeMovingTime <= 0 && this->isMoving) 
        this->reCalcMove();
}

void Unit::update(sf::RenderWindow& window, const float time)
{	
    if (this->isMoving)
        Unit::updatePos(time);

    if (this->isSelected) { // initialize lineToDest (line to cursor position)
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);//забираем коорд курсора
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

        sf::Vector2f spriteCenter = this->sprite.getPosition() + sf::Vector2f(this->size.x / 2, this->size.y / 2);
        sf::Vector2f destVector = mousePos - spriteCenter; //делаем разность между позицией курсора и спрайта.для корректировки нажатия

        // x^2 + y^2 = c^2
        float angle = getAngle(destVector) * 180 / 3.14159265;

        sf::Vector2f initial_fig = this->lineToDest->getSize();

        this->lineToDest->setPosition(spriteCenter);
        this->lineToDest->setScale(vector2fLen(destVector) / initial_fig.x, 1); // От центра до курсора, стандартная ширина
        this->lineToDest->setRotation(angle);
    }
    //sprite.setOrigin(w / 2, h / 2);
    //interactionWithMap();
    //if (health <= 0){ life = false; }   
}

void Unit::leftClicked() {
    std::cout << "left clicked" << std::endl;
    this->isSelected = !this->isSelected;
    this->sprite.setColor(this->isSelected ? sf::Color::Green : sf::Color::White);
    this->lineToDest->setFillColor(this->isSelected ? sf::Color::Green : sf::Color::White);
}

void Unit::draw(sf::RenderWindow& window) {
    window.draw(this->sprite);
    if (this->isSelected || this->isMoving)
        window.draw(*(this->lineToDest));
}
    