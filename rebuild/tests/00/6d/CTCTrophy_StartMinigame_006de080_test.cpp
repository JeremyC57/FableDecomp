#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
struct CCharString{const char*p;CCharString(const char*s):p(s){}};
struct CTCEmoteIcon{enum EEmotePriority{TROPHY=2};int calls;long icon;float time,scale;unsigned char alpha;CTCEmoteIcon():calls(0){}void SetEmoteIcon(EEmotePriority priority,long i,float t,float s,unsigned char a){check(priority==TROPHY,"emote priority");++calls;icon=i;time=t;scale=s;alpha=a;}};
struct CTCSoundPlayer{int calls;const char*last;CTCSoundPlayer():calls(0),last(0){}unsigned long PlayAttachedCriteria(const CCharString&name,bool a,bool b,bool c,long d){check(a&&!b&&!c&&d==-1,"sound flags");++calls;last=name.p;return 42;}};
struct CTCHeroStats{long num;CTCHeroStats():num(10){}long GetNumTrophyGamePlayed()const{return num;}void SetNumTrophyGamePlayed(long n){num=n;}};
enum{TCI_SOUND_PLAYER=16};
struct CThing{bool dead,persisted,hasSound;CTCSoundPlayer sound;CThing():dead(false),persisted(false),hasSound(true){}bool IsDead()const{return dead;}bool HasTC(int type)const{check(type==16,"sound interface");return hasSound;}bool GetTC(int type,CTCSoundPlayer**p){check(type==16,"sound lookup");if(hasSound){*p=&sound;return true;}return false;}void SetAsGameStatePersisted(bool b){persisted=b;}};
struct CThingAICreature:CThing{int disabled;CTCEmoteIcon emote;CThingAICreature():disabled(0){}void SetStateGroupEnabled(const CCharString&name,bool enabled){check(strcmp(name.p,"SG_OPINION_REACTION_ROOT")==0&&!enabled,"opinion state disabled");++disabled;}bool GetTC(CTCEmoteIcon**p){*p=&emote;return true;}};
struct CThingPlayerCreature:CThing{CTCHeroStats stats;bool hasStats;CThingPlayerCreature():hasStats(true){}bool GetTC(CTCHeroStats**p){if(hasStats){*p=&stats;return true;}return false;}};
template<class T>struct CIntelligentPointer{T*p;CIntelligentPointer(T*q=0):p(q){}T*operator->()const{return p;}bool operator!=(int)const{return p!=0;}};
template<class T>bool IS_THING_ALIVE(const T&t){if(t!=0){if(!t->IsDead())return true;}return false;}
struct CPlayer{CThingPlayerCreature hero;CThingPlayerCreature*GetPControlledCreature()const{return const_cast<CThingPlayerCreature*>(&hero);}};
struct CPlayerManager{CPlayer player;CPlayer&GetMainPlayer(){return player;}};
struct CMainGameComponent{CPlayerManager players;static long fps;static CMainGameComponent*current;static CMainGameComponent*Get(){return current;}static long GetConstantFPS(){return fps;}CPlayerManager&GetPlayerManager(){return players;}};long CMainGameComponent::fps=10;CMainGameComponent*CMainGameComponent::current=0;
enum EMiniGameType{MINIGAME_TROPHY=6};
struct QuestInfoDouble{int displays,updates;long owner,count;QuestInfoDouble():displays(0),updates(0){}void DisplayMiniGameInfo(bool show,EMiniGameType type,long p){check(show&&type==MINIGAME_TROPHY,"HUD display contract");++displays;owner=p;}void UpdateMiniGame(float fraction,long value,long maxValue){check(fraction==1&&value==0,"HUD update contract");++updates;count=maxValue;}};
struct CPlayerGui{QuestInfoDouble quest;static CPlayerGui*current;static CPlayerGui&Get(){return *current;}QuestInfoDouble&GetQuestInfo(){return quest;}};CPlayerGui*CPlayerGui::current=0;
struct CTrophyDef{float MinigameInitialTimerSecs;long EmoteIconForAvailableWitness;CCharString SoundStartGame;CTrophyDef():MinigameInitialTimerSecs(13.75f),EmoteIconForAvailableWitness(7),SoundStartGame("start"){}};
struct CTCTrophy{CThing parent;CThing*ParentThing;CTrophyDef def;CTrophyDef*PDef;std::list<CIntelligentPointer<CThingAICreature> >AvailableWitnesses,WitnessesAchieved;bool MinigameInProgress;long MinigameStartingWitnessCount,NumberOfWitnessesLeft,MinigameInitialFramesLeft,MinigameCurrentFramesLeft;CTCTrophy():ParentThing(&parent),PDef(&def),MinigameInProgress(false),MinigameStartingWitnessCount(-1),NumberOfWitnessesLeft(-1),MinigameInitialFramesLeft(-1),MinigameCurrentFramesLeft(-1){}CThing&GetParentThing(){return parent;}unsigned long PlaySoundW(const CCharString&sound);void StartMinigame();};
unsigned long g_InvalidSoundHandle=0xffffffffUL;

