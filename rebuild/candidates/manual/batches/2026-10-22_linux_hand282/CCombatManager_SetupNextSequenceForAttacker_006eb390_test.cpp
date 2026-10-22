// Behaviour test for 0x006EB390 CCombatManager::SetupNextSequenceForAttacker: the source's model and body, with
// working bodies for the out-of-line callees (combatant map find, interface lookups, weapon / carrying / combat
// components, intelligent pointer, sequence virtuals, dialogue manager, manager helpers) that record what the body
// does. Scenarios: every weapon refusal on both branches (no weapon, dead weapon, no weapon component, wrong weapon
// kind), a negative best index, a ranged sequence that only speaks (with and without a speech), a melee sequence that
// marks the last attacker and applies the block response, a break-into-melee sequence that stops when the response
// ends the attack, and one without a combat component that marks and speaks. The def reference and every intelligent
// pointer temporary must be balanced.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

struct CTCBase;
struct CBaseObject {};
struct CThing;

struct CInterfaceMap {                                    // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
    const SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
static int g_lowerBoundCalls = 0;
static int g_constLowerBoundCalls = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lowerBoundCalls;
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    ++g_constLowerBoundCalls;
    const SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}

static int g_defDestroyed = 0;
struct CThingDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_defDestroyed; }          // slot 1
    long RefCount;                                        // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CCreatureDef : CThingDef {
    unsigned char _pad08[0x148 - 0x08];
    unsigned char Flags148;                               // +0x148 (bit 5: a melee attacker needs a melee weapon)
};
template<class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}                            // test only: things are built here
    CDefPointer(const CDefPointer& other) : Data(other.Data) { if (Data) Data->AddRef(); }
    ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};

struct CTCCarrying {
    CThing* PrimarySlot;                                  // test: the item in the primary slot
    CThing* GetThingInPrimarySlot() const { return PrimarySlot; }   // 0x006A1440
};
struct CTCWeapon {
    bool Projectile;                                      // test
    bool Melee;                                           // test
    bool IsProjectileWeapon() const { return Projectile; }    // 0x005DAA40
    bool IsMeleeWeapon() const { return Melee; }              // 0x005DAA00
};
struct CThingCreatureBase;
struct CTCCombat {
    CThingCreatureBase* Owner;                            // test: the creature this component belongs to
    bool EndsAttack;                                      // test: the response takes the owner out of melee
    int Calls;                                            // test
    CThing* LastAttacked;                                 // test
    void ApplyBlockResponse(CThing* attacked);            // 0x00733CF0
};

struct CThing : CBaseObject {
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                            // +0x18
    unsigned long UsingComponents[9];                     // +0x20
    CInterfaceMap Components;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CCreatureDef> Def;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                // +0x91 bit 0: dead
    unsigned __int64 GetUID() const { return UniqueID; }
    CDefPointer<CCreatureDef> GetDef() const { return Def; }
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
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
struct CThingCreatureBase : CThing {
    long RangedWeaponDef;                                 // test
    long MeleeWeaponDef;                                  // test
    bool InMeleeAsAttacker;                               // test
    long GetRangedWeaponDef() const { return RangedWeaponDef; }   // 0x00662400
    long GetMeleeWeaponDef() const { return MeleeWeaponDef; }     // 0x006623F0
    bool IsInMeleeAsAttacker() const { return InMeleeAsAttacker; }   // 0x00662410
};
void CTCCombat::ApplyBlockResponse(CThing* attacked)
{
    ++Calls;
    LastAttacked = attacked;
    if (EndsAttack)
        Owner->InMeleeAsAttacker = false;
}

static int g_livePointers = 0;
static int g_setPItemCalls = 0;
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer() : PData(0) { ++g_livePointers; }   // 0x00A01B10
    virtual ~CBaseIntelligentPointer() { --g_livePointers; }     // 0x00A01C10
    CBaseObject* GetPItem() const { return PData; }              // 0x00A01B50
    void SetPItem(CBaseObject* item) { ++g_setPItemCalls; PData = item; }   // 0x00A01B90
    CBaseObject* PData;                                   // +0x04
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPointer(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPointer(other.GetPointer()); return *this; }
    T* operator->() const { return GetPointer(); }
    T* GetPointer() const { return static_cast<T*>(GetPItem()); }
    void SetPointer(T* item) { SetPItem(item); }
};

