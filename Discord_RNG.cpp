#include <Core/Core.h>
#include "Discord_RNG.h"

using namespace Upp;
void Discord_RNG::launchCommande(ValueMap payload){
	String channel = payload["d"]["channel_id"];
	String content = payload["d"]["content"];
	String username = payload["d"]["author"]["username"];
	String id = payload["d"]["author"]["id"];
	String discriminator = payload["d"]["author"]["discriminator"];
	
	Cout() << "Pouet" << "\n";
	ptrBot->CreateMessage(channel, "pong");
}

Discord_RNG::Discord_RNG(Upp::String _name, Upp::String _prefix){
	name = _name;
	prefix = _prefix;
	
	EventsMap.Add([&](ValueMap e) {this->launchCommande(e);});
}

void Discord_RNG::Events(ValueMap payload){
	for (auto&e : EventsMap){
		e(payload);
	}
}