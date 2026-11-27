// 0068ef50 CInputProcessUseRangedWeapon::ProcessRangedAttackInput
// Leaf: ?ProcessRangedAttackInput@CInputProcessUseRangedWeapon@@AAEXABVCInputEvent@@AAVCProcessedInput@@ABVCThing@@@Z
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_RANGED_ATTACK_INPUT_PASS29
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_BLOCKING
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_PLAYER_ISSPELLMODE

#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/EGameEventType.h"
#include "engine/CInputProcessGameBase.h"
#define private public
#include "engine/CInputProcessUseRangedWeapon.h"
#undef private
#include "engine/CInputEvent.h"
#include "engine/ThingAlive.h"

void CInputProcessUseRangedWeapon::ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed_input, const CThing& weapon)
{
    const CTCCombat* ptc_combat = 0;
    CThing* phero = MyPlayer->GetPControlledCreature();
    if (IS_THING_ALIVE(phero))
        phero->PeekTC(&ptc_combat);

    if (IsEventGameAction(GAME_ACTION_FIRE_RANGED_WEAPON, event)) {
        CTCHeroRangedAttacks* ptc_ranged_attacks = 0;
        phero->GetTC(&ptc_ranged_attacks);
        const CTCSpecialAbilities* ptc_special_abilities;
        phero->PeekTC(&ptc_special_abilities);
        if (!ptc_combat->IsBlocking() && !MyPlayer->IsSpellMode() && !ptc_special_abilities->GetAbilityMutexLock()) {
            if (IsEventButtonHeld(event) || IsEventButtonPressed(event)) {
                if (IsEventButtonPressed(event))
                    ptc_ranged_attacks->AttemptToLoadRangedWeapon();
                float button_pressure = GetButtonPressureForEvent(event);
                ptc_ranged_attacks->SetButtonPressure(button_pressure);
                ptc_ranged_attacks->ContinueAimingRangedWeapon();
            } else {
                ptc_ranged_attacks->AttemptToFireRangedWeapon();
                processed_input.SetAsPerformedEvent();
            }
        } else {
            processed_input.SetAsPerformedEvent();
        }
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_SHEATHE_RANGED_WEAPON, event)) {
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON));
        } else if (IsEventGameAction(GAME_ACTION_UNSHEATHE_MELEE_WEAPON, event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        }
    }
}

#include <assert.h>
#include <stdio.h>
#include <string.h>

// The engine constructs these objects in the game. The fixture supplies their native
// layouts and exercises the actual component-map search and inline field access.
template<class T> union TestStorage {
    double alignment;
    unsigned char bytes[sizeof(T)];
    T* pointer() { return reinterpret_cast<T*>(bytes); }
};
static TestStorage<CPlayer> player;
static TestStorage<CInputProcessUseRangedWeapon> process;
static TestStorage<CThingPlayerCreature> creature;
static TestStorage<CTCCombat> combat;
static TestStorage<CTCHeroRangedAttacks> ranged;
static TestStorage<CTCSpecialAbilities> special;
static TestStorage<CProcessedInput> result;
static TestStorage<CInputEvent> input;
static CThingPlayerCreature* controlled;
static EGameAction action;
static bool pressed, held, blocking;
static int callLog[8], count;
static void record(int n) { assert(count < 8); callLog[count++]=n; }

CThingPlayerCreature* CPlayer::GetPControlledCreature() const { assert(this==player.pointer()); return controlled; }
bool CInputProcessGameBase::IsEventGameAction(EGameAction a,const CInputEvent& e) const { assert(&e==input.pointer());return a==action; }
bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent&)const { return pressed; }
bool CInputProcessGameBase::IsEventButtonHeld(const CInputEvent&)const { return held; }
float CInputProcessGameBase::GetButtonPressureForEvent(const CInputEvent&)const { return 0.625f; }
bool CTCCombat::IsBlocking()const { assert(this==combat.pointer());return blocking; }
void CTCHeroRangedAttacks::AttemptToLoadRangedWeapon(){ assert(this==ranged.pointer());record(1); }
void CTCHeroRangedAttacks::SetButtonPressure(float pressure){ assert(this==ranged.pointer() && pressure==0.625f);record(2); }
void CTCHeroRangedAttacks::ContinueAimingRangedWeapon(){ assert(this==ranged.pointer());record(3); }
void CTCHeroRangedAttacks::AttemptToFireRangedWeapon(){ assert(this==ranged.pointer());record(4); }
void CProcessedInput::SetAsPerformedEvent(){ assert(this==result.pointer());record(5); }
void CGameEvent::AddData(long size,const void* data){ assert(size==1 && EndPos==0);memcpy(Data,data,size);EndPos=1; }
void CProcessedInput::AddGameEvent(const CGameEvent& e) {
    assert(this==result.pointer() && e.Player==-1 && e.Valid && !e.Replacement);
    if (e.Type==GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON) { assert(e.EndPos==0);record(6); }
    else { assert(e.Type==GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON && e.EndPos==1 && e.Data[0]==1);record(7); }
}

typedef std::pair<ETCInterfaceType,CTCBase*> ComponentEntry;
static ComponentEntry entries[3];
int main() {
    const ETCInterfaceType keys[3]={TCI_COMBAT,TCI_HERO_RANGED_ATTACKS,TCI_SPECIAL_ABILITIES};
    CTCBase* components[3]={combat.pointer(),ranged.pointer(),special.pointer()};
    for(int i=0;i<3;++i){
        entries[i].first=keys[i];entries[i].second=components[i];
        unsigned long bit=1UL<<(keys[i]%32);
        memcpy(creature.bytes+0x20+4*(keys[i]/32),&bit,4);
    }
    ComponentEntry* first=entries;
    ComponentEntry* last=entries+3;
    memcpy(creature.bytes+0x44,&first,sizeof(first));
    memcpy(creature.bytes+0x48,&last,sizeof(last));
    CPlayer* playerPtr=player.pointer();
    memcpy(process.bytes+0x18,&playerPtr,sizeof(playerPtr));
    const EGameAction actions[]={GAME_ACTION_FIRE_RANGED_WEAPON,GAME_ACTION_SHEATHE_RANGED_WEAPON,GAME_ACTION_UNSHEATHE_MELEE_WEAPON,GAME_ACTION_NULL};
    for(int i=0;i!=4;++i)for(int bits=0;bits!=32;++bits)for(int hero=0;hero!=3;++hero){
        if(i==0 && hero!=2)continue; // Firing requires a living hero with these three components.
        controlled=hero==0?0:creature.pointer();creature.bytes[0x91]=hero==1;
        action=actions[i];pressed=(bits&1)!=0;held=(bits&2)!=0;blocking=(bits&4)!=0;
        player.bytes[0x218]=(bits&8)!=0;special.bytes[0xc]=(bits&16)!=0;count=0;
        process.pointer()->ProcessRangedAttackInput(*input.pointer(),*result.pointer(),*creature.pointer());
        int expected[4];int n=0;
        if(i==0) {
            if(bits&28) expected[n++]=5;
            else if(pressed || held){ if(pressed)expected[n++]=1;expected[n++]=2;expected[n++]=3; }
            else { expected[n++]=4;expected[n++]=5; }
        } else if(pressed && (i==1 || i==2))expected[n++]=i==1?6:7;
        assert(n==count && memcmp(expected,callLog,n*sizeof(int))==0);
    }
    puts("0068ef50_TEST PASS");
    return 0;
}