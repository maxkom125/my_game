#include <SFML/Graphics.hpp>

class Unit {
    private:

        sf::Texture _texture;
    public:
        bool isMoving;
        bool isSelected;
        float freeMovingTime;

        sf::Vector2f size;
        sf::Vector2f pos;
        sf::Vector2f velocity;
        sf::Vector2f dest_pos;

        sf::RectangleShape* lineToDest;

        sf::Sprite sprite;

        Unit(float size, float pos_x, float pos_y);

        virtual ~Unit();
        
        void update(sf::RenderWindow& window, const float time);

        void reSize(sf::Vector2f new_size);

        void updatePos(float time);

        void setPos(float new_pos_x, float new_pos_y);

        void newVelocity(float velocity_x, float velocity_y);

        void reCalcMove();

        void startMove(sf::Vector2f destPos);

        void stopMove();

        sf::Vector2f getSize();

        bool contains(float x, float y);

        void leftClicked();

        void draw(sf::RenderWindow& window);

        //void interactionWithMap(sf::Vector2f topLeftPoint, sf::Vector2f BotRightPoint);//ф-ция взаимодействия с картой
};