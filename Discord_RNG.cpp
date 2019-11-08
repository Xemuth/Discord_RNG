#include <Core/Core.h>
#include <GraphBuilder/GraphBuilder.h>
#include "Discord_RNG.h"

using namespace Upp;


void Discord_RNG::PrepareEvent(){
	EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("rng"))this->Number();});
	EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("teams"))this->Teams();});
	EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("ow"))this->Ow();});
    EventsMapMessageCreated.Add([&](ValueMap e){if(this->NameOfFunction.IsEqual("love"))this->Love(e);});
}

void Discord_RNG::Teams(){
	int NbEquipes = 2;
	VectorMap<int,String> allPlayer;
	if(MessageArgs.Find("nbequipe") && MessageArgs.Get("nbequipe").GetTypeName().IsEqual("int")) NbEquipes =  MessageArgs.Get("nbequipe").Get<int>();
	if(MessageArgs.Find("joueurs") && MessageArgs.Get("joueurs").GetTypeName().IsEqual("String") &&  MessageArgs.Get("joueurs").Get<String>().Find(",") !=-1 ){
		auto splitter = Split(MessageArgs.Get("joueurs").Get<String>(),",");
		int NbBotToAdd = splitter.GetCount()%NbEquipes;
		for(int e = 0;e < NbBotToAdd ; e++){
			splitter.Add("<@!314391413200650250> NO "+(e+1));
		}
		for(String& str : splitter){
			allPlayer.Add(Randomf() * 1000,	str);
		}
		//Tri de la liste des joueurs selon leur nombre random du plus petit au plus grand
		SortByKey(allPlayer);
		//Faire l'affichage
		int NbJParEquipe = (int)NbEquipes/splitter;
		int cpt  = 0;
		String message ="```";
		for(i = 0; i < NbEquipes ; i++){
			message << SayTeam((i+1)) << "\n";
			for(e = 0; e < NbJParEquipe; e++){
				message <<  "	"<<SayJoueur(allPlayer.Get(allPlayer.GetKey(cpt)))
				cpt++;
			}
			message << "\n\n";
		}
		message = "```";
		BotPtr->CreateMessage(ChannelLastMessage,message);
	}else{
		BotPtr->CreateMessage(ChannelLastMessage,"Il me faut une liste de joueur !")
	}
}

Upp::String Discord_RNG::SayTeam(int numEquipe){
	return "Team n." << AsString(numEquipe) << " : \n";
}

Upp::String Discord_RNG::SayJoueur(String joueur){
	return "-" << joueur << "\n";
}

void Discord_RNG::Number(){
	int StartValue = 0;
	int MaxValue= 100;
	if(MessageArgs.Find("startvalue") != -1 &&  MessageArgs.Get("startvalue").GetTypeName().IsEqual("int")) StartValue = MessageArgs.Get("startvalue").Get<int>();
	if(MessageArgs.Find("maxvalue") != -1 &&  MessageArgs.Get("maxvalue").GetTypeName().IsEqual("int")) StartValue = MessageArgs.Get("maxvalue").Get<int>();

	int random =  StartValue + (Randomf() * MaxValue);
	
 	BotPtr->CreateMessage(this->ChannelLastMessage,  AsString(random));
}

void Discord_RNG::Ow(){
	Vector<Upp::String> heroes={
	"Ana","Ash","Baptiste","Bastion",
	"Brigitte","Bastion","Dva","Doomfist",
	"Genji","Hanzo","Junkrat","Lùcio",
	"McCree","Mei","Mercy","Moira",
	"Orisa","Pharah","Reaper","Reinhardt",
	"Roadhog","Sigma","Soldier: 76","Sombra",
	"Symmetra","Torbjörn","Tracer","Widowmaker",
	"Winston","Wrecking Ball","Zarya","Zenyatta"};
	
	int rng = (Randomf() * heroes.GetCount());
	BotPtr->CreateMessage(this->ChannelLastMessage, heroes[rng]);
}

void Discord_RNG::Love(ValueMap payload){
	String name = AuthorId;
	String name2 = "";
	if(MessageArgs.Find("love") != -1 && MessageArgs.Get("love").Get<String>().IsEmpty()){
		name2 =MessageArgs.Get("love").Get<String>();
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
		
		//Cout() << name  << " + " << name2 << " = " << AsString(diff);
		
		BotPtr->CreateMessage(this->ChannelLastMessage, name + " et " + name2 + " sont compatibles à " + AsString(diff) + " \% !");
	}else{
		BotPtr->CreateMessage(this->ChannelLastMessage, "Je n'ai pas de nom !");
	}
}

void Discord_RNG::Help(ValueMap payload){
	Upp::String message;
	
	message = "```";
	message << "Prefixe : !rng\n\n";
	message << "*******'Ops' veut dire Optionnel. Chaque Argument noté Optionnel n'est pas obligatoire.*******\n\n";
	message << "*******'Nom Discord' est le nom affiché lors de la mention de quelqu'un sur discord. Exemple : @Xemuth*******\n\n";
	message << "!rng rng(Ops->StartValue:<Nombre entier> 20 ;Ops->MaxValue:<Nombre entier> 40) -> Renvoie un nombre aléatoire compris entre 0 et le nombre spécifié. Ses arguments son 'maxValue' qui défini la valeur Max et 'StartValue' qui définie la valeur de départ\n\n";
	message << "!rng Teams(nbEquipe:<Nombre entier> 2;Joueurs:<Nom Discord || Chaine de caractère> @Xemuth, @Natty, @Jay, @Kazoie ...) -> Créer des équipes aléatoires avec les joueurs spécifié, par défaut les joueurs sont divisés en 2 équipes. Ses arguments sont 'NbEquipe' et 'Joueurs' qui possède la liste des joueurs séparé par une virgule.\n\n";
	message << "!rng Ow -> Renvoie un héro d'overwatch aléatoire. Cette fonction n'a pas d'arguments.\n\n";
	message << "!rng Love(love: <Nom Discord> @Xemuth) -> Donne le \% de compatibilité amoureuse entre l'émetteur et la personne en paramètre. Les paramètres sont 'love'.\n\n";
	message << "!rng credit" <<" -> Affiche les crédit du module rng. Cette fonction n'a pas d'arguments\n\n";
	message << "```";
	
	BotPtr->CreateMessage(this->ChannelLastMessage, message);
}

Discord_RNG::Discord_RNG(Upp::String _name, Upp::String _prefix){
	name = _name;
	AddPrefix(_prefix);
	PrepareEvent();
}

Discord_RNG::Discord_RNG(Upp::String _name, Vector<String> _prefix){
	name = _name;
	AddPrefix(_prefix);
	PrepareEvent();
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