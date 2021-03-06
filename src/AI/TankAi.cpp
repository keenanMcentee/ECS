#include "ai/TankAi.h"

/// <summary>
/// Base constructor for the tank AI.
/// </summary>
/// <param name="obstacles"></param>
/// <param name="waypoints"></param>
/// <param name="id"></param>
TankAi::TankAi(std::vector<sf::CircleShape> const & obstacles, std::vector<sf::CircleShape> const & waypoints, entityx::Entity::Id id)
  : m_aiBehaviour(AiBehaviour::PATH_FOLLOWING)
  , m_steering(0,0)
  , m_obstacles(obstacles)
	, m_waypoints(waypoints)
{
}
/// <summary>
/// Update function for moving the TankAI towards its target and also make sure it doesn't collide with a wall.
/// </summary>
/// <param name="playerId"></param>
/// <param name="aiId"></param>
/// <param name="entities"></param>
/// <param name="dt"></param>
/// <param name="events"></param>
void TankAi::update(entityx::Entity::Id playerId,
	entityx::Entity::Id aiId,
	entityx::EntityManager& entities,
	double dt, entityx::EventManager &events)
{
	entityx::Entity aiTank = entities.get(aiId);
	Motion::Handle motion = aiTank.component<Motion>();
	Position::Handle position = aiTank.component<Position>();

	sf::Vector2f vectorToPlayer = seek(playerId,
		aiId,
		entities);

	sf::Vector2f vectorToNode = seek(aiId, entities);

	switch (m_aiBehaviour)
	{
	case AiBehaviour::SEEK_PLAYER:
		m_steering += thor::unitVector(vectorToPlayer);
		m_steering += collisionAvoidance(aiId, entities);
		m_steering = Math::truncate(m_steering, MAX_FORCE);
		m_velocity = Math::truncate(m_velocity + m_steering, MAX_SPEED);

		break;
	case AiBehaviour::STOP:
		motion->m_speed = 0;

	case AiBehaviour::PATH_FOLLOWING:
		m_steering += thor::unitVector(vectorToNode);
		m_steering += collisionAvoidance(aiId, entities);
		m_steering = Math::truncate(m_steering, MAX_FORCE);
		m_velocity = Math::truncate(m_velocity + m_steering, MAX_SPEED);
		break;
	default:
		break;
	}

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / thor::Pi * 180 + 180; 

	auto currentRotation = position->m_rotation;	

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}
	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		position->m_rotation = static_cast<int>((position->m_rotation) + 1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		position->m_rotation = static_cast<int>((position->m_rotation) - 1) % 360;
	}

	if (m_aiBehaviour != AiBehaviour::PATH_FOLLOWING)
	{
		if (thor::length(vectorToPlayer) < MAX_SEE_AHEAD)
		{
			m_aiBehaviour = AiBehaviour::STOP;
		}
		else
		{
			motion->m_speed = thor::length(m_velocity);
			m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
		}
	}
	else
	{
		if (thor::length(vectorToNode) < m_waypoints.at(currentNode).getRadius())
		{
			if (currentNode < 20)
			{
				currentNode += 1;
				events.emit<EvCurrentWaypoint>(currentNode);
			}
			else
			{
				currentNode = 0;
				events.emit<EvCurrentWaypoint>(currentNode);
			}


		}
		else
		{
			motion->m_speed = thor::length(m_velocity);
			m_aiBehaviour = AiBehaviour::PATH_FOLLOWING;
		}
	}
}
/// <summary>
/// Function that handles setting the heading of the tank towards the player.
/// </summary>
/// <param name="playerId"></param>
/// <param name="aiId"></param>
/// <param name="entities"></param>
/// <returns></returns>
sf::Vector2f TankAi::seek(entityx::Entity::Id playerId,
						  entityx::Entity::Id aiId,
	                      entityx::EntityManager& entities) const  
{
	entityx::Entity aiTank = entities.get(aiId);
	entityx::Entity playerTank = entities.get(playerId);
	sf::Vector2f aiPos = aiTank.component<Position>()->m_position;
	sf::Vector2f playerPos = playerTank.component<Position>()->m_position;
	sf::Vector2f returnVect = playerPos - aiPos;
	return returnVect;

}

/// <summary>
/// Function to set the heading of the tank toward the currently selected waypoint.
/// </summary>
/// <param name="aiId"></param>
/// <param name="entities"></param>
/// <returns></returns>
sf::Vector2f TankAi::seek(entityx::Entity::Id aiId,
	entityx::EntityManager& entities) const
{
	entityx::Entity aiTank = entities.get(aiId);
	sf::Vector2f aiPos = aiTank.component<Position>()->m_position;
	sf::Vector2f nodePos = m_waypoints.at(currentNode).getPosition();
	sf::Vector2f returnVect = nodePos - aiPos;
	return returnVect;

}
/// <summary>
/// Function that handles avoiding all obstacles.
/// </summary>
/// <param name="aiId"></param>
/// <param name="entities"></param>
/// <returns></returns>
sf::Vector2f TankAi::collisionAvoidance(entityx::Entity::Id aiId, 
									    entityx::EntityManager& entities)
{
	entityx::Entity aiTank = entities.get(aiId);
	Position::Handle aiPos = aiTank.component<Position>();	

	auto headingRadians = thor::toRadian(aiPos->m_rotation);	
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = aiPos->m_position + headingVector;	
	m_halfAhead = aiPos->m_position + (headingVector * 0.5f);
	const sf::CircleShape mostThreatening = findMostThreateningObstacle(aiId, entities);
	sf::Vector2f avoidance(0, 0);
	if (mostThreatening.getRadius() != 0.0)
	{
		auto threatPos = mostThreatening.getPosition();
		auto mypos = aiPos->m_position;
		avoidance.x = m_ahead.x - mostThreatening.getPosition().x;
		avoidance.y = m_ahead.y - mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{
		avoidance *= 0.0f;
	}
    return avoidance;
}

/// <summary>
/// Function that finds the closest obstacle and sets the tank on the right path to avoid it.
/// </summary>
/// <param name="aiId"></param>
/// <param name="entities"></param>
/// <returns></returns>
const sf::CircleShape TankAi::findMostThreateningObstacle(entityx::Entity::Id aiId,
															     entityx::EntityManager& entities) 
{		
	sf::CircleShape mostThreatening;
	mostThreatening.setRadius(0);
	float distToClosest = Math::distance(entities.get(aiId).component<Position>()->m_position, m_obstacles[0].getPosition());
	for (auto & obstacle : m_obstacles)
	{
		if (Math::lineIntersectsCircle(m_ahead, m_halfAhead, obstacle))
		{
			float distance = Math::distance(entities.get(aiId).component<Position>()->m_position, obstacle.getPosition());
			if (mostThreatening.getRadius() == 0 || distance < distToClosest)
			{
				distToClosest = distance;
				mostThreatening = obstacle;
			}

		}
	}
	
	return mostThreatening;
}

