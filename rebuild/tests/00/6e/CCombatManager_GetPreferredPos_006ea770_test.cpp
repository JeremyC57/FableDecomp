// Behaviour test for 006ea770: final.cpp copied verbatim, every declared callee stubbed below so arguments are observable.
// CCombatManager::GetPreferredPos(const CThingCreatureBase&) const
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Grade: RELOCATION_MATCH -- the frame-slot gap (sub esp,60h v 5Ch: the first UID find key kept its own slot because VC7.1 treated the
//   extern find's result as possibly derived from the key it was handed, so the spilled &info.AttackerInfo kept that slot alive) closes
//   with a same-TU __declspec(noinline) body for CCombatantMap::find (the STLport _Rb_tree lower-bound walk): the key slot is then
//   released after the call and both keys share local[8] with the wheel spill, as retail.
// CCombatManager::GetPreferredPos(const CThingCreatureBase& creature) const -- where a registered attacker should stand: the
// attacked creature's position extrapolated seven frames by its physics velocity, then (when the attacker owns a combat-wheel
// ring) walked in from the wheel's attack vector in 0.25 steps until a navigable spot on the attacked's navigation layer is
// found; otherwise the prong is blocked and the extrapolated position returned.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA770, 799 bytes, `ret 8` (hidden C3DVector result + creature).
// Callees: CThingCreatureBase::IsRegisteredCombatant 0x00662410, combatant map find 0x006F0250 (twice, unchecked),
//   CThing::GetPosition 0x004C73D0, CCombatWheel::ComputeAttackVector 0x006E89B0, CInterfaceMap::LowerBound (const) 0x004365B0,
//   CThing::GetThingStandingOn (vtable +0x98), CThingCreatureBase::GetRegionAtPosition 0x00661D20,
//   CThingFindNavigationLayerAt::operator() 0x0063EFB0, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620,
//   CCombatWheel::BlockAttackerProng 0x006E9200. Globals: 7.0f 0x01254D80, 1.0f 0x0122DED8, g_Epsilon 0x0129BA3C.
// Shape notes: the normalise goes through an inline GetLength() (keeps 1/len on the x87 stack to the end: `fld Z; fmul st(1);
//   fstp Z; fstp st(0)`); extern const g_Epsilon stops the clamp-then-test fold; user C3DVector copy ctor gives the return copy;
//   pack(4) keeps the __int64 UID at +0x18 in the polymorphic CThing.
// Frame: the combatant map find has a same-TU noinline body (below) so VC7.1 knows its node result is not derived from the key
//   it was handed; with the extern declaration alone the first key's slot stays alive while &info.AttackerInfo (spilled for
//   BlockAttackerProng) lives, and the frame grows to 60h (key spellings, pointer/reference info, block scopes, /Oa, QFE do not help).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CPhysicsObject {
    unsigned char _pad00[0x0c];
    C3DVector Pos;                                    // +0x0c
    C3DVector LastPos;                                // +0x18
};
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620
};
#pragma pack(push, 4)
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37();
    virtual CThing* GetThingStandingOn() const;       // slot 38 (+0x98)
    unsigned char _pad04[0x14];
    unsigned __int64 UniqueID;                        // +0x18
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsObject* PPhysics;                         // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char m_flags6c;                          // +0x6c
    unsigned __int64 GetUID() const { return UniqueID; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template<class T> __forceinline T* GetTCIfPresent(long type) const
    {
        T* component = 0;
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            component = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
        return component;
    }
};
#pragma pack(pop)
struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant() const;               // 0x00662410
    long GetRegionAtPosition(const C3DVector& pos) const;   // 0x00661D20
};
extern const float g_Epsilon;                        // 0x0129BA3C (1e-4f)
struct CThingFindNavigationLayerAt {
    C3DVector Pos;                                    // +0x00
    long      Layer;                                  // +0x0c
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) {}
    bool operator()(CThing* thing);                   // 0x0063EFB0
};
struct CAttackingCombatantInfo {
    CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                              // +0x04
    long CombatWheelRingIndex;                        // +0x08
    unsigned char _rest[0x30 - 0x0c];
};
struct CCombatWheel {
    unsigned char CombatRings[12];
    C3DVector ComputeAttackVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring) const;   // 0x006E89B0
    void BlockAttackerProng(const CThingCreatureBase& attacker, const CAttackingCombatantInfo* attackerInfo) const;   // 0x006E9200
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;             // +0x04
    unsigned char Attackers[0x0c];                    // +0x34
    CCombatWheel CombatWheel;                         // +0x40
    unsigned char _rest[0x70 - 0x4c];
};
struct CCombatantMap {
    struct node {
        long colour; node* parent; node* left; node* right;
        unsigned __int64 key;                         // +0x10
        CCombatantInfo value;                         // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;    // 0x006F0250
};
// same-TU stand-in for the combatant map find (0x006F0250): the STLport _Rb_tree lower-bound walk; VC7.1 then knows the
// returned node pointer is not derived from the key it was handed.
__declspec(noinline) CCombatantMap::node* CCombatantMap::find(const unsigned __int64& key) const
{
    node* y = header;
    node* x = header->parent;
    while (x) {
        if (!(x->key < key)) { y = x; x = x->left; }
        else x = x->right;
    }
    if (y == header || key < y->key)
        y = header;
    return y;
}
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                    // +0x04
    CCombatantMap Combatants;                         // +0x10
    const CCombatantInfo& GetCombatantInfo(unsigned __int64 uid) const { return Combatants.find(uid)->value; }
    C3DVector GetPreferredPos(const CThingCreatureBase& creature) const;
};