inline unsigned long CTCTrophy::PlaySoundW(const CCharString& sound) {
 if(ParentThing->HasTC(TCI_SOUND_PLAYER)) {
  CTCSoundPlayer* sound_player;
  if(ParentThing->GetTC(TCI_SOUND_PLAYER,&sound_player)) {
   unsigned long handle=sound_player->PlayAttachedCriteria(sound,true,false,false,-1);
   return handle;
  }
 }
 return g_InvalidSoundHandle;
}
void CTCTrophy::StartMinigame()
{
 MinigameStartingWitnessCount=AvailableWitnesses.size();
 NumberOfWitnessesLeft=MinigameStartingWitnessCount;
 WitnessesAchieved.clear();
 for(std::list<CIntelligentPointer<CThingAICreature> >::iterator it=AvailableWitnesses.begin(),end_it=AvailableWitnesses.end();it!=end_it;++it) {
  if(IS_THING_ALIVE(*it)) {
   (*it)->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT",false);
   CTCEmoteIcon* ptc_emoteicon;
   (*it)->GetTC(&ptc_emoteicon);
   if(PDef->EmoteIconForAvailableWitness)ptc_emoteicon->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)2,PDef->EmoteIconForAvailableWitness,1000000.0f,1.0f,255);
  }
 }
 MinigameInProgress=true;
 MinigameInitialFramesLeft=(long)(CMainGameComponent::GetConstantFPS()*PDef->MinigameInitialTimerSecs);
 MinigameCurrentFramesLeft=MinigameInitialFramesLeft;
 GetParentThing().SetAsGameStatePersisted(true);
 PlaySoundW(PDef->SoundStartGame);
 CPlayerGui::Get().GetQuestInfo().DisplayMiniGameInfo(true,MINIGAME_TROPHY,(long)&GetParentThing());
 CPlayerGui::Get().GetQuestInfo().UpdateMiniGame(1.0f,0,MinigameStartingWitnessCount);
 CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 CTCHeroStats* HeroStats;
 if(phero->GetTC(&HeroStats))HeroStats->SetNumTrophyGamePlayed(HeroStats->GetNumTrophyGamePlayed()+1);
}
int main(){
 {CMainGameComponent game;CPlayerGui gui;CMainGameComponent::current=&game;CPlayerGui::current=&gui;CTCTrophy trophy;trophy.StartMinigame();check(trophy.MinigameStartingWitnessCount==0&&trophy.NumberOfWitnessesLeft==0&&trophy.WitnessesAchieved.empty(),"empty witness lists");check(trophy.MinigameInProgress&&trophy.MinigameInitialFramesLeft==137&&trophy.MinigameCurrentFramesLeft==137,"timer truncation and activation");check(trophy.parent.persisted&&trophy.parent.sound.calls==1&&strcmp(trophy.parent.sound.last,"start")==0,"persistence and start sound");check(gui.quest.displays==1&&gui.quest.updates==1&&gui.quest.count==0&&gui.quest.owner==(long)&trophy.parent,"HUD owner and counters");check(game.players.player.hero.stats.num==11,"hero game count increments");}
 {CMainGameComponent game;CPlayerGui gui;CMainGameComponent::current=&game;CPlayerGui::current=&gui;CTCTrophy trophy;CThingAICreature live,dead;dead.dead=true;trophy.AvailableWitnesses.push_back(CIntelligentPointer<CThingAICreature>(&live));trophy.AvailableWitnesses.push_back(CIntelligentPointer<CThingAICreature>(&dead));trophy.AvailableWitnesses.push_back(CIntelligentPointer<CThingAICreature>());trophy.WitnessesAchieved.push_back(CIntelligentPointer<CThingAICreature>(&dead));trophy.StartMinigame();check(trophy.MinigameStartingWitnessCount==3&&trophy.NumberOfWitnessesLeft==3&&trophy.WitnessesAchieved.empty(),"counts include all entries and previous results cleared");check(live.disabled==1&&dead.disabled==0&&live.emote.calls==1&&dead.emote.calls==0,"live witness filtering");check(live.emote.icon==7&&live.emote.time==1000000&&live.emote.scale==1&&live.emote.alpha==255,"emote contract");check(gui.quest.count==3,"HUD witness count");}
 {CMainGameComponent game;CPlayerGui gui;CMainGameComponent::current=&game;CPlayerGui::current=&gui;CTCTrophy trophy;CThingAICreature live;trophy.def.EmoteIconForAvailableWitness=0;trophy.def.MinigameInitialTimerSecs=0;trophy.parent.hasSound=false;game.players.player.hero.hasStats=false;trophy.AvailableWitnesses.push_back(CIntelligentPointer<CThingAICreature>(&live));trophy.StartMinigame();check(live.disabled==1&&live.emote.calls==0&&trophy.parent.sound.calls==0,"no icon or sound skips effects");check(trophy.MinigameInitialFramesLeft==0&&game.players.player.hero.stats.num==10,"zero timer and missing stats");check(trophy.PlaySoundW(trophy.def.SoundStartGame)==g_InvalidSoundHandle,"absent sound returns invalid handle");}
 puts("006de080_TEST PASS");return 0;
}