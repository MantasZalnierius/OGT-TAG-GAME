#include "Player.h"

Player::Player() : m_isTagged(false)
{
	setUpPlayer();
}

void Player::setPosition(sf::Vector2f t_pos)
{
	m_body.setPosition(t_pos);
}

void Player::tag(bool t_isTagged)
{
	m_isTagged = t_isTagged;
	if (!m_isTagged)
	{
		m_body.setFillColor(sf::Color::Blue); //set back to real color
	}
	else
	{
		m_body.setFillColor(sf::Color::Green); //set back to real color
	}
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

bool Player::processEvents(sf::Event t_event)
{
	bool isMoved = false;
	if (sf::Keyboard::Up == t_event.key.code)
	{
		setPosition(getPosition() + sf::Vector2f(0, -10));
		isMoved = true;
	}
	if (sf::Keyboard::Down == t_event.key.code)
	{
		setPosition(getPosition() + sf::Vector2f(0, 10));
		isMoved = true;
	}
	if (sf::Keyboard::Left == t_event.key.code)
	{
		setPosition(getPosition() + sf::Vector2f(-10, 0));
		isMoved = true;
	}
	if (sf::Keyboard::Right == t_event.key.code)
	{
		setPosition(getPosition() + sf::Vector2f(10, 0));
		isMoved = true;
	}
	HandleBoundaryCollision();
	return isMoved;
}

bool Player::handleCollisions(Player t_player1, Player t_player2)
{
	if (t_player1.getCircle().getGlobalBounds().intersects(t_player2.getCircle().getGlobalBounds()))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Player::setUpPlayer()
{
	m_body.setRadius(20);
	m_body.setFillColor(sf::Color::Blue);
	m_body.setOrigin(sf::Vector2f(m_body.getGlobalBounds().width / 2, m_body.getGlobalBounds().height / 2));
}

void Player::HandleBoundaryCollision()
{
	if (getPosition().x < 0 + 20.0f)
	{
		setPosition(sf::Vector2f(0 + 20.0f, getPosition().y));
	}
	if (getPosition().x > 800 - 20.0f)
	{
		setPosition(sf::Vector2f(800 - 20.0f, getPosition().y));
	}
	if (getPosition().y < 0 + 20.0f)
	{
		setPosition(sf::Vector2f(getPosition().x, 0 + 20.0f));
	}
	if (getPosition().y > 600 - 20.0f)
	{
		setPosition(sf::Vector2f(getPosition().x, 600 - 20.0f));
	}
}
