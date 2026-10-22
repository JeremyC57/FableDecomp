// Behaviour test for 0x008CDA30 CCreatureAction_BattleChargeMiss::FinishAction: the source's model and body, with working
// bodies for the out-of-line callees (base FinishAction, intelligent pointer, interface lookup, particle emitter, the
// unsheathe action and SetCurrentAction) that record what the body does. Scenarios: everything present (ability lock
// released, both feet effects re-attached, the removed item re-equipped), nothing alive (null and dead pointers, no 0xA7
// component), and single-foot cases; every string and action temporary must be released.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif

static int g_liveStrings = 0;
struct CTCBase;
struct CThing;
struct CBaseObject;

struct CCharString {
    char* PStringData;
    CCharString(const char* text, long length = -1)
    {
        size_t n = length < 0 ? strlen(text) : (size_t)length;
        PStringData = (char*)malloc(n + 1);
        memcpy(PStringData, text, n);
        PStringData[n] = 0;
        ++g_liveStrings;
    }
    ~CCharString() { free(PStringData); --g_liveStrings; }
private:
    CCharString(const CCharString&);
    CCharString& operator=(const CCharString&);
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CThingCreatureBase;
static int g_liveActions = 0;
struct CCreatureActionBase {
    virtual void v00() {}
    virtual ~CCreatureActionBase() {}
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    CThingCreatureBase* Owner;
    char Animation[64];
    CThing* Item;
    long Priority;
    long Timeout;
    CCreatureActionBase* Next;
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& owner, const CCharString& animation, CThing& item,
                                               long priority, long timeout, CCreatureActionBase* next)
        : Owner(&owner), Item(&item), Priority(priority), Timeout(timeout), Next(next)
    {
        strcpy(Animation, animation.PStringData);
        ++g_liveActions;
    }
    ~CCreatureAction_UnsheatheItemFromInventory() { --g_liveActions; }
};

struct CThing {
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

// SetCurrentAction record
static int g_setActionCalls = 0;
static CThingCreatureBase* g_setActionOn = 0;
static CThingCreatureBase* g_setActionOwner = 0;
static char g_setActionAnimation[64];
static CThing* g_setActionItem = 0;
static long g_setActionPriority = 0, g_setActionTimeout = 0;
static CCreatureActionBase* g_setActionNext = (CCreatureActionBase*)1;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action)
    {
        const CCreatureAction_UnsheatheItemFromInventory& a = (const CCreatureAction_UnsheatheItemFromInventory&)action;
        ++g_setActionCalls;
        g_setActionOn = this;
        g_setActionOwner = a.Owner;
        strcpy(g_setActionAnimation, a.Animation);
        g_setActionItem = a.Item;
        g_setActionPriority = a.Priority;
        g_setActionTimeout = a.Timeout;
        g_setActionNext = a.Next;
        return true;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* PData;
    CBaseObject* GetPItem() const { return PData; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
template<class T> inline bool IS_THING_ALIVE(const T& p)
{
    if (p.GetPItem() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

struct CTCSpecialAbilities {
    bool AbilityMutexLock;
    long CurrentAbility;
};

// AttachToThing record
struct SAttach { const void* emitter; CThing* thing; long attachType; char helperName[16]; long helperIndex; float offset; };
static SAttach g_attach[4];
static int g_attachCount = 0;
struct CTCDParticleEmitter {
    int Id;
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset)
    {
        SAttach& r = g_attach[g_attachCount++];
        r.emitter = this;
        r.thing = &thing;
        r.attachType = attachType;
        strcpy(r.helperName, helperName.PStringData);
        r.helperIndex = helperIndex;
        r.offset = offset;
    }
};

static int g_baseFinishCalls = 0;
struct CCombatActionBase {
    virtual void v00() {}
    CIntelligentPointer<CThingCreatureBase> PCreature;
    CIntelligentPointer<CThing> PItemRemoved;
    void FinishAction() { ++g_baseFinishCalls; }
};
struct CCreatureAction_BattleChargeMiss : CCombatActionBase {
    CIntelligentPointer<CThing> Left;
    CIntelligentPointer<CThing> Right;
    void FinishAction();
};

void CCreatureAction_BattleChargeMiss::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpecialAbilities* abilities;
    if (PCreature->GetInterface(&abilities, 0xa7)) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    CTCDParticleEmitter* emitter;
    if (IS_THING_ALIVE(Left)) {
        Left->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(Right)) {
        Right->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        PCreature->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*PCreature, animation, *PItemRemoved, 100, 250, 0));
    }
}

