#include "Game.h"

///////////////////////////////////////////////////////////////////////////////
Game::Game() :
	m_exitGame{ false },
	m_window{ sf::VideoMode{ 800, 600, 32U }, "TAG" }
{
	setUpGame();
}

///////////////////////////////////////////////////////////////////////////////
Game::~Game()
{
}

///////////////////////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (!m_exitGame)
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		render(); // as many as possible
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame.asSeconds()); //60 fps
			render(); // as many as possible
			if (m_exitGame) break;
		}
	}
}

void Game::init(Client& t_client)
{
	m_currentClient = &t_client;
}

void Game::HandleCollisions()
{
	// HANDLES ALL THE COLLISIONS BETWEEN ALL PLAYERS, WHEN THERE IS 3 ACTIVE CLIENTS.
	for (Player& player : m_players)
	{
		if (player.getIsTagged())
		{
			for (Player& otherPlayer : m_players)
			{
				if (otherPlayer.getID() != player.getID())
				{
					if (otherPlayer.handleCollisions(otherPlayer, player))
					{
						TagMessage tagPlayer(otherPlayer.getID());
						Packet p = tagPlayer.toPacket();
						m_currentClient->sendAll(p.buffer, p.size);
					}
				}
			}
		}
	}
	// HANDLES ALL THE COLLISIONS BETWEEN ALL PLAYERS, WHEN THERE IS 3 ACTIVE CLIENTS.
}

void Game::ListenForNewConnectionThread(Server& t_server)
{
	while (true) {
		t_server.ListenForNewConnection();
	}
}

void Game::setUpPlayerID(int t_ID)
{
	// SETS ALL THE IDS FOR THE PLAYERS, AND CHOOSES A RANDOM PLAYER (FROM 0-2) TO BE THE TAGED PLAYER.
	m_myPlayerID = t_ID;
	for (int i = 0; i < 3; i++)
	{
		Player p = *new Player();
		p.setId(i);
		p.setPosition(m_startPos.at(i));
		m_players.push_back(p);
	}
	if (t_ID == 2)
	{
		srand(static_cast<unsigned>(time(nullptr)));
		int randomID = rand() % 3;
		TagMessage tagRandomPlayer(randomID);
		Packet p = tagRandomPlayer.toPacket();
		m_currentClient->sendAll(p.buffer, p.size);
		m_clock.restart();//restart clock
	}
	// SETS ALL THE IDS FOR THE PLAYERS, AND CHOOSES A RANDOM PLAYER (FROM 0-2) TO BE THE TAGED PLAYER.
}

void Game::setPlayerPosition(int t_id, sf::Vector2f t_pos)
{
	// SETS THE POSITIONS OF EACH PLAYER.
	for (Player& player : m_players)
	{
		if (player.getID() == t_id)
		{
			player.setPosition(t_pos);
		}
	}
	// SETS THE POSITIONS OF EACH PLAYER.
}

void Game::tagPlayer(int idToTag)
{
	// TAGS A PLAYER.
	for (Player& player : m_players)
	{
		if (player.getID() == idToTag)
		{
			player.tag(true);
		}
		else
		{
			player.tag(false);
		}
	}
	if (!isFirstTag)
	{
		isFirstTag = true;
	}
	else {
		m_gameState = GameState::Tagged;
		m_clock.restart(); //reset clock
	}
	// TAGS A PLAYER.
}

void Game::resetPlayers()
{
	// RESETS ALL PLAYERS BACK TO THIER INTIAL POSITIONS.
	for (int i = 0; i < 3; i++)
	{
		m_players.at(i).setPosition(m_startPos.at(i));
	}
	m_gameState = GameState::GamePlay;
	m_clock.restart();
	// RESETS ALL PLAYERS BACK TO THIER INTIAL POSITIONS.
}

