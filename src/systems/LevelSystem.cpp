#include "systems/LevelSystem.h"


LevelSystem::LevelSystem(entityx::EntityManager& entities,
						 entityx::EventManager& eventManager)
: m_entityManager(entities)
, m_eventManager(eventManager)
{
}

void LevelSystem::configure(entityx::EventManager &eventManager)
{
   eventManager.subscribe<EvInit>(*this);
}

void LevelSystem::update(entityx::EntityManager& entities,
						 entityx::EventManager& events,
                         double dt)
{
 

}

void LevelSystem::receive(const EvInit& e)
{
   
   BackgroundCreator(e.m_level.m_background.m_fileName).create(m_entityManager.create());

   entityx::Entity tankBaseEntity = m_entityManager.create();

   TankBaseCreator(e.m_level.m_tank.m_position, false).create(tankBaseEntity);

   TurretCreator(e.m_level.m_tank, tankBaseEntity, false).create(m_entityManager.create());
   
}

