#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "engine/EAIAction.h"
#include "engine/EMessageEventType.h"
static void check(bool value) { if (!value) exit(2); }

struct C3DVector { float X,Y,Z; };
struct CMessageEvent { EMessageEventType Type; long ExtraData; };
struct CMessageEventFilter_IsOfType {
    EMessageEventType type;
    explicit CMessageEventFilter_IsOfType(EMessageEventType t) : type(t) {}
    bool operator()(const CMessageEvent& e) const {return e.Type==type;}
};
struct CMessageEventManager {
    std::list<CMessageEvent> Messages;
    const CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter) const {
        for(std::list<CMessageEvent>::const_iterator i=Messages.begin();i!=Messages.end();++i)
            if(filter(*i))return &*i;
        return 0;
    }
    void RemoveMessage(const CMessageEvent*);
};
struct CWorld { CMessageEventManager manager; long GetFrame(){return 1024;} CMessageEventManager& GetMessageEventManager(){return manager;} };
struct CCharString { long group; explicit CCharString(long n) : group(n) {} };
struct CTCWife { int calls; bool answer; void ReceivedConfirmation(bool a){++calls;answer=a;} };
struct CTCShopKeeper { int opened; CCharString GetFriendlynessIndicator(){return CCharString(13);} void OpenShoppingInterface(){++opened;} };
struct CTCSoundPlayer { int calls; void PlayAttachedDialogue(const CCharString& s,bool a,bool b,bool c,bool d){check(s.group==13 && !a && !b && c && !d);++calls;} };
struct CTCVillage { int endFrame,paid; int GetCostToBribe(){++paid;return 8;} void SetFrameBribeEnds(long f){endFrame=f;} };
struct CTCVillageMember { CTCVillage* village; CTCVillage* GetPVillage(){return village;} };
struct CTCHeroStats {
    int money,total;
    void AddMoney(long change,bool visible){check(change==-8 && visible);money+=change;}
    int GetTotalBribeAmount()const{return total;} void SetTotalBribeAmount(long n){total=n;}
};
struct CTCHeroOnlineScoreboard {
    enum EOnlineScoreboardActivity { OSA_BRIBES=37 }; int entries;float last;
    void UpdateScoreForActivity(EOnlineScoreboardActivity a,float score,bool flag){check(a==37 && !flag);++entries;last=score;}
};
struct CThingPlayerCreature {
    bool alive; CTCHeroStats* stats; CTCHeroOnlineScoreboard* board;
    bool GetTC(CTCHeroStats** x){*x=stats;return stats!=0;}
    bool GetTC(CTCHeroOnlineScoreboard** x){*x=board;return board!=0;}
};
struct CThingAICreature {
    CTCWife* wife; CTCShopKeeper* shop; CTCSoundPlayer* sound; CTCVillageMember* villageMember;
    bool GetTC(CTCWife** x){*x=wife;return wife!=0;}
    bool GetTC(CTCShopKeeper** x){*x=shop;return shop!=0;}
    bool GetTC(CTCSoundPlayer** x){*x=sound;return sound!=0;}
    bool GetTC(CTCVillageMember** x){*x=villageMember;return villageMember!=0;}
};
struct CPlayer { CThingPlayerCreature* hero; CThingPlayerCreature* GetPControlledCreature(){return hero;} CPlayer& GetMainPlayer(){return *this;} };
template<class T> struct CIntelligentPointer { T* pointer; T& operator*() const{return *pointer;} };
static bool IS_THING_ALIVE(const CIntelligentPointer<CThingPlayerCreature>& p){return p.pointer && p.pointer->alive;}
enum EInteractionReactionType {INTERACTION_REACTION_GOSSIP=0,INTERACTION_REACTION_WIFE=1,INTERACTION_REACTION_OPINION=2,INTERACTION_REACTION_TRADER=3,INTERACTION_REACTION_BRIBE=4};
namespace NCreatureAI {
    static bool escape;
    static CThingPlayerCreature* expectedPlayer;
    static CThingAICreature* expectedCreature;
    static int visits;
    static bool IsThingRunningAwayFromThing(const CThingPlayerCreature& p,const CThingAICreature& c,const C3DVector& pos){
        check(&p==expectedPlayer && &c==expectedCreature && pos.X==2 && pos.Y==4 && pos.Z==8);
        ++visits;return escape;
    }
}
struct CAIStateGroup_MinionReactToPlayerInteraction {
    CWorld world; CThingAICreature creature; CPlayer players;
    int looks,ends,delay; EAIActionReturn idle;
    bool WaitingForConfirmation,DidTraderInteraction;
    EInteractionReactionType ReactionType; C3DVector PositionPlayerWasWhenWaitingForConfirmation;
    CWorld& GetWorld(){return world;} CThingAICreature& GetCreature(){return creature;}
    CPlayer& GetPlayerManager(){return players;}
    void Terminate(){++ends;} EAIActionReturn AIAction_DoNothing(){return idle;}
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<CThingPlayerCreature> p,long flags,float speed){
        check(repeat==ACTION_REPEAT_CALLBACK && p.pointer==players.hero && flags==19 && speed==0.0027777f);
        ++looks;return looks<=delay ? AI_ACTION_IN_PROGRESS : AI_ACTION_TERMINATE_PROCESS;
    }
    void UpdateWaitingForConfirmation(CIntelligentPointer<CThingPlayerCreature>&);
};

