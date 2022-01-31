#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically) 
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Brodcast locally, Address = localhost (this pc)
	}

	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	//Make socket listen
	sListen = socket(AF_INET, SOCK_STREAM, NULL);

	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverPtr = this;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PacketSenderThread, NULL, NULL, NULL); //Create thread that will manage outgoing packets
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
	if (newConnection == 0)
	{
		std::cout << "Failed to accept the client's connection" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Client Connected! ID:" << ConnectionCounter << std::endl;
		connections[ConnectionCounter].socket = newConnection;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(ConnectionCounter), NULL, NULL);
		std::string MOTD = "Welcome! This is the Message of the Day.";
		SendString(ConnectionCounter, MOTD);
		SendID(ConnectionCounter, ConnectionCounter);
		ConnectionCounter++;
		return true;
	}

}

bool Server::ProcessPacket(int ID, PacketData packetType)
{
	// PRCESSES ALL PACKETS.
	switch (packetType)
	{
	case PacketData::ChatMessage:
	{
		std::string message;
		if (!GetString(ID, message))
			return false;

		for (int i = 0; i < ConnectionCounter; i++)
		{
			if (i == ID)
				continue;
			SendString(i, message);
		}
		std::cout << "Processed chat message" << std::endl;
		break;
	}
	case PacketData::Position:
	{
		int playerID;
		if (!GetInt(ID, playerID))
			return false;
		float xPos;
		float yPos;
		if (!GetPosition(ID, xPos, yPos))
			return false;
		for (int i = 0; i < ConnectionCounter; i++)
		{
			if (i == ID)
				continue;
			SendPosition(i, ID, xPos, yPos);
		}
		std::cout << "Processed setPosition" << std::endl;
		break;
	}
	case PacketData::TaggedPlayer:
	{
		int id;
		if (!GetInt(ID, id))
			return false;
		for (int i = 0; i < ConnectionCounter; i++)
		{
			SendTagPlayer(i, id);
		}
		std::cout << "Processed tagplayer "<< std::endl;
		break;
	}
	case PacketData::ResetPlayers:
	{
		for (int i = 0; i < ConnectionCounter; i++) // Next we need to send message to each user
		{
			SendPacketType(i, PacketData::ResetPlayers); //send tag
		}
		std::cout << "Processed reset Packet" << std::endl;
		break;
	}
	default:
		break;
	}
	return true;
	// PRCESSES ALL PACKETS.
}

void Server::ClientHandlerThread(int ID)
{
	PacketData packetType;
	while (true)
	{
		if (!serverPtr->GetPacketType(ID, packetType))
			break;
		if (!serverPtr->ProcessPacket(ID, packetType))
			break; // break out 
	}
	closesocket(serverPtr->connections[ID].socket);
}

void Server::PacketSenderThread()
{
	while (true)
	{
		for (int i = 0; i < serverPtr->ConnectionCounter; i++)
		{
			if (serverPtr->connections[i].packetManager.HasPendingPackets())
			{
				Packet p = serverPtr->connections[i].packetManager.Retrieve();
				if (!serverPtr->sendAll(i, p.buffer, p.size))
				{
					std::cout << "Failed to send packet" << std::endl;
				}
				delete p.buffer;
			}
		}
	}
}
