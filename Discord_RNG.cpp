#include <Core/Core.h>
#include <GraphBuilder/GraphBuilder.h>
#include "Discord_RNG.h"

using namespace Upp;
void Discord_RNG::launchCommande(ValueMap payload){
	int nbEquipe;
	int nbJoueurs;
	int nbJoueursParEquipe;
	
	if (isStringisANumber(MessageArgs[1]) && !MessageArgs[1].IsEqual("0")){
		nbEquipe = atoi(MessageArgs[1]); 
		nbJoueurs = MessageArgs.GetCount() - 2; // MessageArgs[0] == "teams" && MessageArgs[1] == "{nombre d'équipe}"
	} else{
		// Si MessageArgs[1] n'est pas un int, on défini 2 équipes par défaut
		nbEquipe = 2;
		nbJoueurs = MessageArgs.GetCount() - 1; // MessageArgs[0] == "teams"
	}
	
	VectorMap<int, Upp::String> lesJoueurs;
	
	if (nbJoueurs % nbEquipe == 0){ 
		// Nombre de joueurs divisable par le nombre d'équipe, pas besoin de Bot
	} else if(nbJoueurs % nbEquipe == 1){ // Un joueur manquant
		ptrBot->CreateMessage(this->ChannelLastMessage, "Le joueur manquant sera remplacé par <@!314391413200650250>.");
		lesJoueurs.Add(1, "<@!314391413200650250>");
		nbJoueurs += 1;
	}
	else if(nbJoueurs % nbEquipe == 2){ // Deux joueurs manquant
		ptrBot->CreateMessage(this->ChannelLastMessage, "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1,  <@!314391413200650250> n.2 et <@!314391413200650250> n.3.");
		lesJoueurs.Add(1, "<@!314391413200650250>");
		lesJoueurs.Add(2, "<@!314391413200650250>");
		nbJoueurs += 2;
	}
	else{ // Foutage de gueule
		ptrBot->CreateMessage(this->ChannelLastMessage, "Tu m'a saoulé avec tes calculs à la con toi !");
		return;
	}
	
	// Maintenant que l'on est sur que l'on peut mettre tout les joueurs dans les équipes, on
	// peut savoir combien de joueurs il y aura par équipe
	nbJoueursParEquipe = nbJoueurs / nbEquipe;
	
	for (int i = 1; i < nbJoueurs; i++){		
		lesJoueurs.Add(rand() % 1000, MessageArgs[i]);
	}
	
	//Tri de la liste des joueurs selon leur nombre random du plus petit au plus grand
	SortByKey(lesJoueurs);
	
	Upp::String message = "";
	
	int equipeNumber = 1;
	
	message = message << "Team n." << equipeNumber << " : \n";
	
	for (int joueur = 1; joueur < nbJoueurs + 1; ++joueur){
		
		if (joueur % nbJoueursParEquipe != 0){
			message = message << "  -  " << lesJoueurs[joueur - 1] << "\n";
		}
		else{
			message = message << "  -  " << lesJoueurs[joueur - 1] << "\n";
			equipeNumber++;
			message = message <<"Team n." << equipeNumber << " : \n";
		}
		
	}
	
	ptrBot->CreateMessage(this->ChannelLastMessage, message);
}

Discord_RNG::Discord_RNG(Upp::String _name, Upp::String _prefix){
	name = _name;
	prefix = _prefix;
	
	EventsMapMessageCreated.Add([&](ValueMap e) {if(MessageArgs[0].IsEqual("teams"))this->launchCommande(e);});
}

void Discord_RNG::EventsMessageCreated(ValueMap payload){
	for (auto&e : EventsMapMessageCreated){
		e(payload);
	}
}