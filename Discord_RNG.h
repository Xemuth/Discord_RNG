#ifndef _Discord_RNG_Discord_RNG_h_
#define _Discord_RNG_Discord_RNG_h_
#include <SmartUppBot/SmartBotUpp.h>

using namespace Upp;

class Discord_RNG: public DiscordModule{
	private:
		
		void Teams(ValueMap payload);
		void Number(ValueMap payload);
		void Ow(ValueMap payload);
		void Help(ValueMap payload);
		void Love(ValueMap payload);
		Upp::String SayTeam(int numEquipe);
		Upp::String SayJoueur(String joueur);
		
	public:
		
		Discord_RNG(Upp::String _name, Upp::String _prefix);
		
		void EventsMessageCreated(ValueMap payload);

};
#endif
