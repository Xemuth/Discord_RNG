#include <Core/Core.h>
#include <GraphBuilder/GraphBuilder.h>
#include "Discord_RNG.h"

using namespace Upp;
void Discord_RNG::Teams(ValueMap payload){
	int nbEquipe;
	int nbJoueurs;
	int nbJoueursParEquipe;
	VectorMap<int, Upp::String> lesJoueurs;
	Upp::String message;
	
	if (isStringisANumber(MessageArgs[1]) && !MessageArgs[1].IsEqual("0")){
		nbEquipe = atoi(MessageArgs[1]); 
		nbJoueurs = MessageArgs.GetCount() - 2; // MessageArgs[0] == "teams" && MessageArgs[1] == "{nombre d'équipe}"
		
		for (int i = 2; i <= nbJoueurs + 1; i++){		
			lesJoueurs.Add(Randomf() * 1000, MessageArgs[i]);
		}
	} else{
		// Si MessageArgs[1] n'est pas un int, on défini 2 équipes par défaut
		nbEquipe = 2;
		nbJoueurs = MessageArgs.GetCount() - 1; // MessageArgs[0] == "teams"
		for (int i = 1; i <= nbJoueurs; i++){		
			lesJoueurs.Add(Randomf() * 1000, MessageArgs[i]);
		}
	}
	
	if (nbJoueurs < nbEquipe){
		message = message + "Le nombre d'équipes à former est supérieur au nombre de joueurs !";// + nbEquipe + " avec " + nbJoueurs + " !");
		return;
	}
	
	switch(nbJoueurs % nbEquipe){ //Reste de joueurs sur la dernière équipe
		case 0:
			// Nombre de joueurs divisable par le nombre d'équipe, pas besoin de Bot
			break;
		case 1: // Il reste un joueur dans la dernière équipe
			switch(nbEquipe){
				case 2: // Si on a 2 équipe et qu'il ne reste qu'un joueur, on ajoute 1 bot
					message = message + "Le joueur manquant sera remplacé par <@!314391413200650250>.";
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				case 3: // Si on a 3 équipes et qu'il ne reste qu'un joueur, on ajoute 2 bots
					message = message + "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.";
					lesJoueurs.Add(1, 	"<@!314391413200650250> n.1");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.2");
					nbJoueurs += 2;
					break;
				case 4:// Si on a 4 équipes et qu'il ne reste qu'un joueur, on ajoute 3 bots
					message = message + "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.";
					lesJoueurs.Add(1, "<@!314391413200650250> n.1");
					lesJoueurs.Add(500, "<@!314391413200650250> n.2");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.3");
					nbJoueurs += 3;
					break;
				default: // Fonctionnalité limité à 2, 3 ou 4 équipes
					message = message + "Impossible de créer moins de 2 équipes ou plus de 4 équipes";
					return;
			}
			break;
		case 2: // Il reste 2 joueurs dans la dernière équipe
			switch(nbEquipe){
				case 3: // Si on a 3 équipes et qu'il reste 2 joueurs, on ajoute 1 bots
					message = message + "Le joueur manquant sera remplacé par <@!314391413200650250>.";
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				case 4:// Si on a 4 équipes et qu'il reste 2 joueurs, on ajoute 2 bots
					message = message + "Les joueurs manquants seront remplacés par <@!314391413200650250> n.1 et <@!314391413200650250> n.2.";
					lesJoueurs.Add(1, "<@!314391413200650250> n.1");
					lesJoueurs.Add(1000, "<@!314391413200650250> n.2");
					nbJoueurs += 2;
					break;
				default:// Fonctionnalité limité à 2, 3 ou 4 équipes
					message = message + "Impossible de créer moins de 2 équipes ou plus de 4 équipes";
					return;
			}
			break;
		case 3: // Il reste 3 joueurs dans la dernière équipe
			switch(nbEquipe){
				case 4: // Si on a 4 équipes et qu'il reste 3 joueurs, on ajoute 1 bots
					message = message + "Le joueur manquant sera remplacé par <@!314391413200650250>.";
					lesJoueurs.Add(1, "<@!314391413200650250>");
					nbJoueurs += 1;
					break;
				default:// Fonctionnalité limité à 2, 3 ou 4 équipes
					message = message + "Impossible de créer moins de 2 équipes ou plus de 4 équipes";
					return;
			}
			break;
		default:
				message = message + "Tu m'a saoulé avec tes calculs à la con toi !";
			return;
	}
	
	// Maintenant que l'on est sur que l'on peut mettre tout les joueurs dans les équipes, on
	// peut savoir combien de joueurs il y aura par équipe
	nbJoueursParEquipe = nbJoueurs / nbEquipe;
	
	//Tri de la liste des joueurs selon leur nombre random du plus petit au plus grand
	SortByKey(lesJoueurs);
	
	
	int equipeNumber = 1;
	message = message + SayTeam(equipeNumber);
	
	for (int joueur = 1; joueur <= nbJoueurs; joueur++){
		message = message + SayJoueur(lesJoueurs[joueur - 1]);
		if (joueur % nbJoueursParEquipe == 0){
			equipeNumber++;
			if (equipeNumber <= nbEquipe){	
				message = message + SayTeam(equipeNumber);
			}
		}
	}
	
	BotPtr->CreateMessage(this->ChannelLastMessage, message);
}

Upp::String Discord_RNG::SayTeam(int numEquipe){
	return "Team n." << AsString(numEquipe) << " : \n";
}

Upp::String Discord_RNG::SayJoueur(String joueur){
	return "  -  " << joueur << "\n";
}

