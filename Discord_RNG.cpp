#include <Core/Core.h>
#include <GraphBuilder/GraphBuilder.h>
#include "Discord_RNG.h"

using namespace Upp;
void Discord_RNG::RandomTeams(ValueMap payload){
	int nbEquipe;
	int nbJoueurs;
	int nbJoueursParEquipe;
	VectorMap<int, Upp::String> lesJoueurs;
	
	if (isStringisANumber(MessageArgs[1]) && !MessageArgs[1].IsEqual("0")){
		nbEquipe = atoi(MessageArgs[1]); 
		nbJoueurs = MessageArgs.GetCount() - 2; // MessageArgs[0] == "teams" && MessageArgs[1] == "{nombre d'équipe}"
		
		for (int i = 2; i <= nbJoueurs + 1; i++){		
			lesJoueurs.Add(rand() % 1000, MessageArgs[i]);
		}
	} else{
		// Si MessageArgs[1] n'est pas un int, on défini 2 équipes par défaut
		nbEquipe = 2;
		nbJoueurs = MessageArgs.GetCount() - 1; // MessageArgs[0] == "teams"
		for (int i = 1; i <= nbJoueurs; i++){		
			lesJoueurs.Add(rand() % 1000, MessageArgs[i]);
		}
	}
	
	if (nbJoueurs < nbEquipe){
		ptrBot->CreateMessage(this->ChannelLastMessage, "Le nombre d'équipes à former est supérieur au nombre de joueurs !");// + nbEquipe + " avec " + nbJoueurs + " !");
		return;
	}
	
	int nbJoueursManquant = 0;
	
	switch(nbJoueurs % nbEquipe){ //Reste de joueurs sur la dernière équipe
		case 0:
			// Nombre de joueurs divisable par le nombre d'équipe, pas besoin de Bot
			break;
		case 1: // Il reste un joueur dans la dernière équipe
			switch(nbEquipe){
				case 2: // Si on a 2 équipe et qu'il ne reste qu'un joueur, on ajoute 1 bot
					ptrBot->CreateMessage(this->ChannelLastMessage, "Le joueur manquant sera remplacé par <@!314391413200650250>.");
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				case 3: // Si on a 3 équipes et qu'il ne reste qu'un joueur, on ajoute 2 bots
					ptrBot->CreateMessage(this->ChannelLastMessage, "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.");
					lesJoueurs.Add(1, "<@!314391413200650250> n.1");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.2");
					nbJoueurs += 2;
					break;
				case 4:// Si on a 4 équipes et qu'il ne reste qu'un joueur, on ajoute 3 bots
					ptrBot->CreateMessage(this->ChannelLastMessage, "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.");
					lesJoueurs.Add(1, "<@!314391413200650250> n.1");
					lesJoueurs.Add(500, "<@!314391413200650250> n.2");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.3");
					nbJoueurs += 3;
					break;
				default: // Fonctionnalité limité à 2, 3 ou 4 équipes
					ptrBot->CreateMessage(this->ChannelLastMessage, "Impossible de créer moins de 2 équipes ou plus de 4 équipes");
					return;
			}
			break;
		case 2: // Il reste 2 joueurs dans la dernière équipe
			switch(nbEquipe){
				case 3: // Si on a 3 équipes et qu'il reste 2 joueurs, on ajoute 1 bots
					ptrBot->CreateMessage(this->ChannelLastMessage, "Le joueur manquant sera remplacé par <@!314391413200650250>.");
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				case 4:// Si on a 4 équipes et qu'il reste 2 joueurs, on ajoute 2 bots
					ptrBot->CreateMessage(this->ChannelLastMessage, "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.");
					lesJoueurs.Add(1, "<@!314391413200650250> n.1");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.2");
					nbJoueurs += 2;
					break;
				default:// Fonctionnalité limité à 2, 3 ou 4 équipes
					ptrBot->CreateMessage(this->ChannelLastMessage, "Impossible de créer moins de 2 équipes ou plus de 4 équipes");
					return;
			}
			break;
		case 3: // Il reste 3 joueurs dans la dernière équipe
			switch(nbEquipe){
				case 4: // Si on a 4 équipes et qu'il reste 3 joueurs, on ajoute 1 bots
					ptrBot->CreateMessage(this->ChannelLastMessage, "Le joueur manquant sera remplacé par <@!314391413200650250>.");
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				default:// Fonctionnalité limité à 2, 3 ou 4 équipes
					ptrBot->CreateMessage(this->ChannelLastMessage, "Impossible de créer moins de 2 équipes ou plus de 4 équipes");
					return;
			}
			break;
		default:
			ptrBot->CreateMessage(this->ChannelLastMessage, "Tu m'a saoulé avec tes calculs à la con toi !");
			return;
	}
	
	// Maintenant que l'on est sur que l'on peut mettre tout les joueurs dans les équipes, on
	// peut savoir combien de joueurs il y aura par équipe
	nbJoueursParEquipe = nbJoueurs / nbEquipe;
	
	//Tri de la liste des joueurs selon leur nombre random du plus petit au plus grand
	SortByKey(lesJoueurs);
	
	for (Upp::String jo : lesJoueurs){
		Cout() << jo << "\n";
	}
	
	Upp::String message = "";
	
	int equipeNumber = 1;
	
	message = message << "Team n." << equipeNumber << " : \n";
	
	for (int joueur = 1; joueur <= nbJoueurs; joueur++){
		message = message << "  -  " << lesJoueurs[joueur - 1] << "\n";
		if (joueur % nbJoueursParEquipe == 0){
			equipeNumber++;
			if (equipeNumber <= nbEquipe){	
				message = message <<"Team n." << equipeNumber << " : \n";
			}
		}
	}
	ptrBot->CreateMessage(this->ChannelLastMessage, message);
}

void Discord_RNG::RandomNumber(ValueMap payload){
	int maxRand = atoi(MessageArgs[0]);
	int random = rand() % maxRand;
 	ptrBot->CreateMessage(this->ChannelLastMessage,  AsString(random));
}

void Discord_RNG::RandomOwHero(ValueMap payload){
	Vector<Upp::String> heroes;
	
	heroes.Add("Ana");
	heroes.Add("Bastion");
	heroes.Add("Baptiste");
	heroes.Add("Brigitte");
	heroes.Add("Bastion");
	heroes.Add("Faucheur");
	heroes.Add("Fatale");
	heroes.Add("Dva");
	
	int rng = rand() % 8;
	Upp::String random = heroes[rng];
	
	ptrBot->CreateMessage(this->ChannelLastMessage, random);
}

Discord_RNG::Discord_RNG(Upp::String _name, Upp::String _prefix){
	name = _name;
	prefix = _prefix;
	
	EventsMapMessageCreated.Add([&](ValueMap e){if(isStringisANumber(MessageArgs[0]))this->RandomNumber(e);});
	EventsMapMessageCreated.Add([&](ValueMap e){if(MessageArgs[0].IsEqual("teams"))this->RandomTeams(e);});
	EventsMapMessageCreated.Add([&](ValueMap e){if(MessageArgs[0].IsEqual("ow"))this->RandomOwHero(e);});
}
void Discord_RNG::EventsMessageCreated(ValueMap payload){
	for (auto&e : EventsMapMessageCreated){
		e(payload);
	}
}