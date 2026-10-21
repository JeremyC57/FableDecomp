// Behaviour test for CTCShotDefinitionNymphFun::OnCollision (0x007bdb66): first hit by a live trap-spawning shooter spawns the trap (at the hit point, or snapped below a live terrain thing), makes the shooter immune, triggers it, marks the shot spent; base collision always runs.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
class CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet;
static int g_strings;
struct CCharString {
    const char* m_data;
    CCharString(const char* text, long length = -1) : m_data(text) { (void)length; ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) { SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
};
struct CTCBitset {
    unsigned long Words[9];
    bool test(unsigned long bit) const { return (Words[bit >> 5] >> (bit & 31)) & 1; }
};
class CThing {
public:
    CTCBitset TCBits;
    CInterfaceMap m_interfaces;
    unsigned char Flags6c;
    signed char Team;
    bool Alive;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && thing->Alive; }
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CWorldMap { float GetHeightZBelowPos(const C3DVector& pos) { return pos.Z - 0.25f; } };
struct CTrapSpawnDef { long TrapObjectType; };
struct CTCTrapSpawner { CTrapSpawnDef* Def; };
static long g_createdType; static C3DVector g_createdAt; static long g_createdTeam; static const char* g_createdName;
struct CThingObject : CThing {
    static CThingObject* s_next;
    static CThingObject* __fastcall Create(long type, const C3DVector& pos, long team, const CRightHandedSet* rh, unsigned long flags,
                                           const CCharString& name)
    { g_createdType = type; g_createdAt = pos; g_createdTeam = team; g_createdName = name.m_data; (void)rh; (void)flags; return s_next; }
};
CThingObject* CThingObject::s_next;
struct CTCTrapBase {
    CThing* immune; int triggers; int summonCalls;
    void SetThingAsImmune(CThing* thing) { immune = thing; }
    void ManualTrigger() { ++triggers; }
};
void __fastcall MakeSummonedCreaturesImmuneToTrap(CTCTrapBase* trap) { ++trap->summonCalls; }
static int g_baseCalls;
struct CTCShotDefinitionBase {
    CThing* ParentThing;
    CBaseIntelligentPointer PShooter;
    CWorldMap Map;
    CWorldMap* GetWorldMap() { return &Map; }
    void OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other) { ++g_baseCalls; (void)type; (void)pos; (void)normal; (void)other; }
};
struct CTCShotDefinitionNymphFun : CTCShotDefinitionBase {
    bool Spent;
    void OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other);
};

void CTCShotDefinitionNymphFun::OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other)
{
    if (!Spent) {
        if (IS_THING_ALIVE(PShooter.GetPItem())) {
            CThing* shooter = PShooter.GetPItem();
            if (shooter->TCBits.test(0xf5)) {
                long trapType = ((CTCTrapSpawner*)shooter->GetTC(0xf5))->Def->TrapObjectType;
                if (trapType > 0) {
                    C3DVector at = pos;
                    if (IS_THING_ALIVE(other) && (other->Flags6c & 8)) {
                        at = other->GetPosition();
                        at.Z += 1.0f;
                        at.Z = GetWorldMap()->GetHeightZBelowPos(at);
                    }
                    CThingObject* trapThing;
                    {
                        CCharString name("");
                        long team = PShooter.GetPItem()->Team;
                        trapThing = CThingObject::Create(trapType, at, team, 0, 0, name);
                    }
                    CTCTrapBase* trap = 0;
                    if (trapThing->TCBits.test(0x7d))
                        trap = (CTCTrapBase*)trapThing->GetTC(0x7d);
                    trap->SetThingAsImmune(PShooter.GetPItem());
                    trap->ManualTrigger();
                    MakeSummonedCreaturesImmuneToTrap(trap);
                    Spent = true;
                }
            }
        }
    }
    CTCShotDefinitionBase::OnCollision(type, pos, normal, other);
}

static void Init(CThing& t) { memset(&t, 0, sizeof(t)); t.Alive = true; }
int main()
{
    int fails = 0;
    static CThing shooter, rock; static CThingObject trapThing;
    Init(shooter); Init(rock); Init(trapThing);
    shooter.Team = -2;
    static CTrapSpawnDef def; def.TrapObjectType = 9;
    static CTCTrapSpawner spawner; spawner.Def = &def;
    CInterfaceMap::SEntry se[1] = { { 0xf5, (CTCBase*)&spawner } };
    shooter.m_interfaces.first = se; shooter.m_interfaces.last = se + 1; shooter.TCBits.Words[0xf5 >> 5] |= 1UL << (0xf5 & 31);
    static CTCTrapBase trap; memset(&trap, 0, sizeof(trap));
    CInterfaceMap::SEntry te[1] = { { 0x7d, (CTCBase*)&trap } };
    trapThing.m_interfaces.first = te; trapThing.m_interfaces.last = te + 1; trapThing.TCBits.Words[0x7d >> 5] |= 1UL << (0x7d & 31);
    CThingObject::s_next = &trapThing;
    rock.Flags6c = 8; rock.Pos.X = 4; rock.Pos.Y = 5; rock.Pos.Z = 6;
    C3DVector hit; hit.X = 1; hit.Y = 2; hit.Z = 3; C3DVector normal; normal.X = 0; normal.Y = 0; normal.Z = 1;
    CTCShotDefinitionNymphFun shot; shot.ParentThing = 0; shot.PShooter.m_pItem = &shooter; shot.Spent = false;
    shot.OnCollision(1, hit, normal, 0);
    if (!shot.Spent || g_createdType != 9 || g_createdAt.X != 1 || g_createdAt.Z != 3 || g_createdTeam != -2 || strcmp(g_createdName, "") != 0) ++fails;
    if (trap.immune != &shooter || trap.triggers != 1 || trap.summonCalls != 1 || g_baseCalls != 1 || g_strings != 0) ++fails;
    shot.OnCollision(1, hit, normal, &rock);                     // spent: base only
    if (trap.triggers != 1 || g_baseCalls != 2) ++fails;
    shot.Spent = false; shot.OnCollision(1, hit, normal, &rock);  // snapped: (4,5, 6+1-0.25)
    if (g_createdAt.X != 4 || g_createdAt.Y != 5 || g_createdAt.Z != 6.75f || trap.triggers != 2) ++fails;
    shot.Spent = false; shooter.Alive = false; shot.OnCollision(1, hit, normal, &rock);   // dead shooter
    if (shot.Spent || trap.triggers != 2 || g_baseCalls != 4) ++fails;
    shooter.Alive = true; def.TrapObjectType = 0; shot.OnCollision(1, hit, normal, &rock);  // no trap type
    if (shot.Spent || trap.triggers != 2) ++fails;
    if (fails) { printf("007bdb66_TEST FAIL %d\n", fails); return 1; }
    printf("007bdb66_TEST PASS\n");
    return 0;
}