C3DVector CCombatManager::GetPreferredPos(const CThingCreatureBase& creature) const
{
    if (creature.IsRegisteredCombatant()) {
        const CCombatantInfo& info = GetCombatantInfo(creature.GetUID());
        const CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
        const CCombatantInfo& attackedInfo = GetCombatantInfo(attacked->GetUID());
        CPhysicsObject* physics = attacked->PPhysics;
        C3DVector predicted = (physics->Pos - physics->LastPos) * 7.0f + attacked->GetPosition();
        if (info.AttackerInfo.CombatWheelRingIndex >= 0) {
            const CCombatWheel& wheel = attackedInfo.CombatWheel;
            C3DVector direction = wheel.ComputeAttackVector(creature, *attacked, info.AttackerInfo.CombatWheelRingIndex);
            float distance = direction.GetLength();
            direction *= 1.0f / distance;
            CTCCreatureNavigation* navigation = creature.GetTCIfPresent<CTCCreatureNavigation>(7);
            CThing* standingOn = 0;
            if (attacked->GetThingStandingOn() && (attacked->GetThingStandingOn()->m_flags6c & 2))
                standingOn = attacked->GetThingStandingOn();
            long region = attacked->GetRegionAtPosition(attacked->GetPosition());
            C3DVector candidate;
            do {
                candidate = predicted + direction * distance;
                bool sameLayer = true;
                if (standingOn) {
                    CThingFindNavigationLayerAt finder(candidate);
                    finder(standingOn);
                    sameLayer = (float)finder.Layer == (float)region;
                }
                if (sameLayer && navigation->IsAbleToNavigateFromPosToPos(&creature.GetPosition(), &candidate, 0.5f))
                    return candidate;
                distance -= 0.25f;
                if (distance < 0.0f)
                    distance = 0.0f;
            } while (distance > g_Epsilon);
            wheel.BlockAttackerProng(creature, &info.AttackerInfo);
            return predicted;
        }
        return creature.GetPosition();
    }
    return creature.GetPosition();
}

// ---- stubs, globals and cases ----
const float g_Epsilon = 1e-4f;
static char g_log[2048];
static C3DVector g_creaturePos, g_attackedPos, g_attackVector;
static CThing* g_standingOn = 0;
static long g_region = 0, g_layer = 0;
static int g_registered = 1;
static int g_navOkAt = -1;           // the navigation call index (0-based) that succeeds, -1 = never
static int g_navCalls = 0;
static C3DVector g_navTo[16];
static const C3DVector* g_navFrom = 0;
static float g_navTol = 0.0f;
static C3DVector g_finderPos[16]; static int g_finderCalls = 0;
static CThing* g_finderThing = 0;
static const CAttackingCombatantInfo* g_blockedInfo = 0; static const CThingCreatureBase* g_blockedAttacker = 0; static int g_blocks = 0;
static const CThingCreatureBase *g_cavAttacker = 0, *g_cavDefender = 0; static long g_cavRing = -1; static const CCombatWheel* g_cavWheel = 0;
static CInterfaceMap::SEntry g_entries[1]; static long g_lbKey = 0; static int g_lbCalls = 0;
static CThingCreatureBase g_creature, g_attacked, g_platform;
static CTCCreatureNavigation g_navigation;
static CPhysicsObject g_physics;
static C3DVector g_regionPos;

