#include "HUDSystem.h"



HUDSystem::HUDSystem()
{
}


HUDSystem::~HUDSystem()
{
}
void HUDSystem::configure(entityx::EventManager& events)
{
	events.subscribe<entityx::ComponentAddedEvent<EvCurrentWaypoint>>(*this);
}

void HUDSystem::receive(const EvCurrentWaypoint& e)
{
	m_currentWaypoint = e.m_path;
}