void Discord_RNG::Number(ValueMap payload){
	int maxRand = atoi(MessageArgs[0]);
	int random = Randomf() * maxRand;
 	BotPtr->CreateMessage(this->ChannelLastMessage,  AsString(random));
}

void Discord_RNG::Ow(ValueMap payload){
/*	HttpRequest reqApi;
	reqApi.Url("https://ow-api.com/v1/stats/pc/euw/NattyRoots-21691/complete");
	reqApi.GET();
	auto json = ParseJSON(reqApi.Execute());
	
	Vector<Upp::String> heroes;
	
	heroes.Add("Ana");
	heroes.Add("Ash");
	heroes.Add("Baptiste");
	heroes.Add("Bastion");
	heroes.Add("Brigitte");
	heroes.Add("Bastion");
	heroes.Add("Dva");
	heroes.Add("Doomfist");
	heroes.Add("Genji");
	heroes.Add("Hanzo");
	heroes.Add("Junkrat");
	heroes.Add("Lùcio");
	heroes.Add("McCree");
	heroes.Add("Mei");
	heroes.Add("Mercy");
	heroes.Add("Moira");
	heroes.Add("Orisa");
	heroes.Add("Pharah");
	heroes.Add("Reaper");
	heroes.Add("Reinhardt");
	heroes.Add("Roadhog");
	heroes.Add("Sigma");
	heroes.Add("Soldier: 76");
	heroes.Add("Sombra");
	heroes.Add("Symmetra");
	heroes.Add("Torbjörn");
	heroes.Add("Tracer");
	heroes.Add("Widowmaker");
	heroes.Add("Winston");
	heroes.Add("Wrecking Ball");
	heroes.Add("Zarya");
	heroes.Add("Zenyatta");
	
	int r = rand() % sizeof(json["competitiveStats"]["topHeroes"]);
	//BotPtr->CreateMessage(this->ChannelLastMessage, json["competitiveStats"]["topHeroes"].GetKey(ton ITERATOR);	
	//Cout() << json["competitiveStats"]["topHeroes"].GetVA().Get(1).ToString();*/
	BotPtr->CreateMessage(this->ChannelLastMessage, "Coming soon"); //.GetKey(r))
}

void Discord_RNG::Love(ValueMap payload){
	String name = payload["d"]["author"]["username"];
	String name2 = MessageArgs[0];
	
	int diff = 0;
	int tmpDiff = 0;
	
	if (name.GetLength() > name2.GetLength()){
		// name est le plus grand nom
		for(int x = 0; x != name2.GetLength(); x++){
			tmpDiff = AsString(name[x]).Compare(AsString(name2[x]));	
			
			switch(tmpDiff){
				case 1:
					tmpDiff = 16;
					break;
				case -1:
					tmpDiff = 19;
					break;
				default:
					break;
			}
			
			diff += tmpDiff;
			
			if (diff > 100){
				diff -= 100;
			}
		}		
	}
	else{
		// name2 est soit plus grand soit égal à name
		for(int x = 0; x != name.GetLength(); x++){
			tmpDiff = AsString(name[x]).Compare(AsString(name2[x]));	
			
			switch(tmpDiff){
				case 1:
					tmpDiff = 10;
					break;
				case -1:
					tmpDiff = 20;
					break;
				default:
					break;
			}
			
			diff += tmpDiff;
			
			if (diff > 100){
				diff -= 100;
			}
		}
	}
	
	Cout() << name  << " + " << name2 << " = " << AsString(diff);
	
	BotPtr->CreateMessage(this->ChannelLastMessage, name + " et " + name2 + " sont compatibles à " + AsString(diff) + " \% !");
}

void Discord_RNG::Help(ValueMap payload){
	Upp::String message;
	
	message = "```\n";
	
	message = message << "Prefixe : !rng\n\n";
	
	message = message << "!rng <int max> -> Renvoie un nombre aléatoire compris entre 0 et le nombre spécifié.\n\n";
	message = message << "!rng teams <int NombreEquipes = 2> <String joueur> <...> -> Créer des équipes aléatoires avec les joueurs spécifié, par défaut les joueurs sont divisés en 2 équipes.\n\n";
	message = message << "!rng ow -> Renvoie un héro d'overwatch aléatoire.\n\n";
	message << "!rng credit()" <<" -> Affiche les crédit du module rng.\n\n";
	message = message << "```";
	
	BotPtr->CreateMessage(this->ChannelLastMessage, message);
}

Discord_RNG::Discord_RNG(Upp::String _name, Upp::String _prefix){
	name = _name;
	AddPrefix(_prefix);
	
	EventsMapMessageCreated.Add([&](ValueMap e){if(isStringisANumber(Split(this->Message, " ")[0]))this->Number(e);});
	EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("teams"))this->Teams(e);});
	EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("ow"))this->Ow(e);});
    EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("love"))this->Love(e);});

}

String Discord_RNG::Credit(ValueMap json,bool sendCredit){
	String credit =  "----RNG Module have been made By Félix Soignon---\n";
	credit << "-----------https://github.com/NattyRoot-----------\n";
	credit << "Rng module is used to generate various random thing like team or number\n";
	credit << "https://github.com/Xemuth/Discord_RNG";
	if(sendCredit) BotPtr->CreateMessage(ChannelLastMessage,"```"+credit +"```");
	return credit;
}

void Discord_RNG::EventsMessageCreated(ValueMap payload){
	for (auto&e : EventsMapMessageCreated){
		e(payload);
	}
}