struct CCombatSequenceBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual bool IsMeleeSequence() const = 0;             // slot 7 (+0x1C), 0x006F0700
    virtual bool IsBreakIntoMeleeSequence() const = 0;    // slot 8 (+0x20), 0x006F0710
};
static int g_meleeQueries = 0;
static int g_breakQueries = 0;
struct CTestSequence : CCombatSequenceBase {
    bool Melee;
    bool BreakIntoMelee;
    CTestSequence(bool melee, bool breakIntoMelee) : Melee(melee), BreakIntoMelee(breakIntoMelee) {}
    bool IsMeleeSequence() const { ++g_meleeQueries; return Melee; }
    bool IsBreakIntoMeleeSequence() const { ++g_breakQueries; return BreakIntoMelee; }
};
struct CSequencePointer {                                 // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};
template<class T> struct CRetailVector {                  // STLport vector, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
};
struct CAttackingCombatantInfo {                          // retail 0x30 bytes
    CThingCreatureBase* Attacked;                         // +0x00
    long AttackerNumber;                                  // +0x04
    long CombatWheelRingIndex;                            // +0x08
    bool PreferredAttackDollySet;                         // +0x0c
    long PreferredAttackDollyTimer;                       // +0x10
    float PreferredAttackDollyVector[2];                  // +0x14
    bool UsingCombatWheel;                                // +0x1c
    long CurrentSequence;                                 // +0x20
    CRetailVector<CSequencePointer> CombatSequences;      // +0x24
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                           // +0x00
    CAttackingCombatantInfo AttackerInfo;                 // +0x04
    unsigned char Attackers[0x0c];                        // +0x34
    unsigned char CombatWheel[0x0c];                      // +0x40
    float LastVictimWheelUpdatePos[3];                    // +0x4c
    long LastWheelUpdateWorldFrame;                       // +0x58
    CIntelligentPointer<CThing> PLastAttacker;            // +0x5c
};
struct CCombatantMap {                                    // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long colour; node* parent; node* left; node* right;
        unsigned __int64 key;                             // +0x10
        CCombatantInfo value;                             // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);              // 0x006F0250
};
static int g_findCalls = 0;
static CCombatantMap::node* g_nodes[4];
static int g_nodeCount = 0;
CCombatantMap::node* CCombatantMap::find(const unsigned __int64& key)
{
    ++g_findCalls;
    for (int i = 0; i < g_nodeCount; ++i)
        if (g_nodes[i]->key == key)
            return g_nodes[i];
    return header;
}
struct CCombatManager;
struct CombatDialogueManager {
    unsigned long NextSpeech;                             // test: what GetBestSpeechForCreature answers
    int BestCalls;                                        // test
    int SayCalls;                                         // test
    const CThingCreatureBase* BestSpeaker;                // test
    const CThingCreatureBase* BestTarget;                 // test
    const CCombatManager* BestManager;                    // test
    CThingCreatureBase* SaidBy;                           // test
    unsigned long SaidSpeech;                             // test
    unsigned long GetBestSpeechForCreature(const CThingCreatureBase& speaker, const CThingCreatureBase& target,
                                           const CCombatManager& manager) const;   // 0x008AF990
    bool SaySpeech(CThingCreatureBase& speaker, unsigned long speech);   // 0x008AFC40
};
unsigned long CombatDialogueManager::GetBestSpeechForCreature(const CThingCreatureBase& speaker,
                                                              const CThingCreatureBase& target,
                                                              const CCombatManager& manager) const
{
    CombatDialogueManager* self = const_cast<CombatDialogueManager*>(this);
    ++self->BestCalls;
    self->BestSpeaker = &speaker;
    self->BestTarget = &target;
    self->BestManager = &manager;
    return NextSpeech;
}
bool CombatDialogueManager::SaySpeech(CThingCreatureBase& speaker, unsigned long speech)
{
    ++SayCalls;
    SaidBy = &speaker;
    SaidSpeech = speech;
    return true;
}
struct CCombatManager {
    virtual void v00() {}
    unsigned char _sequences[0xc];                        // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                             // +0x10
    bool CombatStrafeTurningEnabled;                      // +0x1c
    void* World;                                          // +0x20
    unsigned char _pad24[0x38 - 0x24];
    CombatDialogueManager DialogueManager;                // +0x38
    // test state
    int ResetCalls;
    CThingCreatureBase* ResetCreature;
    bool OnRotatingRing;
    long BestIndex;
    int BestIndexCalls;
    const CCombatantInfo* BestAttacker;
    const CCombatantInfo* BestDefender;
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUID())->value; }
    void PerformSequenceReset(CThingCreatureBase& creature) { ++ResetCalls; ResetCreature = &creature; }   // 0x006EA2C0
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase&) const { return OnRotatingRing; }   // 0x006E9700
    long GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker, const CCombatantInfo& defender) const;   // 0x006EAE20
    void SetupNextSequenceForAttacker(CCombatantInfo& info);
};
long CCombatManager::GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker, const CCombatantInfo& defender) const
{
    CCombatManager* self = const_cast<CCombatManager*>(this);
    ++self->BestIndexCalls;
    self->BestAttacker = &attacker;
    self->BestDefender = &defender;
    return BestIndex;
}

