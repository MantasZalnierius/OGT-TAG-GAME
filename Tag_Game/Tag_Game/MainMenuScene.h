#pragma once

#include "Scene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene(sf::Font t_font);
	virtual ~MainMenuScene();

	void update(float t_deltaTime);

	void render(sf::RenderWindow& t_window);

	void processEvent(sf::Event t_event, sf::RenderWindow& t_window);

private:
	void setUpScene();
	sf::Font m_font;
	sf::Text m_Title;
	sf::RectangleShape m_serverButton;
	sf::RectangleShape m_clientButton;
	sf::Text m_serverText;
	sf::Text m_clientText;
	std::vector<sf::RectangleShape> m_buttons;
	std::vector<sf::Text> m_texts;
};
