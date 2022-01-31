#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(sf::Font t_font) : Scene(GameState::GamePlay), m_font(t_font)
{
	setUpScene();
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::update(float t_deltaTime)
{
}

void MainMenuScene::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_Title);
	for (auto& button : m_buttons)
	{
		t_window.draw(button);
	}
	for (auto& text : m_texts)
	{
		t_window.draw(text);
	}
}

void MainMenuScene::processEvent(sf::Event t_event, sf::RenderWindow& t_window)
{
	if (t_event.type == sf::Event::MouseButtonReleased)
	{
		if (t_event.mouseButton.button == sf::Mouse::Left)
		{
			if (m_serverButton.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(t_window)))
			{
				m_hostOrPlayer =  1;
				m_isEnded = true;
			}
			else if (m_clientButton.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(t_window)))
			{
				m_hostOrPlayer = 2;
				m_isEnded = true;
			}
		}
	}
}

void MainMenuScene::setUpScene()
{
	// SETS UP THE MAIN MENU SCENE.
	m_isEnded = false;
	m_Title.setFont(m_font);
	m_Title.setString("TAG GAME");
	m_Title.setCharacterSize(40);
	m_Title.setPosition(sf::Vector2f(400, 50));
	m_Title.setOrigin(m_Title.getGlobalBounds().width / 2, m_Title.getGlobalBounds().height / 2);

	m_serverButton.setPosition(sf::Vector2f(200, 250));
	m_serverButton.setSize(sf::Vector2f(200, 150));
	m_serverButton.setFillColor(sf::Color::White);
	m_serverButton.setOrigin(m_serverButton.getGlobalBounds().width / 2, m_serverButton.getGlobalBounds().height / 2);

	m_serverText.setFont(m_font);
	m_serverText.setString("HOST");
	m_serverText.setCharacterSize(30);
	m_serverText.setFillColor(sf::Color::Black);
	m_serverText.setPosition(m_serverButton.getPosition());
	m_serverText.setOrigin(m_serverText.getGlobalBounds().width / 2, m_serverText.getGlobalBounds().height / 2);

	m_clientButton.setPosition(sf::Vector2f(600, 250));
	m_clientButton.setSize(sf::Vector2f(200, 150));
	m_clientButton.setFillColor(sf::Color::White);
	m_clientButton.setOrigin(m_clientButton.getGlobalBounds().width / 2, m_clientButton.getGlobalBounds().height / 2);

	m_clientText.setFont(m_font);
	m_clientText.setString("SERVER");
	m_clientText.setCharacterSize(30);
	m_clientText.setFillColor(sf::Color::Black);
	m_clientText.setPosition(m_clientButton.getPosition());
	m_clientText.setOrigin(m_clientText.getGlobalBounds().width / 2, m_clientText.getGlobalBounds().height / 2);

	m_buttons.push_back(m_serverButton);
	m_buttons.push_back(m_clientButton);
	m_texts.push_back(m_serverText);
	m_texts.push_back(m_clientText);
	// SETS UP THE MAIN MENU SCENE.
}
