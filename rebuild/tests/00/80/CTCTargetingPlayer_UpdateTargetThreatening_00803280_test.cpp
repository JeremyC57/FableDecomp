// Behaviour test for row 0x00803280 CTCTargetingPlayer::UpdateTargetThreatening (links the row's object; the CVectorMap LowerBound
// and CIntelligentPointer<CThing> bodies come from the row's own COMDATs). The test models the bytes the row reads: the thing type
// byte (+0x10), the component presence bits (+0x20) and the sorted {type, component} map (+0x44 / +0x48), CPlayer::ZTargeting
// (+0x20A), the targeting component's vtable slot 33 (GetPTargetedThing, +0x84), ParentThing (+0x04) and PLastThreatenedThing
// (+0xD4). Every condition is exercised on both sides: no target (pointer cleared), non-creature target, neither targeting mode,
// first-person targeting only, enemies willing / not willing / not enemies, no opinion-of-hero component, not aware (and the `true`
// flag), no deed log, no carrying, empty primary slot, no weapon, and the three weapon states choosing deed 41 / 40.
#include <stdio.h>
#include <string.h>

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
    CBaseObject* Item;
};
CBaseIntelligentPointer::CBaseIntelligentPointer() : Item(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return Item; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { Item = item; }
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: virtual ~CIntelligentPointer() { } };

class C3DVector { public: float X, Y, Z; C3DVector& AccurateNormalise(); };
// Pulled in by an anonymous-namespace helper the shared targeting headers compile into the row's object (never called here).
C3DVector& C3DVector::AccurateNormalise() { return *this; }
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
class CPlayer {
public:
    bool IsFirstPersonTargeting() const;
    unsigned char Pad[0x20A];
    bool ZTargeting;                                    // +0x20A
    bool FirstPerson;                                   // test only
};
bool CPlayer::IsFirstPersonTargeting() const { return FirstPerson; }

struct SPair { long Type; void* Component; };
class CThing {
public:
    CPlayer& GetPlayer();
    const C3DVector& GetPos() const;
    unsigned char Pad0[0x10];
    unsigned char Type;                                 // +0x10
    unsigned char Pad1[0x0F];
    unsigned long Bits[9];                              // +0x20
    SPair* MapBegin;                                    // +0x44
    SPair* MapEnd;                                      // +0x48
    SPair* MapCapacity;                                 // +0x4C
    unsigned char Pad2[0x40];
    SPair Pairs[4];
    int PairCount;
    CPlayer* Player;
    C3DVector Pos;
    void AddTC(long type, void* component)
    {
        Bits[type >> 5] |= 1UL << (type & 31);
        int i = PairCount++;
        while (i > 0 && Pairs[i - 1].Type > type) { Pairs[i] = Pairs[i - 1]; --i; }
        Pairs[i].Type = type; Pairs[i].Component = component;
        MapBegin = Pairs; MapEnd = Pairs + PairCount; MapCapacity = Pairs + 4;
    }
};
CPlayer& CThing::GetPlayer() { return *Player; }
const C3DVector& CThing::GetPos() const { return Pos; }

class CThingAICreature;
class CFaction;
enum EOpinionDeedType { DEED_DUMMY = 0x7fffffff };
enum EOpinionPostDeedType { POST_DUMMY = 0x7fffffff };

class CTCCarrying { public: CThing* Slot; CThing* GetThingInPrimarySlot() const; };
CThing* CTCCarrying::GetThingInPrimarySlot() const { return Slot; }
class CTCWeapon { public: bool Projectile, Loaded; bool IsProjectileWeapon() const; bool IsProjectileWeaponLoaded() const; };
bool CTCWeapon::IsProjectileWeapon() const { return Projectile; }
bool CTCWeapon::IsProjectileWeaponLoaded() const { return Loaded; }

static int g_deeds;
static long g_deed_type, g_post_type, g_extra;
static const C3DVector* g_deed_pos;
static const CThing* g_deed_target;
static CThingAICreature* g_deed_creature;
static bool g_b1, g_b2, g_b3;
static const CFaction* g_faction;
class CTCHeroOpinionDeedLog {
public:
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);
};
long CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                                     EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1)
{
    ++g_deeds; g_deed_type = type; g_deed_pos = &position; g_deed_target = thing; g_deed_creature = creature; g_post_type = postType;
    g_b1 = b1; g_b2 = b2; g_faction = faction; g_b3 = b3; g_extra = l1;
    return 7;
}

static bool g_enemies, g_willing, g_aware, g_aware_flag;
static int g_willing_calls;
class CTCEnemy { public: static bool __fastcall AreThingsEnemies(const CThing&, const CThing&); };
bool __fastcall CTCEnemy::AreThingsEnemies(const CThing&, const CThing&) { return g_enemies; }
namespace NCreatureAI {
bool __fastcall IsWillingToAttackTarget(const CThingAICreature*, const CThing*);
bool __fastcall IsAwareOfThing(const CThingAICreature*, const CThing*, bool);
bool __fastcall IsWillingToAttackTarget(const CThingAICreature*, const CThing*) { ++g_willing_calls; return g_willing; }
bool __fastcall IsAwareOfThing(const CThingAICreature*, const CThing*, bool flag) { g_aware_flag = flag; return g_aware; }
}