// ---- the source body (src/006eb390.cpp) ----
void CCombatManager::SetupNextSequenceForAttacker(CCombatantInfo& info)
{
    CThingCreatureBase* defender = info.AttackerInfo.Attacked;
    CThingCreatureBase* attacker = info.Parent;
    const CCreatureDef* def = attacker->GetDef().Data;
    CThingCreatureBase& attacked = *info.AttackerInfo.Attacked;
    CCombatantInfo& defenderInfo = GetCombatantInfo(attacked);
    PerformSequenceReset(*attacker);
    CThing* weapon = 0;
    if (attacker->HasTC(0x46))
        weapon = attacker->GetConstTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
    if (IsCreatureOnRotatingCombatRing(*attacker)) {
        if (attacker->GetRangedWeaponDef() > 0) {
            if (weapon == 0 || (weapon->Flags91 & 1) || !weapon->HasTC(0x48))
                return;
            if (!weapon->GetConstTC<CTCWeapon>(0x48)->IsProjectileWeapon())
                return;
        }
    } else if (def->Flags148 & 0x20) {
        if (attacker->GetMeleeWeaponDef() > 0) {
            if (weapon == 0 || (weapon->Flags91 & 1) || !weapon->HasTC(0x48))
                return;
            if (!weapon->GetConstTC<CTCWeapon>(0x48)->IsMeleeWeapon())
                return;
        }
    }
    long index = GetBestSequenceIndexAttackerOnDefender(info, defenderInfo);
    info.AttackerInfo.CurrentSequence = index;
    if (index < 0)
        return;
    CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences.first[index].Object;
    if (sequence->IsMeleeSequence() || sequence->IsBreakIntoMeleeSequence()) {
        defenderInfo.PLastAttacker = CIntelligentPointer<CThing>(attacker);
        CTCCombat* combat;
        if (attacker->GetInterface(&combat, 0x30)) {
            combat->ApplyBlockResponse(defender);
            if (!attacker->IsInMeleeAsAttacker())
                return;
        }
    }
    if (!sequence->IsMeleeSequence()) {
        unsigned long speech = DialogueManager.GetBestSpeechForCreature(*attacker, attacked, *this);
        if (speech)
            DialogueManager.SaySpeech(*attacker, speech);
    }
}
// ---- end of the source body ----

static void InitThing(CThing& thing, unsigned __int64 uid, CCreatureDef* def, CInterfaceMap::SEntry* entries, int count)
{
    memset(thing.UsingComponents, 0, sizeof(thing.UsingComponents));
    thing.UniqueID = uid;
    thing.Def.Data = def;
    thing.Flags91 = 0;
    thing.Components.first = entries;
    thing.Components.last = entries + count;
    for (int i = 0; i < count; ++i)
        thing.UsingComponents[entries[i].type >> 5] |= 1UL << (entries[i].type & 31);
}
static void InitCreature(CThingCreatureBase& creature, unsigned __int64 uid, CCreatureDef* def,
                         CInterfaceMap::SEntry* entries, int count)
{
    InitThing(creature, uid, def, entries, count);
    creature.RangedWeaponDef = 0;
    creature.MeleeWeaponDef = 0;
    creature.InMeleeAsAttacker = true;
}
static void ResetManager(CCombatManager& manager)
{
    manager.ResetCalls = 0;
    manager.ResetCreature = 0;
    manager.OnRotatingRing = false;
    manager.BestIndex = -1;
    manager.BestIndexCalls = 0;
    manager.BestAttacker = 0;
    manager.BestDefender = 0;
    manager.DialogueManager.NextSpeech = 0;
    manager.DialogueManager.BestCalls = 0;
    manager.DialogueManager.SayCalls = 0;
    manager.DialogueManager.BestSpeaker = 0;
    manager.DialogueManager.BestTarget = 0;
    manager.DialogueManager.BestManager = 0;
    manager.DialogueManager.SaidBy = 0;
    manager.DialogueManager.SaidSpeech = 0;
}
static int Fail(int line)
{
    printf("006eb390_TEST FAIL\n");
    fprintf(stderr, "check failed at line %d\n", line);
    return 1;
}
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)