void CThing::v00() {}
void CThing::v01() {}
void CThing::v02() {}
void CThing::v03() {}
void CThing::v04() {}
void CThing::v05() {}
void CThing::v06() {}
void CThing::v07() {}
void CThing::v08() {}
void CThing::v09() {}
void CThing::v10() {}
void CThing::v11() {}
void CThing::v12() {}
void CThing::v13() {}
void CThing::v14() {}
void CThing::v15() {}
void CThing::v16() {}
void CThing::v17() {}
void CThing::v18() {}
void CThing::v19() {}
void CThing::v20() {}
void CThing::v21() {}
void CThing::v22() {}
void CThing::v23() {}
void CThing::v24() {}
void CThing::v25() {}
void CThing::v26() {}
void CThing::v27() {}
void CThing::v28() {}
void CThing::v29() {}
void CThing::v30() {}
void CThing::v31() {}
void CThing::v32() {}
void CThing::v33() {}
void CThing::v34() {}
void CThing::v35() {}
void CThing::v36() {}
void CThing::v37() {}
CThing* CThing::GetThingStandingOn() const { strcat(g_log, "StandingOn;"); return g_standingOn; }
bool CThingCreatureBase::IsRegisteredCombatant() const { strcat(g_log, this == &g_creature ? "IsRegistered(creature);" : "IsRegistered(?);"); return g_registered != 0; }
const C3DVector& CThing::GetPosition() const { return this == &g_creature ? g_creaturePos : g_attackedPos; }
long CThingCreatureBase::GetRegionAtPosition(const C3DVector& pos) const { g_regionPos = pos; strcat(g_log, this == &g_attacked ? "Region(attacked);" : "Region(?);"); return g_region; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { ++g_lbCalls; g_lbKey = *type; return g_entries; }
C3DVector CCombatWheel::ComputeAttackVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring) const
{
    g_cavWheel = this; g_cavAttacker = &attacker; g_cavDefender = &defender; g_cavRing = ring; strcat(g_log, "AttackVector;");
    return g_attackVector;
}
void CCombatWheel::BlockAttackerProng(const CThingCreatureBase& attacker, const CAttackingCombatantInfo* attackerInfo) const
{
    ++g_blocks; g_blockedAttacker = &attacker; g_blockedInfo = attackerInfo; g_cavWheel = this; strcat(g_log, "Block;");
}
bool CThingFindNavigationLayerAt::operator()(CThing* thing)
{
    if (g_finderCalls < 16) g_finderPos[g_finderCalls] = Pos;
    ++g_finderCalls; g_finderThing = thing; Layer = g_layer; strcat(g_log, "Finder;");
    return true;
}
bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance)
{
    if (g_navCalls < 16) g_navTo[g_navCalls] = *to;
    g_navFrom = from; g_navTol = tolerance; strcat(g_log, "Nav;");
    return g_navCalls++ == g_navOkAt;
}
static bool near_(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 2e-4f; }
static bool same(const C3DVector& a, const C3DVector& b) { return near_(a.X, b.X) && near_(a.Y, b.Y) && near_(a.Z, b.Z); }
static void Reset() { g_log[0] = 0; g_navCalls = 0; g_finderCalls = 0; g_blocks = 0; g_lbCalls = 0; g_cavRing = -1; g_blockedInfo = 0; }