class CTCTargetingPlayer {
public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { }
    virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { }
    virtual void v10() { } virtual void v11() { } virtual void v12() { } virtual void v13() { } virtual void v14() { }
    virtual void v15() { } virtual void v16() { } virtual void v17() { } virtual void v18() { } virtual void v19() { }
    virtual void v20() { } virtual void v21() { } virtual void v22() { } virtual void v23() { } virtual void v24() { }
    virtual void v25() { } virtual void v26() { } virtual void v27() { } virtual void v28() { } virtual void v29() { }
    virtual void v30() { } virtual void v31() { } virtual void v32() { }
    virtual CThing* GetTargeted() { return Targeted; }   // slot 33 (+0x84)
    CThing* ParentThing;                                  // +0x04
    unsigned char Pad[0xD4 - 0x08];
    CBaseIntelligentPointer PLastThreatenedThing;         // +0xD4
    CThing* Targeted;
    void UpdateTargetThreatening();
};

static CPlayer g_player;
static CThing g_parent, g_target, g_carried;
static CTCHeroOpinionDeedLog g_log;
static CTCCarrying g_carrying;
static CTCWeapon g_weapon;
static CTCTargetingPlayer g_tp;
static CIntelligentPointer<CThing> g_instantiate;   // emits the deleting destructors the row's vtable COMDAT names
static int g_fail;

static void reset()
{
    memset(&g_player, 0, sizeof(g_player)); memset(&g_parent, 0, sizeof(g_parent));
    memset(&g_target, 0, sizeof(g_target)); memset(&g_carried, 0, sizeof(g_carried));
    g_player.ZTargeting = true;
    g_parent.Player = &g_player; g_parent.Pos.X = 3.0f;
    g_parent.AddTC(70, &g_carrying); g_parent.AddTC(81, &g_log); g_parent.AddTC(12, 0);
    g_target.Type = 1; g_target.AddTC(79, 0); g_target.AddTC(5, 0);
    g_carrying.Slot = &g_carried; g_carried.AddTC(72, &g_weapon);
    g_weapon.Projectile = true; g_weapon.Loaded = true;
    g_enemies = false; g_willing = false; g_aware = true; g_aware_flag = false; g_willing_calls = 0;
    g_deeds = 0; g_deed_type = -1;
    g_tp.ParentThing = &g_parent; g_tp.Targeted = &g_target;
    g_tp.PLastThreatenedThing.Item = (CBaseObject*)&g_carried;   // stale value
}
static void expect(const char* what, bool threatened, long deed)
{
    g_tp.UpdateTargetThreatening();
    bool ok;
    if (threatened)
        ok = g_deeds == 1 && g_deed_type == deed && g_deed_pos == &g_parent.Pos && g_deed_target == &g_target &&
             g_deed_creature == (CThingAICreature*)&g_target && g_post_type == 1 && !g_b1 && !g_b2 && g_faction == 0 && !g_b3 &&
             g_extra == 0 && g_aware_flag && g_tp.PLastThreatenedThing.Item == (CBaseObject*)&g_target;
    else
        ok = g_deeds == 0 && g_tp.PLastThreatenedThing.Item == (CBaseObject*)&g_carried;
    if (!ok) { printf("FAIL %s (deeds %d type %ld)\n", what, g_deeds, g_deed_type); g_fail = 1; }
}

int main()
{
    reset(); expect("loaded projectile weapon", true, 41);
    reset(); g_weapon.Loaded = false; expect("unloaded projectile weapon", true, 40);
    reset(); g_weapon.Projectile = false; expect("melee weapon", true, 40);
    reset(); g_tp.Targeted = 0; g_tp.UpdateTargetThreatening();
    if (g_tp.PLastThreatenedThing.Item != 0 || g_deeds != 0) { printf("FAIL no target\n"); g_fail = 1; }
    reset(); g_target.Type = 2; expect("not an AI creature", false, 0);
    reset(); g_player.ZTargeting = false; expect("no targeting mode", false, 0);
    reset(); g_player.ZTargeting = false; g_player.FirstPerson = true; expect("first-person targeting", true, 41);
    reset(); g_enemies = true; g_willing = true; expect("enemy willing to attack", false, 0);
    reset(); g_enemies = true; expect("enemy not willing", true, 41);
    reset(); g_willing = true; expect("not enemies", true, 41);
    if (g_willing_calls != 0) { printf("FAIL willing tested for a non-enemy\n"); g_fail = 1; }
    reset(); g_target.Bits[79 >> 5] = 0; expect("no opinion of hero", false, 0);
    reset(); g_aware = false; expect("not aware", false, 0);
    reset(); g_parent.Bits[81 >> 5] &= ~(1UL << (81 & 31)); expect("no deed log", false, 0);
    reset(); g_parent.Bits[70 >> 5] &= ~(1UL << (70 & 31)); expect("no carrying", false, 0);
    reset(); g_carrying.Slot = 0; expect("empty primary slot", false, 0);
    reset(); g_carried.Bits[72 >> 5] = 0; expect("carried thing is no weapon", false, 0);
    if (!g_fail) printf("00803280_TEST PASS\n");
    return g_fail;
}