// ---- harness ----
static void ClearThing(CThing& t)
{
    memset(t.UsingComponents, 0, sizeof(t.UsingComponents));
    t.Components.first = t.Components.last = 0;
    t.Flags91 = 0;
}
static void SetEntries(CThing& t, CInterfaceMap::SEntry* entries, int count)
{
    t.Components.first = entries;
    t.Components.last = entries + count;
    for (int i = 0; i < count; ++i)
        t.UsingComponents[entries[i].type >> 5] |= 1UL << (entries[i].type & 31);
}
static void Point(CBaseIntelligentPointer& p, CThing* thing) { p.PData = (CBaseObject*)thing; }
static void ResetRecords()
{
    g_attachCount = 0;
    memset(g_attach, 0, sizeof(g_attach));
    g_setActionCalls = 0;
    g_setActionOn = g_setActionOwner = 0;
    g_setActionAnimation[0] = 0;
    g_setActionItem = 0;
    g_setActionPriority = g_setActionTimeout = 0;
    g_setActionNext = (CCreatureActionBase*)1;
    g_baseFinishCalls = 0;
}
static int g_fail = 0;
static void Check(int ok, int line)
{
    if (!ok && !g_fail) {
        g_fail = 1;
        printf("008cda30_TEST FAIL\n");
        fprintf(stderr, "check failed at line %d\n", line);
    }
}
#define CHECK(x) Check((x) ? 1 : 0, __LINE__)

