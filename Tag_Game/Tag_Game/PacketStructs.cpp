#include "PacketStructs.h"

ChatMessage::ChatMessage(std::string t_msg)
{
	message = t_msg;
}

Packet ChatMessage::toPacket()
{
	// CONVERTS A CHATMESSAGE TO A PACKET
	const int packetSize = sizeof(int) * 2 + message.size() * sizeof(char); //calculate packet size
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketData::ChatMessage);
	int messageSize = htonl(message.size());
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &messageSize, sizeof(int)); //copy packet size
	memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
	// CONVERTS A CHATMESSAGE TO A PACKET
}

IDMessage::IDMessage(int t_ID)
{
	m_ID = t_ID;
}

Packet IDMessage::toPacket()
{
	// CONVERTS A IDMESSAGE TO A PACKET.
	const int packetSize = sizeof(int) * 2; //calculate packet size (type,value)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketData::ID);
	int message = htonl(m_ID);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &message, sizeof(int)); //copy packet size
	//memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
	// CONVERTS A IDMESSAGE TO A PACKET.
}

PositionMessage::PositionMessage(int t_id, float t_xPos, float t_yPos)
{
	m_id = t_id;
	m_xPos = t_xPos;
	m_yPos = t_yPos;
}

Packet PositionMessage::toPacket()
{
	// CONVERTS A POSITIONMESSAGE TO A PACKET.
	const int packetSize = (sizeof(int) * 2) + (sizeof(float) * 2); //calculate packet size (type + id & 2 coords)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketData::Position);
	int id = htonl(m_id);
	float xPos = htonl(m_xPos);
	float yPos = htonl(m_yPos);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &id, sizeof(int)); //copy player id
	memcpy(buffer + (sizeof(int) * 2), &xPos, sizeof(float)); //copy xcoord
	memcpy(buffer + (sizeof(int) * 2) + sizeof(float), &yPos, sizeof(float));
	Packet packet(buffer, packetSize);
	return packet;
	// CONVERTS A POSITIONMESSAGE TO A PACKET.
}

TagMessage::TagMessage(int t_id)
{
	m_id = t_id;
}

Packet TagMessage::toPacket()
{
	// CONVERTS A TAGMESSAGE TO A PACKET.
	const int packetSize = sizeof(int) * 2; //calculate packet size (type,value)
	char* buffer = new char[packetSize];
	int packetType = htonl((int)PacketData::TaggedPlayer);
	int id = htonl(m_id);
	memcpy(buffer, &packetType, sizeof(int)); //copy packet type
	memcpy(buffer + sizeof(int), &id, sizeof(int)); //copy packet size
	//memcpy(buffer + sizeof(int) * 2, message.c_str(), message.size() * sizeof(char));
	Packet packet(buffer, packetSize);
	return packet;
	// CONVERTS A TAGMESSAGE TO A PACKET.
}
