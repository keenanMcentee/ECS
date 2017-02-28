#pragma once

#include "entityx/Entity.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML\Graphics\CircleShape.hpp>


struct Display : public entityx::Component<Display>
{
   Display();

   Display(const sf::IntRect& coord);

   Display(const sf::IntRect& coord, sf::Color color);

   Display(const sf::CircleShape& circle);

   sf::IntRect m_coord;
   sf::CircleShape m_circle;
   sf::Color m_color;
};

