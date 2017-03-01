#include "HUDSystem.h"


/// <summary>
/// Constructor to set up the hud for our game.
/// </summary>
/// <param name="window"></param>
HUDSystem::HUDSystem(sf::RenderWindow  *window)
{
	m_window = window;

	font.loadFromFile("./resources/fonts/akashi.ttf");
	
	m_currentPath.setPosition(100, 50);
	m_currentPath.setFont(font);
	m_currentPath.setCharacterSize(18);
	m_currentPath.setColor(sf::Color::Blue);
	m_txtTimeForLap.setPosition(600, 50);
	m_txtTimeForLap.setFont(font);
	m_txtTimeForLap.setCharacterSize(18);
	m_txtTimeForLap.setColor(sf::Color::Blue);
	std::cout << "Previous Lap Times: " << std::endl;
}
/// <summary>
/// Sucscribes us to the EvCurrentWaypoint event so we can read the current waypoint.
/// </summary>
/// <param name="events"></param>
void HUDSystem::configure(entityx::EventManager& events)
{
	events.subscribe<EvCurrentWaypoint>(*this);
}
/// <summary>
/// Receives the current waypoint heading when its updated.
/// </summary>
/// <param name="e"></param>
void HUDSystem::receive(const EvCurrentWaypoint &e)
{
	m_currentWaypoint = e.m_path;
	if (m_currentWaypoint == 0)
	{
		timeForLap = 0;
		std::cout << "   "+m_currentLapTime << std::endl;
	}
}
	
/// <summary>
/// Updates the elements in the Hud (Current waypoint txt & lap time)
/// </summary>
/// <param name="entities"></param>
/// <param name="events"></param>
/// <param name="dt"></param>
void HUDSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, double dt)
{
	m_currentPath.setString("Current Waypoint: " + std::to_string(m_currentWaypoint));
	timeForLap += dt/160;
	m_currentLapTime = "Lap Time: " + std::to_string((int)(timeForLap / 60)) + "m : " + std::to_string((int)timeForLap % 60) + "s : " + std::to_string((int)((timeForLap - (int)timeForLap) * 100)) + "ms";
	m_txtTimeForLap.setString(m_currentLapTime + "\n \n" + pastLaps);
	render();
}
/// <summary>
/// Renders the hud to the screen.
/// </summary>
void HUDSystem::render()
{
	m_window->draw(m_currentPath);
	m_window->draw(m_txtTimeForLap);
}