inline void CMessageEventManager::RemoveMessage(const CMessageEvent* pevent)
{
    for (std::list<CMessageEvent>::iterator it = Messages.begin(); it != Messages.end(); ++it) {
        const CMessageEvent& event = *it;
        if (&event == pevent) {
            Messages.erase(it);
            return;
        }
    }
}

void CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation(CIntelligentPointer<CThingPlayerCreature>& player)
{
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(player) || !WaitingForConfirmation)
            return;
        CMessageEventManager& manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* pevent = manager.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
        if (pevent) {
            switch (ReactionType) {
            case INTERACTION_REACTION_WIFE: {
                CTCWife* ptc_wife;
                if (GetCreature().GetTC(&ptc_wife))
                    ptc_wife->ReceivedConfirmation(pevent->ExtraData == 1);
                break;
            }
            case INTERACTION_REACTION_TRADER: {
                ReactionType = INTERACTION_REACTION_GOSSIP;
                if (pevent->ExtraData == 1) {
                    CTCShopKeeper* ptc_shopkeeper;
                    if (GetCreature().GetTC(&ptc_shopkeeper)) {
                        CCharString group_name = ptc_shopkeeper->GetFriendlynessIndicator();
                        CTCSoundPlayer* ptc_sound_player;
                        GetCreature().GetTC(&ptc_sound_player);
                        ptc_sound_player->PlayAttachedDialogue(group_name, false, false, true, false);
                        ptc_shopkeeper->OpenShoppingInterface();
                        DidTraderInteraction = true;
                        if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                            Terminate();
                            return;
                        }
                    }
                }
                break;
            }
            case INTERACTION_REACTION_BRIBE: {
                if (pevent->ExtraData == 1) {
                    CTCVillageMember* tc_village_member;
                    if (GetCreature().GetTC(&tc_village_member) && tc_village_member->GetPVillage()) {
                        CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                        CTCVillage* village = tc_village_member->GetPVillage();
                        CTCHeroStats* ptc_hero_stats;
                        if (phero->GetTC(&ptc_hero_stats)) {
                            ptc_hero_stats->AddMoney(-village->GetCostToBribe(), true);
                            ptc_hero_stats->SetTotalBribeAmount(ptc_hero_stats->GetTotalBribeAmount() + village->GetCostToBribe());
                            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                            GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_hero_scoreboard);
                            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_BRIBES,(float)ptc_hero_stats->GetTotalBribeAmount(),false);
                        }
                        tc_village_member->GetPVillage()->SetFrameBribeEnds(GetWorld().GetFrame()+450);
                    }
                }
                break;
            }
            }
            manager.RemoveMessage(pevent);
            WaitingForConfirmation = false;
        }
        if (NCreatureAI::IsThingRunningAwayFromThing(*player,GetCreature(),PositionPlayerWasWhenWaitingForConfirmation))
            return;
        ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,player,19,0.0027777f);
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
}
int main() {
    for(int life=0;life<3;++life) for(int waited=0;waited<2;++waited)
    for(int kind=0;kind<5;++kind) for(int event=0;event<5;++event)
    for(int features=0;features<32;++features) for(int idle=0;idle<2;++idle)
    for(int escape=0;escape<2;++escape) for(int delay=0;delay<2;++delay) {
        CAIStateGroup_MinionReactToPlayerInteraction state;
        CThingPlayerCreature hero; CTCWife wife={0,false}; CTCShopKeeper shop={0};
        CTCSoundPlayer sound={0}; CTCVillage village={0,0}; CTCVillageMember member;
        CTCHeroStats stats={32,16}; CTCHeroOnlineScoreboard board={0,0.0f};
        member.village=(features&8)?&village:0; hero.stats=(features&16)?&stats:0;
        hero.board=&board;hero.alive=(life==2);state.players.hero=&hero;
        state.creature.wife=(features&1)?&wife:0;state.creature.shop=(features&2)?&shop:0;
        state.creature.villageMember=(features&4)?&member:0;state.creature.sound=&sound;
        CIntelligentPointer<CThingPlayerCreature> player={life?&hero:0};
        NCreatureAI::expectedPlayer=&hero;NCreatureAI::expectedCreature=&state.creature;
        NCreatureAI::escape=escape!=0;NCreatureAI::visits=0;
        state.ReactionType=static_cast<EInteractionReactionType>(kind);
        state.WaitingForConfirmation=waited!=0;state.DidTraderInteraction=false;
        state.idle=idle?AI_ACTION_IN_PROGRESS:AI_ACTION_FINISHED;state.delay=delay;state.ends=state.looks=0;
        state.PositionPlayerWasWhenWaitingForConfirmation.X=2;state.PositionPlayerWasWhenWaitingForConfirmation.Y=4;state.PositionPlayerWasWhenWaitingForConfirmation.Z=8;
        if(event) {
            CMessageEvent unrelated={static_cast<EMessageEventType>(1),111};state.world.manager.Messages.push_back(unrelated);
            if(event>=2) {
                CMessageEvent selected={MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO,event==4?1:event==3?2:0};
                state.world.manager.Messages.push_back(selected);
                CMessageEvent old={MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO,-1};state.world.manager.Messages.push_back(old);
            }
        }
        const unsigned int before=state.world.manager.Messages.size();
        const bool active=life==2 && waited, answered=active && event>=2, yes=answered && event==4;
        const bool acceptedTrade=yes && kind==3 && (features&2);
        const bool ends=acceptedTrade && !idle, bribe=yes && kind==4 && (features&4) && (features&8);
        state.UpdateWaitingForConfirmation(player);
        check(state.ends==(ends?1:0) && state.DidTraderInteraction==acceptedTrade);
        check(state.ReactionType==(answered && kind==3?INTERACTION_REACTION_GOSSIP:kind));
        check(state.WaitingForConfirmation==(waited && (!answered || ends)));
        check(state.world.manager.Messages.size()==before-(answered && !ends ? 1 : 0));
        if(answered && !ends){check(state.world.manager.Messages.front().ExtraData==111 && state.world.manager.Messages.back().ExtraData==-1);}
        check(wife.calls==(answered && kind==1 && (features&1)?1:0));if(wife.calls)check(wife.answer==yes);
        check(shop.opened==(acceptedTrade?1:0) && sound.calls==shop.opened);
        bool paid=bribe && (features&16);check(village.endFrame==(bribe?1474:0) && village.paid==(paid?2:0));
        check(stats.money==(paid?24:32) && stats.total==(paid?24:16) && board.entries==(paid?1:0));
        if(paid)check(board.last==24.0f);
        const int ticks=active && !ends ? (answered || escape ? 1 : 1+delay) : 0;
        check(NCreatureAI::visits==ticks && state.looks==(escape?0:ticks));
    }
    puts("0092ee10_TEST PASS");return 0;
}