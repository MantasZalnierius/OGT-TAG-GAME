#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "Player.h"
#include "Client.h"
#include "Server.h"
#include "PacketStructs.h"
#include "GameStates.h"
#include "ConnectionType.h"
#include "MainMenuScene.h"

class Game
{
public:

	/// <summary>
	/// default constructor
	/// setup the window properties
	/// load and setup the text 
	/// load and setup the image
	/// </summary>
	Game();

	/// <summary>
	/// default destructor we didn't dynamically allocate anything
	/// so we don't need to free it, but mthod needs to be here
	/// </summary>
	~Game();

	/// <summary>
	/// main game loop
	/// update 60 times per second,
	/// process update as often as possible and at least 60 times per second
	/// draw as often as possible but only updates are on time
	/// if updates run slow then don't render frames
	/// </summary>
	void run();

	// HELPER FUNCTIONS FOR UPDATING CLINETS/PLAYERS.
	void init(Client& t_client);
	void HandleCollisions();
	static void ListenForNewConnectionThread(Server& t_server);
	void setUpPlayerID(int t_ID);
	void setPlayerPosition(int t_id, sf::Vector2f t_pos);
	void tagPlayer(int idToTag);
	void resetPlayers();
	// HELPER FUNCTIONS FOR UPDATING CLINETS/PLAYERS.

private:

	/// <summary>
	/// handle user and system events/ input
	/// get key presses/ mouse moves etc. from OS
	/// and user :: Don't do game update here
	/// </summary>
	void processEvents();

	/// <summary>
	/// deal with key presses from the user
	/// </summary>
	/// <param name="t_event">key press event</param>
	void processKeys(sf::Event t_event);

	/// <summary>
	/// Update the game world
	/// </summary>
	/// <param name="t_delta">seconds per frame</param>
	void update(float t_delta);

	/// <summary>
	/// draw the frame and then switch buffers
	/// </summary>
	void render();

	/// <summary>
	/// Closes the window and ends the game.
	/// </summary>
	void exit();

	void setUpGame();

	bool m_exitGame; // control exiting game
	sf::RenderWindow m_window;
	Scene* currentScene;
	sf::Font m_font;

	GameState m_gameState;
	ConnectionType m_connectionType;
	Client* m_currentClient;
	Server* m_server;
	int m_myPlayerID;
	sf::Text m_TagText; //text to display when tag occurs
	float m_elapsedTime;
	sf::Clock m_clock;
	std::vector<Player> m_players;
	bool isFirstTag;
	std::vector<sf::Vector2f> m_startPos;
};
#endif // !GAME_H