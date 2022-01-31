#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "PacketData.h"

class Game;

class Client
{
public:
	Client(std::string IP, int PORT, Game& game);
	bool Connect();
	bool CloseConnection();
	bool SendString(std::string& string);

	bool ProcessPacket(PacketData packetType);
	static void ClientThread();

	// FUNCTIONS FOR SENDING PACKET DATA.
	bool sendAll(char* data, int totalbytes);
	bool SendInt(int t_int);
	bool SendPacketType(PacketData packetType);
	bool SendTagPlayer(int t_idToTag);
	// FUNCTIONS FOR SENDING PACKET DATA.

	// FUNCTIONS FOR GETTING PACKET DATA.
	bool recieveAll(char* data, int totalbytes);
	bool GetInt(int& t_int);
	bool GetPacketType(PacketData& packetType);
	bool GetString(std::string& string);
	bool GetPosition(sf::Vector2f& t_pos);
	// FUNCTIONS FOR GETTING PACKET DATA.

private:
	SOCKET currentConnection; // connection socket
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeOfAddr = sizeof(addr); //Need size of addr for connect function

	Game* m_game;
};

static Client* m_client;
#include "Game.h"