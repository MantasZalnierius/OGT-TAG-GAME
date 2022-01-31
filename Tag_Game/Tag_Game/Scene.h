#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include "GameStates.h"

/// <summary>
/// Repersents a Scene Abstraction.
/// </summary>
class Scene
{
public:

	/// <summary>
	/// Constructs the base class with a Game State of the next scene.
	/// </summary>
	/// <param name="t_nextGameState">The next scene's Game State.</param>
	Scene(GameState t_nextGameState);

	/// <summary>
	/// Default destructor for the Scene. 
	/// </summary>
	virtual ~Scene() { }

	/// <summary>
	/// Gets a boolean to check if the Scene has ended.
	/// </summary>
	/// <returns>Boolean to check if a Scene has ended.</returns>
	virtual const bool& isEnded() const;

	/// <summary>
	/// Gets the next Game State after a Scene.
	/// </summary>
	/// <returns></returns>
	virtual const GameState& getNewGameState() const;

	virtual int getServerOrClient() const;

	/// <summary>
	/// Updates a Scene.
	/// </summary>
	virtual void update(float t_deltaTime) = 0;

	/// <summary>
	/// Renders a Scene.
	/// </summary>
	virtual void render(sf::RenderWindow& t_window) = 0;

	/// <summary>
	/// Handles the Process Events for the Scene.
	/// </summary>
	/// <param name="t_event">New event.</param>
	virtual void processEvent(sf::Event t_event, sf::RenderWindow& t_window) = 0;

protected:
	bool m_isEnded;
	int m_hostOrPlayer = 0;
	GameState m_nextGameState;

};
#endif