int main()
{
    CThingCreatureBase creature;
    CThing leftFx, rightFx, item;
    CTCSpecialAbilities abilities;
    CTCDParticleEmitter leftEmitter, rightEmitter;
    leftEmitter.Id = 1;
    rightEmitter.Id = 2;
    CInterfaceMap::SEntry creatureEntries[3] = { { 0x30, 0 }, { 0xa7, (CTCBase*)&abilities }, { 0xb0, 0 } };
    CInterfaceMap::SEntry leftEntries[2] = { { 0x10, 0 }, { 0x4b, (CTCBase*)&leftEmitter } };
    CInterfaceMap::SEntry rightEntries[1] = { { 0x4b, (CTCBase*)&rightEmitter } };

    // 1. everything alive: ability lock released, both feet re-attached, the removed item re-equipped
    {
        ClearThing(creature); ClearThing(leftFx); ClearThing(rightFx); ClearThing(item);
        SetEntries(creature, creatureEntries, 3);
        SetEntries(leftFx, leftEntries, 2);
        SetEntries(rightFx, rightEntries, 1);
        abilities.AbilityMutexLock = true;
        abilities.CurrentAbility = 12;
        CCreatureAction_BattleChargeMiss action;
        Point(action.PCreature, &creature);
        Point(action.PItemRemoved, &item);
        Point(action.Left, &leftFx);
        Point(action.Right, &rightFx);
        ResetRecords();
        action.FinishAction();
        CHECK(g_baseFinishCalls == 1);
        CHECK(abilities.AbilityMutexLock == false);
        CHECK(abilities.CurrentAbility == 0);
        CHECK(g_attachCount == 2);
        CHECK(g_attach[0].emitter == &leftEmitter && g_attach[1].emitter == &rightEmitter);
        for (int i = 0; i < 2; ++i) {
            CHECK(g_attach[i].thing == &creature);
            CHECK(g_attach[i].attachType == 1);
            CHECK(strcmp(g_attach[i].helperName, "") == 0);
            CHECK(g_attach[i].helperIndex == 0);
            CHECK(g_attach[i].offset == 0.0f);
        }
        CHECK(g_setActionCalls == 1);
        CHECK(g_setActionOn == &creature);
        CHECK(g_setActionOwner == &creature);
        CHECK(strcmp(g_setActionAnimation, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") == 0);
        CHECK(g_setActionItem == &item);
        CHECK(g_setActionPriority == 100);
        CHECK(g_setActionTimeout == 250);
        CHECK(g_setActionNext == 0);
        CHECK(g_liveStrings == 0);
        CHECK(g_liveActions == 0);
    }
    // 2. nothing to do: no 0xA7 component, Left null, Right dead, removed item dead
    {
        ClearThing(creature); ClearThing(leftFx); ClearThing(rightFx); ClearThing(item);
        SetEntries(creature, creatureEntries, 1);           // only 0x30
        SetEntries(rightFx, rightEntries, 1);
        rightFx.Flags91 = 1;
        item.Flags91 = 3;
        abilities.AbilityMutexLock = true;
        abilities.CurrentAbility = 7;
        CCreatureAction_BattleChargeMiss action;
        Point(action.PCreature, &creature);
        Point(action.PItemRemoved, &item);
        Point(action.Left, 0);
        Point(action.Right, &rightFx);
        ResetRecords();
        action.FinishAction();
        CHECK(g_baseFinishCalls == 1);
        CHECK(abilities.AbilityMutexLock == true);
        CHECK(abilities.CurrentAbility == 7);
        CHECK(g_attachCount == 0);
        CHECK(g_setActionCalls == 0);
        CHECK(g_liveStrings == 0);
        CHECK(g_liveActions == 0);
    }
    // 3. left foot only (right null), no removed item; the dead bit is bit 0 only
    {
        ClearThing(creature); ClearThing(leftFx); ClearThing(rightFx); ClearThing(item);
        SetEntries(creature, creatureEntries + 1, 2);       // 0xa7, 0xb0
        SetEntries(leftFx, leftEntries, 2);
        leftFx.Flags91 = 2;                                 // other flag bits do not mean dead
        abilities.AbilityMutexLock = true;
        abilities.CurrentAbility = 3;
        CCreatureAction_BattleChargeMiss action;
        Point(action.PCreature, &creature);
        Point(action.PItemRemoved, 0);
        Point(action.Left, &leftFx);
        Point(action.Right, 0);
        ResetRecords();
        action.FinishAction();
        CHECK(abilities.AbilityMutexLock == false && abilities.CurrentAbility == 0);
        CHECK(g_attachCount == 1);
        CHECK(g_attach[0].emitter == &leftEmitter && g_attach[0].thing == &creature);
        CHECK(g_setActionCalls == 0);
        CHECK(g_liveStrings == 0);
    }
    // 4. right foot only (left dead) and the removed item alive
    {
        ClearThing(creature); ClearThing(leftFx); ClearThing(rightFx); ClearThing(item);
        SetEntries(creature, creatureEntries, 3);
        SetEntries(leftFx, leftEntries, 2);
        SetEntries(rightFx, rightEntries, 1);
        leftFx.Flags91 = 1;
        CCreatureAction_BattleChargeMiss action;
        Point(action.PCreature, &creature);
        Point(action.PItemRemoved, &item);
        Point(action.Left, &leftFx);
        Point(action.Right, &rightFx);
        ResetRecords();
        action.FinishAction();
        CHECK(g_attachCount == 1);
        CHECK(g_attach[0].emitter == &rightEmitter && g_attach[0].thing == &creature);
        CHECK(g_setActionCalls == 1 && g_setActionOn == &creature && g_setActionItem == &item);
        CHECK(g_liveStrings == 0);
        CHECK(g_liveActions == 0);
    }
    if (g_fail)
        return 1;
    printf("008cda30_TEST PASS\n");
    return 0;
}