int main()
{
    // the combatant map: header -> root A (the creature, uid 100) with right child B (the attacked, uid 200)
    CCombatantMap::node header, a, b;
    memset(&header, 0, sizeof header); memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    header.parent = &a; a.parent = &header; a.right = &b; b.parent = &a;
    a.key = 100; b.key = 200;
    CCombatManager* mgr = (CCombatManager*)calloc(1, sizeof(CCombatManager));
    mgr->Combatants.header = &header; mgr->Combatants.count = 2;
    memset(&g_creature, 0, sizeof g_creature); memset(&g_attacked, 0, sizeof g_attacked); memset(&g_platform, 0, sizeof g_platform);
    new (&g_creature) CThingCreatureBase; new (&g_attacked) CThingCreatureBase; new (&g_platform) CThingCreatureBase;
    g_creature.UniqueID = 100; g_attacked.UniqueID = 200;
    a.value.AttackerInfo.Attacked = &g_attacked; a.value.AttackerInfo.CombatWheelRingIndex = 2;
    g_attacked.PPhysics = &g_physics;
    g_physics.Pos = C3DVector(10.0f, 20.0f, 30.0f); g_physics.LastPos = C3DVector(9.0f, 20.5f, 30.0f);   // velocity (1, -0.5, 0)
    g_attackedPos = C3DVector(100.0f, 200.0f, 5.0f); g_creaturePos = C3DVector(1.0f, 2.0f, 3.0f);
    g_creature.m_presentBits[0] = 1UL << 7; g_entries[0].type = 7; g_entries[0].component = (CTCBase*)&g_navigation;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 1;
    g_attackVector = C3DVector(0.6f, 0.0f, 0.8f);          // length 1
    C3DVector predicted(100.0f + 7.0f, 200.0f - 3.5f, 5.0f);
    C3DVector result;

    // 1. not registered: the creature's own position
    g_registered = 0; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(same(result, g_creaturePos) && strcmp(g_log, "IsRegistered(creature);") == 0);

    // 2. registered, no combat-wheel ring: the creature's own position, nothing else called
    g_registered = 1; a.value.AttackerInfo.CombatWheelRingIndex = -1; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(same(result, g_creaturePos) && strcmp(g_log, "IsRegistered(creature);") == 0 && g_lbCalls == 0);

    // 3. ring 2, nothing stood on, navigable at the full attack distance: predicted + unit direction * length
    a.value.AttackerInfo.CombatWheelRingIndex = 2; g_navOkAt = 0; g_standingOn = 0; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(same(result, C3DVector(107.6f, 196.5f, 5.8f)));
    CHECK(strcmp(g_log, "IsRegistered(creature);AttackVector;StandingOn;Region(attacked);Nav;") == 0);
    CHECK(g_cavWheel == &b.value.CombatWheel && g_cavAttacker == &g_creature && g_cavDefender == &g_attacked && g_cavRing == 2);
    CHECK(g_lbCalls == 1 && g_lbKey == 7 && g_navFrom == &g_creaturePos && near_(g_navTol, 0.5f) && same(g_regionPos, g_attackedPos));
    CHECK(g_blocks == 0 && g_finderCalls == 0);

    // 4. attack vector of length 2: candidates walk in by 0.25 from 2.0; the third try (1.5) is accepted
    g_attackVector = C3DVector(0.0f, 2.0f, 0.0f); g_navOkAt = 2; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(g_navCalls == 3 && same(g_navTo[0], C3DVector(107.0f, 198.5f, 5.0f)) && same(g_navTo[1], C3DVector(107.0f, 198.25f, 5.0f)));
    CHECK(same(result, C3DVector(107.0f, 198.0f, 5.0f)) && g_blocks == 0);

    // 5. never navigable: 8 tries (2.0 .. 0.25), the prong is blocked with the creature's attacker info, predicted returned
    g_navOkAt = -1; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(g_navCalls == 8 && same(g_navTo[7], C3DVector(107.0f, 196.75f, 5.0f)));
    CHECK(g_blocks == 1 && g_blockedAttacker == &g_creature && g_blockedInfo == &a.value.AttackerInfo && g_cavWheel == &b.value.CombatWheel);
    CHECK(same(result, predicted));

    // 6. stood on a thing without flag bit 1: ignored (no finder)
    g_standingOn = &g_platform; g_platform.m_flags6c = 0; g_navOkAt = 0; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(g_finderCalls == 0 && g_navCalls == 1 && same(result, C3DVector(107.0f, 198.5f, 5.0f)));
    CHECK(strcmp(g_log, "IsRegistered(creature);AttackVector;StandingOn;StandingOn;Region(attacked);Nav;") == 0);

    // 7. stood on a flagged thing: the navigation layer at each candidate must equal the attacked's region
    g_platform.m_flags6c = 2; g_region = 3; g_layer = 3; g_navOkAt = 1; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(g_finderCalls == 2 && g_finderThing == &g_platform && same(g_finderPos[1], C3DVector(107.0f, 198.25f, 5.0f)));
    CHECK(g_navCalls == 2 && same(result, C3DVector(107.0f, 198.25f, 5.0f)));
    CHECK(strcmp(g_log, "IsRegistered(creature);AttackVector;StandingOn;StandingOn;StandingOn;Region(attacked);Finder;Nav;Finder;Nav;") == 0);

    // 8. layer mismatch everywhere: no navigation test at all, 8 finder calls, blocked, predicted
    g_layer = 2; g_navOkAt = 0; Reset();
    result = mgr->GetPreferredPos(g_creature);
    CHECK(g_finderCalls == 8 && g_navCalls == 0 && g_blocks == 1 && same(result, predicted));

    // 9. the attacked's own combatant record supplies the wheel: swap roles (the attacked is uid 200 -> node B)
    b.value.AttackerInfo.Attacked = &g_creature; b.value.AttackerInfo.CombatWheelRingIndex = 0;
    g_creature.PPhysics = &g_physics; g_layer = 3; g_standingOn = 0; g_navOkAt = 0; Reset();
    result = mgr->GetPreferredPos(g_attacked);
    CHECK(g_cavWheel == &a.value.CombatWheel && g_cavAttacker == &g_attacked && g_cavDefender == &g_creature && g_cavRing == 0);
    CHECK(same(result, C3DVector(8.0f, 0.5f, 3.0f)) && g_navFrom == &g_attackedPos);

    if (fails == 0) printf("006ea770_TEST PASS\n"); else printf("006ea770_TEST FAIL\n");
    return fails;
}