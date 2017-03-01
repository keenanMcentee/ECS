#include "entityx/System.h"
#include "SFML\Graphics.hpp"
#include "components/Ai.h"
#include <iostream>

/*
@Author: Keenan McEntee
@Date Created: 28/2/2017
@Description: Hud System for our tank game that displays the current waypoint and the time of the current lap.
*/
class HUDSystem : public entityx::System<HUDSystem>
	, public entityx::Receiver<HUDSystem>
{
public:
	HUDSystem(sf::RenderWindow *window);
	
	void configure(entityx::EventManager& events);

	void receive(const EvCurrentWaypoint& e);

	void update(entityx::EntityManager &entities,
		entityx::EventManager &events,
		double dt);

	void render();

	int m_currentWaypoint;

private:
	sf::RenderWindow *m_window;
	sf::Font font;
	sf::Text m_currentPath;
	sf::Text m_txtTimeForLap;

	double timeForLap;
	bool newLap;
	std::string m_currentLapTime;
	std::string pastLaps;
};

