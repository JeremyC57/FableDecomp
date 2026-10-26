// CAIStateGroupBase::CreateItem(CCharString&, CCharString&) -- look up the creature's carry slot, create the named thing at its position, then put it in the slot; retail 0x008fd0a0 (266 bytes), using CAIBrain/CThingAICreature headers and the opaque +0x44 map view.
#if defined(_MSC_VER)
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayerManager.h"
#include "fable_string.h"
#else
#define __fastcall
class CCharString { public:
    CCharString();
    CCharString(const char*, long);
    ~CCharString();
};
class CThingAICreature { public: unsigned char storage[0x100]; };
class CPlayer;
class CPlayerManager { public: long MainPlayer; };
class CAIBrain { public: CThingAICreature* Creature; CPlayerManager* PlayerManager; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
#endif

class C3DVector { public: float x, y, z; };
class CThing;
class CThingObject;
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(CCharString*); // 0x009ad410
};
// Retail getter 0x00449700 reads native MainPlayer at +0x1c; the donor header
// places it at +0x20 because its Players vector is four bytes larger.
struct CPlayerManagerAccess : CPlayerManager { long GetMainPlayer(); };
class CTCCarrying { public:
    void PutThingInCarrySlot(CThingObject*, long, bool); // 0x006a1f80
};

struct CAIStateGroupBaseCreateItem : CAIStateGroupBase {
    void CreateItem(CCharString*, CCharString*);
};

struct CLongPointerPair { long key; void* value; };
struct CVectorMapLong {
    CLongPointerPair* first;
    CLongPointerPair* last;
    CLongPointerPair* capacity;
    unsigned long flags;
    CLongPointerPair* LowerBound(const long& key); // 0x0040f020
};

CGameDefinitionManager* __fastcall GetGameDefinitionManager(); // 0x0044c6b0
CThingObject* __fastcall CreateObject(long definition, C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags,
                                      const CCharString& name); // 0x00703210
C3DVector* __fastcall GetPosition(CThingAICreature*); // 0x004c73d0

