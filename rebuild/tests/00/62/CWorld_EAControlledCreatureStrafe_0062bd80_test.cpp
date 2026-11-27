// 0062bd80 CWorld::EAControlledCreatureStrafe
// Flags: /GS
// Leaf: ?EAControlledCreatureStrafe@CWorld@@QAEXPBVCGameEvent@@@Z
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
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE_INLINE_PASS29
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CGameEvent.h"
#include "engine/CGameEventDataPackage.h"
#include "engine/CWorld.h"

#include "engine/ThingAlive.h"
#include "engine/fable_nscript_forward.h"

void CWorld::EAControlledCreatureStrafe(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    CPlayer* pplayer = GetPlayer(event->GetPlayerNumber());
    CThingPlayerCreature* phero = pplayer->GetPControlledCreature();
    FableNScriptUInt64 uid = event_data.GetUHUGE();
    float x_axis = event_data.GetFloat();
    float y_axis = event_data.GetFloat();
    EGameAction game_action = static_cast<EGameAction>(event_data.GetSLONG());
    if (IS_THING_ALIVE(phero)) {
        const CTCWeapon* tc_weapon = 0;
        const CTCCarrying* tc_carrying;
        CThing* carried_thing = phero->PeekTC(&tc_carrying)
            ? tc_carrying->GetThingInPrimarySlot() : 0;
        bool is_weapon = carried_thing ? carried_thing->PeekTC(&tc_weapon) : false;
        if (!is_weapon || (is_weapon && tc_weapon && tc_weapon->IsUsableBy(*phero))) {
            C3DVector strafe_vector(x_axis, y_axis, 0.0f);
            phero->ApplyRelativeMovementAcceleration(strafe_vector, game_action);
        }
    }
}

#include "engine/EGameAction.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Engine objects are supplied by the game. The fixture supplies aligned storage
// for their native x86 layouts and intercepts calls at the declared interfaces.
template<class T> union FixtureStorage {
    double alignment;
    unsigned char bytes[sizeof(T)];
    T* pointer() { return reinterpret_cast<T*>(bytes); }
};
static FixtureStorage<CWorld> worldStorage;
static FixtureStorage<CPlayer> playerStorage;
static FixtureStorage<CThingPlayerCreature> heroStorage;
static FixtureStorage<CThing> itemStorage;
static FixtureStorage<CTCCarrying> carryingStorage;
static FixtureStorage<CTCWeapon> weaponStorage;
static CThingPlayerCreature* hero;
static CThing* item;
static bool usable;
static int reads, playerCalls, carryingCalls, usableCalls, moves;
static const float xInput = -2.5f, yInput = 7.25f;
static const EGameAction actionInput = GAME_ACTION_BLOCK;
void CGameEvent::GetDataPackage(CGameEventDataPackage*) const { reads = 0; }
void CGameEventDataPackage::GetData(long size, void* data)
{
    ++reads;
    assert(size == (reads == 1 ? 8 : 4));
    if (reads == 1) memset(data, 0x51, 8);
    else if (reads == 2) memcpy(data, &xInput, 4);
    else if (reads == 3) memcpy(data, &yInput, 4);
    else { assert(reads == 4); memcpy(data, &actionInput, 4); }
}
CPlayer* CWorld::GetPlayer(unsigned long number)
{
    assert(number == static_cast<unsigned long>(-1L));
    ++playerCalls;
    return playerStorage.pointer();
}
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return hero; }
CThing* CTCCarrying::GetThingInPrimarySlot() const { ++carryingCalls; return item; }
bool CTCWeapon::IsUsableBy(const CThing& thing) const
{
    assert(&thing == hero && this == weaponStorage.pointer());
    ++usableCalls;
    return usable;
}
void CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector& v, EGameAction action)
{
    assert(this == hero && v.X == xInput && v.Y == yInput && v.Z == 0 && action == actionInput);
    ++moves;
}
typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
static Entry carryingEntry, weaponEntry;
static void setComponent(unsigned char* bytes, ETCInterfaceType key, CTCBase* component, Entry& entry)
{
    entry.first = key;
    entry.second = component;
    unsigned long bit = 1UL << (key % 32);
    memcpy(bytes + 0x20 + 4 * (key / 32), &bit, 4);
    Entry* first = &entry;
    Entry* last = first + 1;
    memcpy(bytes + 0x44, &first, sizeof(first));
    memcpy(bytes + 0x48, &last, sizeof(last));
}
int main()
{
    for (int scenario = 0; scenario < 8; ++scenario) {
        memset(heroStorage.bytes, 0, sizeof(heroStorage.bytes));
        memset(itemStorage.bytes, 0, sizeof(itemStorage.bytes));
        hero = scenario == 0 ? 0 : heroStorage.pointer();
        heroStorage.bytes[0x91] = scenario == 1;
        if (scenario >= 3)
            setComponent(heroStorage.bytes, TCI_CARRYING, carryingStorage.pointer(), carryingEntry);
        item = scenario >= 4 ? itemStorage.pointer() : 0;
        if (scenario >= 5)
            setComponent(itemStorage.bytes, TCI_WEAPON, scenario == 5 ? 0 : weaponStorage.pointer(), weaponEntry);
        usable = scenario == 7;
        playerCalls = carryingCalls = usableCalls = moves = 0;
        CGameEvent event;
        event.Player = -1;
        worldStorage.pointer()->EAControlledCreatureStrafe(&event);
        assert(reads == 4 && playerCalls == 1);
        assert(carryingCalls == (scenario >= 3) && usableCalls == (scenario >= 6));
        assert(moves == (scenario == 2 || scenario == 3 || scenario == 4 || scenario == 7));
    }
    puts("0062bd80_TEST PASS");
    return 0;
}