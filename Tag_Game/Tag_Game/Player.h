#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	void setId(int t_ID) { m_ID = t_ID; }
	void setPosition(sf::Vector2f t_pos);
	//void setColor(sf::Color t_Color) { m_color = t_Color; }
	void tag(bool t_isTagged);
	void render(sf::RenderWindow& t_window);
	bool processEvents(sf::Event t_event);
	bool handleCollisions(Player t_player1, Player t_player2);

	inline int getID() { return m_ID; }
	inline sf::Vector2f getPosition() { return m_body.getPosition(); }
	inline sf::CircleShape getCircle() { return m_body; };
	inline bool getIsTagged() { return m_isTagged; }
private:
	void setUpPlayer();
	bool m_isTagged; //True if this player is tagged/the chaser, false if not
	void HandleBoundaryCollision();
	sf::CircleShape m_body;//The players circle object
	sf::Color m_color; //The 'real' colour of this player
	int m_ID; // The ID of this player
};