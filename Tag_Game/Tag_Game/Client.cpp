#include "Client.h"

Client::Client(std::string IP, int PORT, Game& t_game)
{
	m_game = &t_game;
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WINSOCK ERROR", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Set IP to passed in IP
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	m_client = this; //update client ptr, used for calling methods inside static function
}

bool Client::Connect()
{
	currentConnection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(currentConnection, (SOCKADDR*)&addr, sizeOfAddr) != 0) //If we are unable to connect..
	{
		MessageBoxA(NULL, "ERROR FAILED TO CONNECT", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	std::cout << "Client Connected" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create client Thread 
	return true;
}

bool Client::CloseConnection()
{
	return false;
}

bool Client::ProcessPacket(PacketData packetType)
{
	// HANDLES ALL THE PACKETS THAT ARE SENT.
	switch (packetType)
	{
	case PacketData::ChatMessage:
	{
		std::string message;
		if (!GetString(message))
			return false;
		std::cout << message << std::endl;
		break;
	}
	case PacketData::ID:
	{
		int ID;
		if (!GetInt(ID))
			return false;
		m_game->setUpPlayerID(ID); //setID
		break;
	}
	case PacketData::Position:
	{
		int ID;
		if (!GetInt(ID))
			return false;
		sf::Vector2f pos;
		if (!GetPosition(pos))
			return false;
		m_game->setPlayerPosition(ID, pos);
		break;
	}
	case PacketData::TaggedPlayer:
		int ID;
		if (!GetInt(ID))
			return false;
		m_game->tagPlayer(ID);
		break;
	case PacketData::ResetPlayers:
		m_game->resetPlayers();
		break;
	default:
		break;
	}
	return true;
	// HANDLES ALL THE PACKETS THAT ARE SENT.
}

void Client::ClientThread()
{
	PacketData packetType;
	while (true)
	{
		if (!m_client->GetPacketType(packetType))
			break;
		if (!m_client->ProcessPacket(packetType))
			break;
	}
	if (m_client->CloseConnection())
	{
		std::cout << "Client left" << std::endl;
	}
	else
	{
		std::cout << "Client Failed to leave" << std::endl;
	}
}