#include "Client.h"

bool Client::sendAll(char* data, int totalBytes)
{
	// SENDS ALL THE DATA PASSED.
	int bytesSent = 0;
	while (bytesSent < totalBytes)
	{
		int RetnCheck = send(currentConnection, data + bytesSent, totalBytes - bytesSent, NULL); //try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesSent += RetnCheck;
	}
	// SENDS ALL THE DATA PASSED.

	return true;
}

bool Client::SendInt(int t_int)
{
	t_int = htonl(t_int);
	if (!sendAll((char*)&t_int, sizeof(int)))
		return false;
	return true;
}

bool Client::recieveAll(char* data, int totalBytes)
{
	// RECIVES ALL THE DATA REQUESTED
	int bytesRecieved = 0;
	while (bytesRecieved < totalBytes)
	{
		int RetnCheck = recv(currentConnection, data + bytesRecieved, totalBytes - bytesRecieved, NULL);
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesRecieved += RetnCheck;
	}
	// RECIVES ALL THE DATA REQUESTED
	return true; //Success!
}

bool Client::GetInt(int& t_int)
{
	if (!recieveAll((char*)&t_int, sizeof(int)))
		return false;
	t_int = ntohl(t_int);
	return true;
}

bool Client::SendPacketType(PacketData t_packetType)
{
	// TRYS TO SEND PACKET OF DATA.
	if (!SendInt((int)t_packetType))
		return false;
	return true;
	// TRYS TO SEND PACKET OF DATA.
}

bool Client::SendTagPlayer(int t_idToTag)
{
	// SENDS A TAG PLAYER PACKET.
	if (!SendPacketType(PacketData::TaggedPlayer))
		return false;
	TagMessage taggedPlayer(t_idToTag);
	Packet p = taggedPlayer.toPacket();
	sendAll(p.buffer, p.size);
	return true;
	// SENDS A TAG PLAYER PACKET.
}

bool Client::GetPacketType(PacketData& t_packetType)
{
	// GETS THE TYPE PACKET OF DATA.
	int packetType;
	if (!GetInt(packetType))
		return false;
	t_packetType = (PacketData)packetType;
	return true;
	// GETS THE TYPE PACKET OF DATA.
}

bool Client::SendString(std::string& _string)
{
	// SENDS THE PACKET AS A CHAT MESSAGE.
	if (!SendPacketType(PacketData::ChatMessage))
		return false; 
	int bufferLength = _string.size();
	if (!SendInt(bufferLength))
		return false;
	if (!sendAll((char*)_string.c_str(), bufferLength))
		return false;
	return true;
	// SENDS THE PACKET AS A CHAT MESSAGE.
}

bool Client::GetString(std::string& _string)
{
	// GETS THE STRING FROM THE PACKET AS REQUESTED.
	int bufferLength;
	if (!GetInt(bufferLength))
		return false;
	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';

	if (!recieveAll(buffer, bufferLength))
	{
		delete[] buffer;
		return false;
	}

	_string = buffer;
	delete[] buffer;
	return true;
	// GETS THE STRING FROM THE PACKET AS REQUESTED.
}

bool Client::GetPosition(sf::Vector2f& t_pos)
{
	// GETS THE POSITION PACKET.
	float xPos;
	if (!recieveAll((char*)&xPos, sizeof(float)))
		return false;
	xPos = ntohl(xPos);
	float yPos;
	if (!recieveAll((char*)&yPos, sizeof(float)))
		return false;
	yPos = ntohl(yPos);
	t_pos.x = xPos;
	t_pos.y = yPos;
	return true;
	// GETS THE POSITION PACKET.
}
