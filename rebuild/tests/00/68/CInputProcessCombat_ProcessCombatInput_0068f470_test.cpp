#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#include "engine/EGameAction.h"
#include "engine/ETargetingType.h"
#include "engine/EGameEventType.h"
#include "engine/EInputButtonEventType.h"

static void require(bool ok) { if(!ok) abort(); }
struct CTCCombat {
    bool flourish,counter;
    bool IsMeleeFlourishValid() const { return flourish; }
    bool IsMeleeBlockCounterValid() const { return counter; }
};
class CThing;
struct CTCTargetingBase {
    CThing* best;
    int types,ranges;
    mutable int fetches;
    CThing* GetPBestThingForMeleeAttack() const { return best; }
    CThing* GetPTargetedThing() const { ++fetches;return best; }
    bool IsTargetedThingOfType(ETargetingType t) const { return (types & t)!=0; }
    bool IsTargetedThingInRange(unsigned long t) const { return (ranges & t)!=0; }
};
struct CTCWeapon { bool projectile; bool IsProjectileWeapon() const { return projectile; } };
struct CTCTargeted { unsigned long mask; unsigned long GetTargetingFlags() const { return mask; } };
struct CTCCarrying { CThing* item; CThing* GetThingInPrimarySlot() const { return item; } };
class CThing {
public:
    bool dead;
    const CTCCombat* combat;
    const CTCTargetingBase* targeting;
    const CTCCarrying* carrying;
    const CTCWeapon* weapon;
    const CTCTargeted* targeted;
    CThing():dead(false),combat(0),targeting(0),carrying(0),weapon(0),targeted(0){}
    bool PeekTC(const CTCCombat** out) const { *out=combat;return combat!=0; }
    bool PeekTC(const CTCTargetingBase** out) const { *out=targeting;return targeting!=0; }
    bool PeekTC(const CTCCarrying** out) const { *out=carrying;return carrying!=0; }
    bool PeekTC(const CTCWeapon** out) const { *out=weapon;return weapon!=0; }
    bool PeekTC(const CTCTargeted** out) const { *out=targeted;return targeted!=0; }
};
class CThingPlayerCreature : public CThing {};
template<class T> bool IS_THING_ALIVE(T* p) { return p && !p->dead; }
struct CPlayer {
    bool spells;
    CThingPlayerCreature* hero;
    bool IsSpellMode() const { return spells; }
    CThingPlayerCreature* GetPControlledCreature() const { require(hero!=0);return hero; }
};
struct CGamePlayerInterface {
    bool shared,requiresHeld;
    mutable int queries;
    bool IsControlInputSharedBetweenGameActions(const std::vector<EGameAction>& gas) const {
        require(gas.size()==2 && gas[0]==GAME_ACTION_INTERACT && gas[1]==GAME_ACTION_ATTACK);
        ++queries;return shared;
    }
    bool IsRequireAttackButtonHeldForFlourish() const {return requiresHeld;}
};
struct CInputEvent { int mask; bool pressed;float pressure; };
struct CGameEvent {
    int type,words,scalars,bytes; unsigned short word;float scalar;unsigned char byte;
    explicit CGameEvent(int id):type(id),words(0),scalars(0),bytes(0),word(0),scalar(0),byte(0){}
    void AddUWORD(unsigned short value){require(!words&&!scalars&&!bytes);++words;word=value;}
    void AddFloat(float value){require(words==1&&!scalars&&!bytes);++scalars;scalar=value;}
    void AddUBYTE(unsigned char value){require(!words&&!scalars&&!bytes);++bytes;byte=value;}
};
struct CProcessedInput {
    std::vector<CGameEvent> events;
    void AddGameEvent(const CGameEvent& e){events.push_back(e);}
};
struct CGameDefinitionManager {
    int GetDefGlobalIndexFromName(const char* name) const {
        if(!strcmp(name,"CREATURE_ABILITY_FLOURISH"))return 0x10011;
        if(!strcmp(name,"CREATURE_ABILITY_BREAK_BLOCK"))return 0x20022;
        require(!strcmp(name,"CREATURE_ABILITY_ATTACK"));return 0x30033;
    }
};
class CInputProcessCombat {
public:
    CPlayer* MyPlayer;
    CGameDefinitionManager* DefinitionManager;
    CGamePlayerInterface* ui;
    bool held;
    const CGamePlayerInterface& GetGamePlayerInterface() const { return *ui; }
    bool IsGameActionEventInQueue(EGameAction a,EInputButtonEventType type) const {
        require(a==GAME_ACTION_ATTACK && type==INPUT_BUTTON_TYPE_HELD);return held;
    }
    bool IsEventButtonPressed(const CInputEvent& e) const{return e.pressed;}
    bool IsEventGameAction(EGameAction a,const CInputEvent& e) const {
        return (e.mask & (1<<int(a)))!=0;
    }
    float GetButtonPressureForEvent(const CInputEvent& e) const{return e.pressure;}
    void ProcessCombatInput(const CInputEvent&,CProcessedInput&);
};
void CInputProcessCombat::ProcessCombatInput(const CInputEvent& event,CProcessedInput& processed_input)
{
    if(MyPlayer->IsSpellMode())
        return;
    const CThingPlayerCreature* phero=MyPlayer->GetPControlledCreature();
    const CTCCombat* ptc_combat=0;
    phero->PeekTC(&ptc_combat);
    const CTCTargetingBase* ptc_targ=0;
    phero->PeekTC(&ptc_targ);
    CThing* pbest_thing_for_attack=ptc_targ->GetPBestThingForMeleeAttack();
    bool allow_melee_attacks=true;
    const CTCCarrying* ptc_carrying=0;
    if(phero->PeekTC(&ptc_carrying)) {
        const CThing* pcarried_thing=ptc_carrying->GetThingInPrimarySlot();
        const CTCWeapon* ptc_weapon=0;
        if(IS_THING_ALIVE(pcarried_thing) && pcarried_thing->PeekTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon()) {
            allow_melee_attacks=false;
            if(IS_THING_ALIVE(pbest_thing_for_attack)) {
                const CTCTargeted* ptc_targeted=0;
                if(pbest_thing_for_attack->PeekTC(&ptc_targeted) && (ptc_targeted->GetTargetingFlags() & TARGETING_STAB))
                    allow_melee_attacks=true;
            }
        }
    }
    std::vector<EGameAction> gas;
    gas.push_back(GAME_ACTION_INTERACT);
    gas.push_back(GAME_ACTION_ATTACK);
    bool interact_and_attack_on_same_button=GetGamePlayerInterface().IsControlInputSharedBetweenGameActions(gas);
    if(interact_and_attack_on_same_button) {
        CThing* ptargeted_thing=ptc_targ->GetPTargetedThing();
        if(!(ptc_targ->IsTargetedThingOfType(TARGETING_MELEE) && ptc_targ->IsTargetedThingInRange(TARGETING_MELEE))) {
            if((ptc_targ->IsTargetedThingOfType(TARGETING_TALKABLE) && ptc_targ->IsTargetedThingInRange(TARGETING_TALKABLE)) ||
               (ptc_targ->IsTargetedThingOfType(TARGETING_USABLE) && ptc_targ->IsTargetedThingInRange(TARGETING_USABLE)))
                allow_melee_attacks=false;
        }
    }
    if(IsEventButtonPressed(event)) {
        if(allow_melee_attacks) {
            bool flourish_prerequisite=true;
            if(GetGamePlayerInterface().IsRequireAttackButtonHeldForFlourish() &&
               !IsGameActionEventInQueue(GAME_ACTION_ATTACK,INPUT_BUTTON_TYPE_HELD))
                flourish_prerequisite=false;
            if(flourish_prerequisite && IsEventGameAction(GAME_ACTION_SPECIAL_ATTACK,event) && ptc_combat->IsMeleeFlourishValid()) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_FLOURISH"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            } else if(IsEventGameAction(GAME_ACTION_SPECIAL_ATTACK,event) && ptc_combat->IsMeleeBlockCounterValid()) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_BREAK_BLOCK"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            } else if(IsEventGameAction(GAME_ACTION_ATTACK,event)) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            }
        }
        if(IsEventGameAction(GAME_ACTION_UNSHEATHE_RANGED_WEAPON,event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_RANGED_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        } else if(IsEventGameAction(GAME_ACTION_UNSHEATHE_MELEE_WEAPON,event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        } else if(IsEventGameAction(GAME_ACTION_SHEATHE_MELEE_WEAPON,event) || IsEventGameAction(GAME_ACTION_SHEATHE_RANGED_WEAPON,event)) {
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON));
        }
    }
}
static void run(int carried,int target,int typeBits,int rangeBits,bool shared,int state,int inputs)
{
    CThingPlayerCreature hero;
    CThing item,victim;
    CPlayer player;
    CTCCombat combat;combat.flourish=(state&8)!=0;combat.counter=(state&16)!=0;
    CTCCarrying carrying;carrying.item=carried>=2?&item:0;
    CTCWeapon weapon;weapon.projectile=carried==5;
    hero.carrying=carried>0?&carrying:0;
    item.dead=carried==2;item.weapon=carried>=4?&weapon:0;
    CTCTargetingBase targeting;targeting.best=target!=0?&victim:0;targeting.fetches=0;
    targeting.types=(typeBits&3)|((typeBits&4)<<2);targeting.ranges=(rangeBits&3)|((rangeBits&4)<<2);
    CTCTargeted targetInfo;targetInfo.mask=target==4?TARGETING_STAB:TARGETING_SHOOTABLE;
    victim.dead=target==1;victim.targeted=target>=3?&targetInfo:0;
    hero.combat=&combat;hero.targeting=&targeting;
    player.spells=(state&1)!=0;player.hero=player.spells?0:&hero;
    CGamePlayerInterface ui;ui.shared=shared;ui.requiresHeld=(state&4)!=0;ui.queries=0;
    CGameDefinitionManager definitions;CInputProcessCombat row;
    row.ui=&ui;row.MyPlayer=&player;row.DefinitionManager=&definitions;row.held=(state&32)!=0;
    CInputEvent e;e.pressed=(state&2)!=0;e.pressure=(state&64)?0.25f:1.0f;e.mask=0;
    const EGameAction actions[]={GAME_ACTION_SPECIAL_ATTACK,GAME_ACTION_ATTACK,GAME_ACTION_UNSHEATHE_RANGED_WEAPON,
                                 GAME_ACTION_UNSHEATHE_MELEE_WEAPON,GAME_ACTION_SHEATHE_MELEE_WEAPON,GAME_ACTION_SHEATHE_RANGED_WEAPON};
    for(int i=0;i<6;++i)if(inputs&(1<<i))e.mask|=1<<int(actions[i]);
    CProcessedInput result;row.ProcessCombatInput(e,result);
    require(ui.queries==(player.spells?0:1));
    require(targeting.fetches==(!player.spells && shared?1:0));
    unsigned k=0;
    if(!player.spells && e.pressed) {
        const int common=typeBits&rangeBits;
        bool canAttack=(carried!=5 || target==4) && (!shared || (common&4) || !(common&3));
        int ability=0;
        if(canAttack) {
            if((inputs&1) && (!ui.requiresHeld || row.held) && combat.flourish) ability=0x11;
            else if((inputs&1) && combat.counter) ability=0x22;
            else if(inputs&2) ability=0x33;
        }
        if(ability) {
            require(k<result.events.size());const CGameEvent& v=result.events[k++];
            require(v.type==22 && v.words==1 && v.word==ability && v.scalars==1 && v.scalar==e.pressure && v.bytes==0);
        }
        int change=(inputs&4)?34:(inputs&8)?33:(inputs&48)?32:0;
        if(change) {
            require(k<result.events.size());const CGameEvent& v=result.events[k++];
            require(v.type==change && !v.words && !v.scalars);
            if(change==32)require(v.bytes==0);else require(v.bytes==1&&v.byte==1);
        }
    }
    require(result.events.size()==k);
}
int main() {
    for(int carry=0;carry<6;++carry)for(int target=0;target<5;++target)
    for(int types=0;types<8;++types)for(int range=0;range<8;++range)for(int shared=0;shared<2;++shared)
        run(carry,target,types,range,shared!=0,122,63);
    for(int state=0;state<128;++state)for(int inputs=0;inputs<64;++inputs) {
        run(0,0,0,0,false,state,inputs);
        run(5,2,0,0,false,state,inputs);
        run(5,4,7,7,true,state,inputs);
    }
    puts("0068f470_TEST PASS");return 0;
}