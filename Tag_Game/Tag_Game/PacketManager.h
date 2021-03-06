#pragma once
#include "Packet.h"
#include <queue>
#include <mutex>

// This class manages all the packets within my game.

class PacketManager
{
private:
	std::queue<Packet> queue_packets;
	std::mutex mutex_packets;
public:
	bool HasPendingPackets(); //returns true if queue_packets.size > 0
	void Append(Packet p);
	Packet Retrieve();
};