#pragma once
#include "entityx/Event.h"
#include "entityx/Entity.h"
#include "utils/LevelLoader.h"

/// <summary>
/// The start game event.
/// </summary>
struct EvStartGame : public entityx::Event<EvStartGame>
{
	EvStartGame() {}
};


/// <summary>
/// The game initialise event.
/// </summary>
struct EvInit : public entityx::Event<EvInit>
{
	/// <summary>
	/// Constructs this event with the specified fade parameter.
	/// </summary>
	/// <param name="levelNr">the game level number</param>
	/// <param name="level">all the required data for this level</param>
	/// </summary>
	EvInit(int levelNr, LevelData const& level)
		: m_levelNr(levelNr)
		, m_level(level)
	{
	}

	int m_levelNr;
	LevelData const& m_level;
};

