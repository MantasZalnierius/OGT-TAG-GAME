#pragma once
#include "PacketData.h"
#include "Packet.h"
#include <string>
#include <Windows.h>

// Default chat message in the winsock example

class ChatMessage
{
public:
	ChatMessage(std::string);
	Packet toPacket(); //converts Chat Message to Packet
private:
	std::string message;
};

// ID message to set ID to different objects

class IDMessage
{
public:
	IDMessage(int t_ID);
	Packet toPacket(); //converts Chat Message to Packet
private:
	int m_ID;
};

// Position message to set the the position to different objects.

class PositionMessage
{
public:
	PositionMessage(int t_id, float t_xPos, float t_yPos);
	Packet toPacket(); //converts Chat Message to Packet
private:
	int m_id; //the id of player 
	float m_xPos; //the x coordinate
	float m_yPos; //the y coordinate
};

// Tag Message to let objects in my game know if they are taged or not taged.

class TagMessage
{
public:
	TagMessage(int t_id);
	Packet toPacket(); //converts Chat Message to Packet
private:
	int m_id; //the id of player 
};