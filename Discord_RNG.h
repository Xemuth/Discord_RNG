#ifndef _Discord_RNG_Discord_RNG_h_
#define _Discord_RNG_Discord_RNG_h_
#include <SmartUppBot/SmartBotUpp.h>

using namespace Upp;

class Discord_RNG: public DiscordModule{
	private:
		
		void RandomTeams(ValueMap payload);
		void RandomNumber(ValueMap payload);
		void RandomOwHero(ValueMap payload);
		void Help(ValueMap payload);
		
	public:
		
		Discord_RNG(Upp::String _name, Upp::String _prefix);
		
		void EventsMessageCreated(ValueMap payload);

};
#endif
