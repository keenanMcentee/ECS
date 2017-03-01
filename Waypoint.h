#pragma once
#include "entityx/Entity.h"

enum class WaypointType
{
	WaypointNode,
};

struct Waypoint : public entityx::Component<Waypoint>
{
	Waypoint(WaypointType type = WaypointType::WaypointNode);

	WaypointType m_type;
};
