#include "Data.h"
#include <algorithm>

//__________
Data::Data() {
    setWorld();
    m_map.readMapFile(*this);
    setCarsPlace();
}

//__________________
void Data::setWorld() {
    //set gravity
    b2Vec2 gravity(0.0f, 10.f);
    m_world = std::make_unique<b2World>(gravity);
}

//______________________
void Data::setWorldStep() {
    m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);
}

//__________________________________
void Data::setObject(std::string &name,
                     const sf::Vector2f &position,
                     const float rotation) {
    if (FactoryObject<MovingObject>::checkIfNameInMap(name)) {
        m_moving.push_back(FactoryObject<MovingObject>::create(m_world, name, position, rotation));
        return;
    }
    if (FactoryObject<StaticObject>::checkIfNameInMap(name)) {
        m_static.push_back(FactoryObject<StaticObject>::create(m_world, name, position, rotation));
        return;
    }
}

//______________________
void Data::setCarsPlace() {
    int carPlace = carPLaceStart;
    std::vector<float> places;

    for (auto &moving: m_moving)
        places.push_back(moving->getPosition().x);

    std::sort(places.begin(), places.end());

    for (auto &place: places) {
        for (auto &moving: m_moving)
            if (moving->getPosition().x == place) {
                moving->setCarPlace(carPlace);
                break;
            }
        carPlace--;
    }
}

//____________________________________________
void Data::moveUserCar(const sf::Event &event) {
    m_moving[User]->move(event);
}

//_________________________________________________
void Data::moveComputerCars(const sf::Event &event) {
    for (auto &moving: m_moving)
        if (moving != m_moving[User])
            moving->move(event);
}

//______________________
void Data::removeObjects() {
    for (auto &moving: m_moving)
        if (moving->getObjectDead())
            moving->destroyBody();

    std::erase_if(m_moving, [](auto &moving) { return moving->getObjectDead(); });

    for (auto &statics: m_static)
        if (statics->getObjectDead())
            statics->destroyBody();

    std::erase_if(m_static, [](auto &statics) { return statics->getObjectDead(); });
}

//____________________________________________________________________________
void Data::drawData(sf::RenderWindow &window, const unique_ptr<GameMenu> &menu) {

    setView(window, 3500.0f, 2000.0f);

    for (auto &moving: m_moving) {
        moving->updateObjects();
        moving->draw(window);
    }

    for (auto &statics: m_static) {
        statics->updateObjects();
        statics->draw(window);
    }

    menu->getButton(InGamePause)->updatePos(Vector2f(getUserPosition().x + 2000, 50));
    menu->getButton(InGameMusic)->updatePos(Vector2f(getUserPosition().x + 100, 800));
    menu->getButton(InGameHome)->updatePos(Vector2f(getUserPosition().x + 250, 800));
    menu->getButton(InGamePlay)->updatePos(Vector2f(getUserPosition().x + 400, 800));
}

//__________________________________________
void Data::setView(sf::RenderWindow &window, float width, float height) const {
    auto view = window.getView();
    view.setCenter(getUserPosition().x + 500, 1000);
    view.setSize(width, height);
    window.setView(view);
}