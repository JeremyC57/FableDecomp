// 006d5b50 behaviour test: CThingAICreature::UpdateAITurning
#ifndef _MSC_VER
#define __forceinline inline
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) { Z = o.Z; Y = o.Y; X = o.X; }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
__forceinline float GetLengthSquared(C3DVector v) { return v.Z * v.Z + v.Y * v.Y + v.X * v.X; }
class CBaseObject;
struct CBaseIntelligentPointer {
    unsigned char _pad00[0x08];
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
struct CTCGraphicAppearance {
    C3DVector GetHeadPosition() const;                      // 0x004C10E0
    C3DVector GetHelperPointOrCentre(float fraction) const; // 0x004C1060
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    unsigned char _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                                 // +0x91
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CTCLook { public:
    void LookAtPos(const C3DVector& position, long speed);  // 0x00753E90
};
struct CAITurnState {
    unsigned long Flags;                                    // +0x00
    bool IsFacingPosition() const { return (unsigned char)(Flags >> 1) & 1; }
    bool IsFacingThing() const { return (unsigned char)(Flags >> 2) & 1; }
    bool IsFacingMovement() const { return (unsigned char)(Flags >> 4) & 1; }
    CThing* GetThing() const { return (CThing*)Thing.GetPItem(); }
    bool IsThingValid() const { CThing* thing = GetThing(); return thing && !thing->Dead; }
    long Speed;                                             // +0x04
    unsigned char _pad08[0x34 - 0x08];
    C3DVector Position;                                     // +0x34
    CBaseIntelligentPointer Thing;                          // +0x40
};
class CThingAICreature : public CThing { public:
    unsigned char _pad94[0x1a0 - 0x94];
    CAITurnState* PTurnState;                               // +0x1a0
    bool IsInMelee() const;                         // 0x00661E80
    void UpdateAITurning(const C3DVector& movement);
};

void CThingAICreature::UpdateAITurning(const C3DVector& movement)
{
    C3DVector target;
    if (PTurnState->IsFacingPosition()) {
        target = PTurnState->Position;
    } else if (PTurnState->IsFacingThing() && PTurnState->IsThingValid()) {
        if (PTurnState->GetThing()->PGraphicAppearance) {
            if (IsInMelee()) {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHeadPosition();
            } else {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHelperPointOrCentre(1.0f);
            }
        } else {
            target = PTurnState->GetThing()->GetPosition();
        }
    } else if (PTurnState->IsFacingMovement() && GetLengthSquared(movement) > 0.0001f) {
        target = GetPosition() + movement;
    } else {
        return;
    }
    CTCLook* look;
    if (HasTC(0x43))
        look = (CTCLook*)GetTC(0x43);
    look->LookAtPos(target, PTurnState->Speed);
}

// ---- mocks ----
class CBaseObject { public: int dummy; };
CBaseObject* CBaseIntelligentPointer::GetPItem() const { CBaseObject* p; memcpy(&p, _pad00, sizeof p); return p; }
static void SetPItem(CBaseIntelligentPointer& ip, void* p) { memset(ip._pad00, 0, sizeof ip._pad00); memcpy(ip._pad00, &p, sizeof p); }
static int g_headCalls, g_helperCalls; static float g_helperFraction;
C3DVector CTCGraphicAppearance::GetHeadPosition() const { ++g_headCalls; return C3DVector(1.0f, 2.0f, 3.0f); }
C3DVector CTCGraphicAppearance::GetHelperPointOrCentre(float fraction) const { ++g_helperCalls; g_helperFraction = fraction; return C3DVector(4.0f, 5.0f, 6.0f); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type) { SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
static C3DVector g_thingPos(7.0f, 8.0f, 9.0f), g_selfPos(10.0f, 20.0f, 30.0f);
static CThing* g_self;
const C3DVector& CThing::GetPosition() const { return this == g_self ? g_selfPos : g_thingPos; }
static int g_lookCalls; static C3DVector g_lookPos; static long g_lookSpeed; static CTCLook* g_lookThis;
void CTCLook::LookAtPos(const C3DVector& position, long speed) { ++g_lookCalls; g_lookPos = position; g_lookSpeed = speed; g_lookThis = this; }
static bool g_headHelper;
bool CThingAICreature::IsInMelee() const { return g_headHelper; }

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)
static bool Eq(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }

static CThingAICreature g_creature;
static CAITurnState g_state;
static CThing g_target;
static CTCGraphicAppearance g_app;
static CTCLook g_look;
static CInterfaceMap::SEntry g_entries[3];

static void Reset(unsigned long flags)
{
    g_headCalls = g_helperCalls = g_lookCalls = 0; g_helperFraction = 0; g_lookSpeed = 0; g_lookThis = 0;
    g_state.Flags = flags; g_state.Speed = 77; g_state.Position = C3DVector(-1.0f, -2.0f, -3.0f);
    SetPItem(g_state.Thing, &g_target);
    g_target.Dead = 0; g_target.PGraphicAppearance = &g_app;
}

int main()
{
    g_self = &g_creature;
    memset(g_creature.m_presentBits, 0, sizeof g_creature.m_presentBits);
    g_creature.m_presentBits[0x43 >> 5] = 1UL << (0x43 & 31);
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x43; g_entries[1].component = (CTCBase*)&g_look;
    g_entries[2].type = 0x50; g_entries[2].component = 0;
    g_creature.m_tcs.first = g_entries; g_creature.m_tcs.last = g_entries + 3; g_creature.m_tcs.end = g_entries + 3;
    g_creature.PTurnState = &g_state;
    C3DVector longMove(0.5f, 0.0f, 0.0f), shortMove(0.001f, 0.001f, 0.001f);

    // 1. facing a fixed position
    Reset(2); g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(Eq(g_lookPos, -1.0f, -2.0f, -3.0f)); CHECK(g_lookSpeed == 77); CHECK(g_lookThis == &g_look);
    // 2. facing a thing with an appearance, head helper
    Reset(4); g_headHelper = true; g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(g_headCalls == 1); CHECK(Eq(g_lookPos, 1.0f, 2.0f, 3.0f));
    // 3. ... helper point at fraction 1
    Reset(4); g_headHelper = false; g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(g_helperCalls == 1); CHECK(g_helperFraction == 1.0f); CHECK(Eq(g_lookPos, 4.0f, 5.0f, 6.0f));
    // 4. a thing without an appearance: its position
    Reset(4); g_target.PGraphicAppearance = 0; g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(Eq(g_lookPos, 7.0f, 8.0f, 9.0f));
    // 5. a dead thing with movement facing: own position plus movement
    Reset(4 | 16); g_target.Dead = 1; g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(Eq(g_lookPos, 10.5f, 20.0f, 30.0f)); CHECK(g_headCalls == 0 && g_helperCalls == 0);
    // 6. a null thing and no movement facing: nothing
    Reset(4); SetPItem(g_state.Thing, 0); g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 0);
    // 7. movement facing with too short a movement: nothing
    Reset(16); g_creature.UpdateAITurning(shortMove);
    CHECK(g_lookCalls == 0);
    // 8. movement facing with a long movement
    Reset(16); g_creature.UpdateAITurning(C3DVector(0.0f, 0.0f, -0.02f));
    CHECK(g_lookCalls == 1); CHECK(Eq(g_lookPos, 10.0f, 20.0f, 29.98f));
    // 9. no facing flags at all
    Reset(0); g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 0);
    // 10. position facing wins over thing facing
    Reset(2 | 4 | 16); g_creature.UpdateAITurning(longMove);
    CHECK(g_lookCalls == 1); CHECK(Eq(g_lookPos, -1.0f, -2.0f, -3.0f)); CHECK(g_headCalls == 0);
    if (g_fail) { printf("006d5b50_TEST FAIL %d\n", g_fail); return 1; }
    printf("006d5b50_TEST PASS\n");
    return 0;
}