///////////////////////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (!currentScene->isEnded())
		{
			if (sf::Event::Closed == newEvent.type)
				exit();
			else
				currentScene->processEvent(newEvent, m_window);
		}
		else
		{
			if (sf::Event::Closed == newEvent.type)
				exit();

			else if (sf::Event::KeyPressed == newEvent.type)
				processKeys(newEvent);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
		exit();
	for (Player& player : m_players)
	{
		if (player.getID() == m_myPlayerID)
		{
			if (player.processEvents(t_event));
			{
				// SENDS PACKETS WHEN A PLAYER MOVES.
				PositionMessage pos(player.getID(), player.getPosition().x, player.getPosition().y);
				Packet p = pos.toPacket();
				m_currentClient->sendAll(p.buffer, p.size);
				// SENDS PACKETS WHEN A PLAYER MOVES.
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Game::update(float t_delta)
{
	if (!m_window.isOpen() || m_exitGame)
		exit();
	if (!currentScene->isEnded())
	{
		currentScene->update(t_delta);
	}
	else
	{
		if (m_gameState == GameState::MainMenu)
		{
			if (currentScene->getServerOrClient() == 1) // SERVER
			{
				try {
					m_server = new Server(6202, true);
					std::thread listenThread(ListenForNewConnectionThread, std::ref(*m_server));
					listenThread.detach();
					m_currentClient = new Client("127.0.0.1", 6202, *this); // VERY IMPORTANT -> CHANGE THIS IP TO YOUR LOCAL MACHINES IP.
					if (!m_currentClient->Connect())
					{
						std::cout << "Failed to connect to server..." << std::endl;
					}
					m_connectionType = ConnectionType::Server;
					m_gameState = GameState::GamePlay;
				}
				catch (...) {
					std::cout << "Server failed to create, must be in use, maybe try join a game." << std::endl;
				}
			}
			else if (currentScene->getServerOrClient() == 2) // CLIENT
			{
				try {
					m_currentClient = new Client("127.0.0.1", 6202, *this); // VERY IMPORTANT -> CHANGE THIS IP TO YOUR LOCAL MACHINES IP.
					if (!m_currentClient->Connect())
					{
						std::cout << "Failed to connect to server..." << std::endl;
					}
					m_connectionType = ConnectionType::Client;
					m_gameState = GameState::GamePlay;
				}
				catch (...) {
					std::cout << "failed to connect, no server or maybe full server. Try Host!" << std::endl;
				}
			}
		}
	}
	// UPDATES GAME STATES.
	switch (m_gameState)
	{
	case GameState::GamePlay:
	{
		if (m_myPlayerID == 0)
			HandleCollisions();
		break;
	}
	case GameState::Tagged:
	{
		m_elapsedTime = m_clock.getElapsedTime().asSeconds();
		if (m_elapsedTime > 2)
		{
			m_gameState = GameState::GameOver;
		}
		break;
	}
	case GameState::GameOver:
	{
		//reset
		m_currentClient->SendPacketType(PacketData::ResetPlayers);
	}
	default:
		break;
	}
	// UPDATES GAME STATES.
}

///////////////////////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color::Black);
	if (!currentScene->isEnded() && m_gameState == GameState::MainMenu)
	{
		currentScene->render(m_window);
	}
	else
	{
		for (auto& player : m_players)
		{
			player.render(m_window);
			if (m_gameState == GameState::Tagged)
			{
				if (player.getIsTagged())
				{
					m_window.draw(m_TagText);
				}
			}
		}
	}
	m_window.display();
}

///////////////////////////////////////////////////////////////////////////////
void Game::exit()
{
	m_exitGame = true;
}

void Game::setUpGame()
{
	if (!m_font.loadFromFile("ASSETS//FONTS//ariblk.ttf")) {};
	currentScene = new MainMenuScene(m_font);
	m_gameState = GameState::MainMenu; //initial state
	m_connectionType = ConnectionType::NotConnected; //initial state
	m_currentClient = nullptr; //null initially until player decides
	m_server = nullptr;
	m_TagText.setFont(m_font);
	m_TagText.setCharacterSize(50);
	m_TagText.setFillColor(sf::Color::Green);
	isFirstTag = false;
	m_clock.restart();

	m_startPos.push_back(sf::Vector2f{ 100.0f, 100.0f });
	m_startPos.push_back(sf::Vector2f{ m_window.getSize().x - 100.0f, 100.0f });
	m_startPos.push_back(sf::Vector2f{ 400.0f, 500.0f });
}
