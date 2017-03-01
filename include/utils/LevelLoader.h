#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"
#include "ResourcePath.hpp"

struct ObstacleData
{
  std::string m_type;
  sf::Vector2f m_position;
  double m_rotation;
};
struct WaypointData
{
	std::string m_type;
	sf::Vector2f m_position;
	float m_radius;
};

struct BackgroundData
{
   std::string m_fileName;
};

struct TankData
{
  sf::Vector2f m_position;
  int m_maxProjectiles;
  int m_reloadTime;
};

struct ProjectileData
{		
	double m_speed;
	int m_damage;
};

struct LevelData
{
   BackgroundData m_background;
   TankData m_tank;
   TankData m_aiTank;
   ProjectileData m_projectile;
   std::vector<WaypointData> m_waypoint;
   std::vector<ObstacleData> m_obstacles;
};

class LevelLoader
{
public:

   LevelLoader();

   static bool load(int nr, LevelData& level);
};