int main()
{
    CCreatureDef plainDef, meleeDef, weaponDef;
    plainDef.RefCount = 1; plainDef.Flags148 = 0;
    meleeDef.RefCount = 1; meleeDef.Flags148 = 0x20;
    weaponDef.RefCount = 1; weaponDef.Flags148 = 0;

    // weapons: a bow (projectile), a sword (melee), a stick with no weapon component
    CTCWeapon bowWeapon; bowWeapon.Projectile = true; bowWeapon.Melee = false;
    CTCWeapon swordWeapon; swordWeapon.Projectile = false; swordWeapon.Melee = true;
    CInterfaceMap::SEntry bowEntries[1] = { { 0x48, (CTCBase*)&bowWeapon } };
    CInterfaceMap::SEntry swordEntries[1] = { { 0x48, (CTCBase*)&swordWeapon } };
    CThing bow, sword, stick;
    InitThing(bow, 900, &weaponDef, bowEntries, 1);
    InitThing(sword, 901, &weaponDef, swordEntries, 1);
    InitThing(stick, 902, &weaponDef, 0, 0);

    // the attacker carries something (0x46) and has a combat component (0x30); the entries are sorted by type
    CTCCarrying carrying; carrying.PrimarySlot = 0;
    CThingCreatureBase hero, bandit;
    CTCCombat combat; combat.Owner = &hero; combat.EndsAttack = false; combat.Calls = 0; combat.LastAttacked = 0;
    CInterfaceMap::SEntry heroEntries[2] = { { 0x30, (CTCBase*)&combat }, { 0x46, (CTCBase*)&carrying } };
    InitCreature(hero, 100, &plainDef, heroEntries, 2);
    InitCreature(bandit, 200, &plainDef, 0, 0);

    CTestSequence meleeSequence(true, false), rangedSequence(false, false), breakSequence(false, true);
    CSequencePointer sequences[3];
    sequences[0].Object = &meleeSequence; sequences[0].Count = 0;
    sequences[1].Object = &rangedSequence; sequences[1].Count = 0;
    sequences[2].Object = &breakSequence; sequences[2].Count = 0;

    CCombatantMap::node heroNode, banditNode;
    heroNode.key = 100; banditNode.key = 200;
    CCombatantInfo& info = heroNode.value;
    CCombatantInfo& banditInfo = banditNode.value;
    info.Parent = &hero;
    info.AttackerInfo.Attacked = &bandit;
    info.AttackerInfo.CurrentSequence = 77;
    info.AttackerInfo.CombatSequences.first = sequences;
    info.AttackerInfo.CombatSequences.last = sequences + 3;
    info.AttackerInfo.CombatSequences.end_of_storage = sequences + 3;
    banditInfo.Parent = &bandit;
    g_nodes[0] = &heroNode; g_nodes[1] = &banditNode; g_nodeCount = 2;

    CCombatManager manager;
    CCombatantMap::node sentinel;
    manager.Combatants.header = &sentinel;
    ResetManager(manager);
    const int liveBase = g_livePointers;                  // the three map nodes' PLastAttacker members

    // 1. on the rotating ring with a ranged weapon def but nothing in hand: refused after the reset
    manager.OnRotatingRing = true;
    hero.RangedWeaponDef = 3;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.ResetCalls == 1 && manager.ResetCreature == &hero);
    CHECK(g_findCalls == 1 && manager.BestIndexCalls == 0 && info.AttackerInfo.CurrentSequence == 77);
    CHECK(plainDef.RefCount == 1 && g_defDestroyed == 0);

    // 2. a dead bow, 3. a stick without a weapon component, 4. a sword (not a projectile weapon): all refused
    carrying.PrimarySlot = &bow; bow.Flags91 = 1;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.BestIndexCalls == 0);
    bow.Flags91 = 0;
    carrying.PrimarySlot = &stick;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.BestIndexCalls == 0);
    carrying.PrimarySlot = &sword;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.BestIndexCalls == 0 && manager.ResetCalls == 4);

    // 5. the bow passes; no sequence fits (-1): the index is stored and nothing else happens
    carrying.PrimarySlot = &bow;
    manager.BestIndex = -1;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.BestIndexCalls == 1 && manager.BestAttacker == &info && manager.BestDefender == &banditInfo);
    CHECK(info.AttackerInfo.CurrentSequence == -1);
    CHECK(g_meleeQueries == 0 && manager.DialogueManager.BestCalls == 0);

    // 6. a ranged sequence: not the last attacker, no block response; it asks for a speech but gets none
    manager.BestIndex = 1;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(info.AttackerInfo.CurrentSequence == 1);
    CHECK(g_meleeQueries == 2 && g_breakQueries == 1);
    CHECK(banditInfo.PLastAttacker.GetPItem() == 0 && combat.Calls == 0 && g_setPItemCalls == 0);
    CHECK(manager.DialogueManager.BestCalls == 1 && manager.DialogueManager.SayCalls == 0);
    CHECK(manager.DialogueManager.BestSpeaker == &hero && manager.DialogueManager.BestTarget == &bandit);
    CHECK(manager.DialogueManager.BestManager == &manager);

    // 7. the same with a speech: it is said by the attacker
    manager.DialogueManager.NextSpeech = 42;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.DialogueManager.SayCalls == 1 && manager.DialogueManager.SaidBy == &hero);
    CHECK(manager.DialogueManager.SaidSpeech == 42);

    // 8. off the ring, the def demands a melee weapon and the bow is not one: refused
    ResetManager(manager);
    hero.Def.Data = &meleeDef;
    hero.MeleeWeaponDef = 5;
    manager.BestIndex = 0;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(manager.ResetCalls == 1 && manager.BestIndexCalls == 0);
    CHECK(meleeDef.RefCount == 1);

    // 9. with the sword, a melee sequence: the hero becomes the bandit's last attacker and the block response runs;
    //    the hero stays in melee, and a melee sequence says nothing
    carrying.PrimarySlot = &sword;
    int meleeBefore = g_meleeQueries;
    int breakBefore = g_breakQueries;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(info.AttackerInfo.CurrentSequence == 0);
    CHECK(banditInfo.PLastAttacker.GetPItem() == static_cast<CBaseObject*>(&hero));
    CHECK(info.PLastAttacker.GetPItem() == 0);
    CHECK(combat.Calls == 1 && combat.LastAttacked == &bandit);
    CHECK(g_meleeQueries == meleeBefore + 2 && g_breakQueries == breakBefore);
    CHECK(manager.DialogueManager.BestCalls == 0);
    CHECK(g_setPItemCalls == 2 && g_livePointers == liveBase);

    // 10. a def without the melee demand skips the weapon checks; a break-into-melee sequence marks the last
    //     attacker, and the block response ends the hero's attack: nothing is said
    hero.Def.Data = &plainDef;
    carrying.PrimarySlot = 0;
    banditInfo.PLastAttacker.SetPItem(0);
    combat.EndsAttack = true;
    manager.BestIndex = 2;
    manager.DialogueManager.NextSpeech = 9;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(info.AttackerInfo.CurrentSequence == 2);
    CHECK(banditInfo.PLastAttacker.GetPItem() == static_cast<CBaseObject*>(&hero));
    CHECK(combat.Calls == 2 && hero.InMeleeAsAttacker == false);
    CHECK(manager.DialogueManager.BestCalls == 0 && manager.DialogueManager.SayCalls == 0);

    // 11. the same without a combat component: no response, and the non-melee sequence speaks
    hero.UsingComponents[0x30 >> 5] &= ~(1UL << (0x30 & 31));
    hero.InMeleeAsAttacker = true;
    banditInfo.PLastAttacker.SetPItem(0);
    manager.SetupNextSequenceForAttacker(info);
    CHECK(banditInfo.PLastAttacker.GetPItem() == static_cast<CBaseObject*>(&hero));
    CHECK(combat.Calls == 2);
    CHECK(manager.DialogueManager.BestCalls == 1 && manager.DialogueManager.SayCalls == 1);
    CHECK(manager.DialogueManager.SaidSpeech == 9 && manager.DialogueManager.SaidBy == &hero);

    // 12. with the component back and the hero staying in melee, the break-into-melee sequence responds and speaks
    hero.UsingComponents[0x30 >> 5] |= 1UL << (0x30 & 31);
    combat.EndsAttack = false;
    manager.SetupNextSequenceForAttacker(info);
    CHECK(combat.Calls == 3 && hero.InMeleeAsAttacker == true);
    CHECK(manager.DialogueManager.BestCalls == 2 && manager.DialogueManager.SayCalls == 2);

    // the def references and the intelligent pointer temporaries are balanced
    CHECK(plainDef.RefCount == 1 && meleeDef.RefCount == 1 && g_defDestroyed == 0);
    CHECK(liveBase == 3 && g_livePointers == liveBase);
    CHECK(g_constLowerBoundCalls > 0 && g_lowerBoundCalls == 3);
    printf("006eb390_TEST PASS\n");
    return 0;
}
