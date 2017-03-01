#include "entityx/System.h"
#include "entityx/Event.h"
#include "systems/Events.h"
#include "components/Ai.h"
class HUDSystem : public entityx::System<HUDSystem>
	, public entityx::Receiver<HUDSystem>
{
public:
	HUDSystem();
	
	void configure(entityx::EventManager& events);

	void receive(const EvCurrentWaypoint& e);

	void update(entityx::EntityManager &entities,
		entityx::EventManager &events,
		double dt);

	int m_currentWaypoint;
};

