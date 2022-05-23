#pragma once

#include "GameObject.h"

class StaticObject : public GameObject {

public:
    StaticObject(int,
                 std::unique_ptr<b2World> &,
                 const sf::Vector2f &,
                 const sf::Vector2f &,
                 b2BodyType);
};