void CAIStateGroupBaseCreateItem::CreateItem(CCharString* definitionName, CCharString* itemName)
{
    CAIBrain* brain = PBrain;
    CThingAICreature* creature = brain->Creature;
    if ((*(unsigned char*)((char*)creature + 0x28) & 0x40) != 0)
    {
        CVectorMapLong* carries = (CVectorMapLong*)((char*)creature + 0x44);
        long key = 0x46;
        CLongPointerPair* entry = carries->LowerBound(key);
        if (entry == carries->last || entry->key > 0x46)
            entry = carries->last;
        CTCCarrying* carry = (CTCCarrying*)entry->value;

        long playerID = ((CPlayerManagerAccess*)brain->PlayerManager)->GetMainPlayer();
        C3DVector position = *GetPosition(PBrain->Creature);
        float orientation[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

        long definition = GetGameDefinitionManager()->GetDefGlobalIndexFromName(definitionName);
        if (definition != 0)
        {
            CThingObject* thing;
            {
                CCharString createName("", -1);
            thing = CreateObject(definition, &position, playerID, orientation, 0, createName);
            }
            long item = GetGameDefinitionManager()->GetDefGlobalIndexFromName(itemName);
            carry->PutThingInCarrySlot(thing, item, true);
        }
    }
}

#include <stdio.h>
#include <string.h>

static CGameDefinitionManager managerObject;
static CPlayerManagerAccess playerManagerObject;
static CPlayerManagerAccess* expectedManager;
static CAIStateGroupBaseCreateItem* activeGroup;
static CAIBrain* replacementBrain;
static CThingAICreature* expectedPositionCreature;
static C3DVector positionValue;
static char thingBytes[4];
static CThingObject* factoryResult;
static CTCCarrying carryNormal, carryFallback;
static CCharString* definitionArgument;
static CCharString* itemArgument;
static long definitionResult, itemResult;
static long playerIDResult;
static long lookupResults[4];
static int lookupCount, getPlayerCount, getPositionCount, createCount, putCount;
static int emptyStringConstructors, emptyStringDestructors;
static int events[32], eventCount;
static int lowerBoundMode;
static C3DVector createdPosition;
static long createdDefinition, createdPlayerID;
static float createdOrientation[6];
static unsigned long createdFlags;
static const CCharString* createdName;
static CTCCarrying* storedCarry;
static CThingObject* storedThing;
static long storedItem;
static bool storedFlag;

static void event(int code) { if (eventCount < 32) events[eventCount++] = code; }
CCharString::CCharString() {}
CCharString::CCharString(const char* text, long)
{
    if (text && text[0] == 0) { ++emptyStringConstructors; event(2); }
}
CCharString::~CCharString()
{
    // The temporary passed to the factory is the only empty constructed string.
    if (emptyStringConstructors > emptyStringDestructors) {
        ++emptyStringDestructors;
        event(4);
    }
}
long CPlayerManagerAccess::GetMainPlayer()
{
    ++getPlayerCount;
    if (this != expectedManager) return -0x1234;
    if (replacementBrain) activeGroup->PBrain = replacementBrain;
    return playerIDResult;
}
CLongPointerPair* CVectorMapLong::LowerBound(const long& key)
{
    if (lowerBoundMode == 1) return last;
    if (lowerBoundMode == 2) { first->key = key + 1; return first; }
    if (first == last) return last;
    if (first->key == key) return first;
    return last;
}
CGameDefinitionManager* __fastcall GetGameDefinitionManager() { return &managerObject; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(CCharString* name)
{
    event(lookupCount == 0 ? 1 : 5);
    if (lookupCount < 4)
        lookupResults[lookupCount] = name == definitionArgument ? definitionResult
            : name == itemArgument ? itemResult : -1;
    ++lookupCount;
    return name == definitionArgument ? definitionResult
        : name == itemArgument ? itemResult : -1;
}
CThingObject* __fastcall CreateObject(long definition, C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags,
                                      const CCharString& name)
{
    int i;
    ++createCount;
    event(3);
    createdDefinition = definition;
    createdPosition = *position;
    createdPlayerID = playerID;
    for (i = 0; i != 6; ++i) createdOrientation[i] = orientation[i];
    createdFlags = flags;
    createdName = &name;
    return factoryResult;
}
C3DVector* __fastcall GetPosition(CThingAICreature* creature)
{
    if (creature != expectedPositionCreature) return 0;
    ++getPositionCount;
    return &positionValue;
}
void CTCCarrying::PutThingInCarrySlot(CThingObject* thing, long item, bool flag)
{
    ++putCount;
    event(6);
    storedCarry = this;
    storedThing = thing;
    storedItem = item;
    storedFlag = flag;
}
static int fail(const char* message)
{
    printf("008fd0a0_TEST FAIL: %s\n", message);
    return 1;
}
static void resetCalls()
{
    lookupCount = getPlayerCount = getPositionCount = createCount = putCount = 0;
    emptyStringConstructors = emptyStringDestructors = eventCount = 0;
    memset(lookupResults, 0, sizeof(lookupResults));
    memset(events, 0, sizeof(events));
    storedCarry = 0; storedThing = 0; storedItem = -1; storedFlag = false;
    replacementBrain = 0;
    factoryResult = (CThingObject*)thingBytes;
}
static int eventSequence(const int* wanted, int count)
{
    int i;
    if (eventCount != count) return 0;
    for (i = 0; i != count; ++i) if (events[i] != wanted[i]) return 0;
    return 1;
}
int main()
{
    CAIStateGroupBaseCreateItem group;
    CAIBrain brain, swappedBrain;
    CThingAICreature creature, swappedCreature;
    CLongPointerPair row, sentinel;
    CVectorMapLong* map;
    CCharString defName, itemName;
    int i;
    const int expectedEvents[] = { 1, 2, 3, 4, 5, 6 };

    memset(&group, 0, sizeof(group)); memset(&brain, 0, sizeof(brain));
    memset(&swappedBrain, 0, sizeof(swappedBrain));
    memset(&creature, 0, sizeof(creature)); memset(&swappedCreature, 0, sizeof(swappedCreature));
    memset(&playerManagerObject, 0, sizeof(playerManagerObject));
    row.key = 0x46; row.value = &carryNormal;
    sentinel.key = 0; sentinel.value = &carryFallback;
    map = (CVectorMapLong*)((char*)&creature + 0x44);
    map->first = &row; map->last = &sentinel; map->capacity = &sentinel; map->flags = 0;
    ((unsigned char*)&creature)[0x28] = 0x40;
    brain.Creature = &creature; brain.PlayerManager = &playerManagerObject;
    group.PBrain = &brain;
    activeGroup = &group; expectedManager = &playerManagerObject;
    expectedPositionCreature = &creature;
    positionValue.x = 1.25f; positionValue.y = -2.5f; positionValue.z = 8.0f;
    definitionArgument = &defName; itemArgument = &itemName;
    definitionResult = 111; itemResult = 222; playerIDResult = -17;
    lowerBoundMode = 0; resetCalls();

    group.CreateItem(&defName, &itemName);
    if (lookupCount != 2 || lookupResults[0] != 111 || lookupResults[1] != 222)
        return fail("both definition names looked up in order");
    if (getPlayerCount != 1 || getPositionCount != 1 || createCount != 1)
        return fail("main-player getter, position, and factory each called once");
    if (createdDefinition != 111 || createdPlayerID != -17 || createdPosition.x != 1.25f ||
        createdPosition.y != -2.5f || createdPosition.z != 8.0f)
        return fail("factory receives definition, numeric main-player id, and copied position");
    if (createdFlags != 0 || !createdName || createdName == &defName || createdName == &itemName ||
        emptyStringConstructors != 1 || emptyStringDestructors != 1)
        return fail("factory receives a temporary empty name that is destroyed");
    for (i = 0; i != 6; ++i) {
        const float expected[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
        if (createdOrientation[i] != expected[i]) return fail("orientation values");
    }
    if (putCount != 1 || storedCarry != &carryNormal || storedThing != factoryResult ||
        storedItem != 222 || !storedFlag)
        return fail("thing is handed to selected carry with second definition and true");
    if (!eventSequence(expectedEvents, 6))
        return fail("lookup, temporary construction, factory, destruction, second lookup, carry order");

    // A factory null result is still handed to the selected carry.
    resetCalls(); factoryResult = 0;
    group.CreateItem(&defName, &itemName);
    if (putCount != 1 || storedThing != 0 || storedCarry != &carryNormal)
        return fail("null factory result still reaches carry helper");

    // Both native fallback routes select the end/sentinel entry's real carry value.
    resetCalls(); lowerBoundMode = 1;
    group.CreateItem(&defName, &itemName);
    if (putCount != 1 || storedCarry != &carryFallback)
        return fail("lower_bound equal to end uses sentinel carry");
    resetCalls(); lowerBoundMode = 2;
    group.CreateItem(&defName, &itemName);
    if (putCount != 1 || storedCarry != &carryFallback)
        return fail("key greater than 0x46 uses sentinel carry");
    lowerBoundMode = 0; row.key = 0x46;

    // The carry and player manager are captured from the initial brain, while position reloads PBrain.
    memset(&swappedBrain, 0, sizeof(swappedBrain)); memset(&swappedCreature, 0, sizeof(swappedCreature));
    swappedBrain.Creature = &swappedCreature; swappedBrain.PlayerManager = &playerManagerObject;
    expectedPositionCreature = &swappedCreature;
    replacementBrain = &swappedBrain;
    resetCalls(); replacementBrain = &swappedBrain;
    group.CreateItem(&defName, &itemName);
    if (getPositionCount != 1 || getPlayerCount != 1 || createdPlayerID != -17 ||
        storedCarry != &carryNormal || group.PBrain != &swappedBrain)
        return fail("position reloads changed brain while captured manager and carry stay original");

    // Gate and first lookup suppress all subsequent behavior.
    group.PBrain = &brain; expectedPositionCreature = &creature;
    ((unsigned char*)&creature)[0x28] = 0; resetCalls();
    group.CreateItem(&defName, &itemName);
    if (lookupCount || createCount || putCount || getPlayerCount || getPositionCount)
        return fail("disabled creation gate has no side effects");
    ((unsigned char*)&creature)[0x28] = 0x40; definitionResult = 0; resetCalls();
    group.CreateItem(&defName, &itemName);
    if (lookupCount != 1 || createCount || putCount || emptyStringConstructors)
        return fail("missing first definition prevents construction and second lookup");
    definitionResult = 111; itemResult = 0; resetCalls();
    group.CreateItem(&defName, &itemName);
    if (lookupCount != 2 || createCount != 1 || putCount != 1 || storedItem != 0)
        return fail("missing carry-item definition is passed to carry helper");

    printf("008fd0a0_TEST PASS\n");
    return 0;
}