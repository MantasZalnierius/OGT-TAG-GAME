#include "Server.h"


bool Server::sendAll(int ID, char* data, int totalBytes)
{
	// SENDS ALL THE PACKETS FROM THE SERVER TO THE CLIENTS.
	int bytesSent = 0;
	while (bytesSent < totalBytes)
	{
		int RetnCheck = send(connections[ID].socket, data + bytesSent, totalBytes - bytesSent, NULL);
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesSent += RetnCheck;
	}
	return true;
	// SENDS ALL THE PACKETS FROM THE SERVER TO THE CLIENTS.
}

bool Server::recieveAll(int ID, char* data, int totalBytes)
{
	// RECIVES ALL THE PACKETS FROM THE CLIENTS
	int bytesRecieved = 0; 
	while (bytesRecieved < totalBytes)
	{
		int RetnCheck = recv(connections[ID].socket, data, totalBytes - bytesRecieved, NULL); 
		if (RetnCheck == SOCKET_ERROR)
			return false;
		bytesRecieved += RetnCheck;
	}
	return true;
	// RECIVES ALL THE PACKETS FROM THE CLIENTS
}

bool Server::SendInt(int ID, int t_int)
{
	t_int = htonl(t_int);
	if (!sendAll(ID, (char*)&t_int, sizeof(int)))
		return false;
	return true;
}

bool Server::GetInt(int ID, int& t_int)
{
	if (!recieveAll(ID, (char*)&t_int, sizeof(int))) 
		return false;
	t_int = ntohl(t_int);
	return true;
}

bool Server::SendPacketType(int ID, PacketData t_packetType)
{
	// SENDS THE PACKET FROM THE SERVER.
	if (!SendInt(ID, (int)t_packetType))
		return false;
	return true;
	// SENDS THE PACKET FROM THE SERVER.
}

bool Server::GetPacketType(int ID, PacketData& t_packetType)
{
	// GETS THE PACKET TYPE.
	int packetType;
	if (!GetInt(ID, packetType))
		return false;
	t_packetType = (PacketData)packetType;
	return true;
	// GETS THE PACKET TYPE.
}

void Server::SendString(int ID, std::string& _string)
{
	// SENDS THE CHAT PACKET.
	ChatMessage message(_string);
	connections[ID].packetManager.Append(message.toPacket());
}

bool Server::GetString(int ID, std::string& _string)
{
	int bufferLength;
	if (!GetInt(ID, bufferLength))
		return false;
	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';

	if (!recieveAll(ID, buffer, bufferLength))
	{
		delete[] buffer;
		return false;
	}

	_string = buffer;
	delete[] buffer;
	return true;
}

void Server::SendID(int t_ID, int t_playerID)
{
	// SENDS THE ID PACKET.
	IDMessage newID(t_playerID);
	connections[t_ID].packetManager.Append(newID.toPacket());
}

bool Server::GetFloat(int ID, float& t_float)
{
	if (!recieveAll(ID, (char*)&t_float, sizeof(float)))
		return false;
	t_float = ntohl(t_float);
	return true;
}

bool Server::GetPosition(int ID, float& xPos, float& yPos)
{
	// GETS THE POSITION PACKET.
	if (!recieveAll(ID, (char*)&xPos, sizeof(float)))
		return false;
	xPos = ntohl(xPos);

	if (!recieveAll(ID, (char*)&yPos, sizeof(float)))
		return false;
	yPos = ntohl(yPos);
	return true;
	// GETS THE POSITION PACKET.
}

void Server::SendPosition(int ID, int playerID, float xPos, float yPos)
{
	// SENDS THE POSITION PACKET.
	PositionMessage newPos(playerID, xPos, yPos);
	connections[ID].packetManager.Append(newPos.toPacket());
}

void Server::SendTagPlayer(int ID, int playerID)
{
	// SENDS THE TAG PACKET.
	TagMessage taggedPlayer(playerID);
	connections[ID].packetManager.Append(taggedPlayer.toPacket());
}
