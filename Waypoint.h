#pragma once
#include "entityx/Entity.h"

enum class WaypointType
{
	WaypointDirection,
};

struct Waypoint : public entityx::Component<Waypoint>
{
	Waypoint(WaypointType type = WaypointType::WaypointDirection);

	WaypointType m_type;
};
