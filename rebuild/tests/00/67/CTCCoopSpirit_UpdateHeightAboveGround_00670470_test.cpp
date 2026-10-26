// Behaviour test for 0x00670470 (source body copied from src/00670470.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
#include <math.h>
#pragma intrinsic(exp)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CTCPhysicsBase {
public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                                 // +0x0c
    void ApplyAcceleration(const C3DVector& acceleration);  // 0x006B0210
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                        // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
class CThingCreatureBase : public CThing {
public:
    float GetHeightZBelowAt(const C3DVector& pos, bool a, bool b) const;   // 0x00666D40
};
class CTCCoopSpirit {
public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                        // +0x04 (CTCBase)
    float GetHeightOffset() const { return 0.2f; }
public:
    void UpdateHeightAboveGround();                         // Ego_r.pdb ?UpdateHeightAboveGround@CTCCoopSpirit@@AAEXXZ (private)
};

void CTCCoopSpirit::UpdateHeightAboveGround()
{
    CThingCreatureBase* creature = ParentThing;
    CTCPhysicsBase* physics = (CTCPhysicsBase*)creature->GetTCIfPresent(2);
    const C3DVector& pos = physics->Position;
    float diff = creature->GetHeightZBelowAt(pos, false, true) + GetHeightOffset() - pos.Z;
    long sign = diff < 0.0f ? -1 : 1;
    float direction = (float)sign;
    float e = (float)(exp(diff * diff * 2.25f) - 1.0);
    physics->ApplyAcceleration(C3DVector(0.0f, 0.0f, direction * (e > 1.0f ? 1.0f : e)));
}

// ---- test harness ----
// Stubs: a one-entry interface map holding the physics component (type 2), a fixed ground height, ApplyAcceleration records.
static float g_ground = 0.0f;
static C3DVector g_acc(0.0f, 0.0f, 0.0f);
static int g_applied = 0;
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= first->type ? first : last; }
float CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool a, bool b) const { return (!a && b) ? g_ground : -999.0f; }
void CTCPhysicsBase::ApplyAcceleration(const C3DVector& acceleration) { g_acc = acceleration; ++g_applied; }
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
struct STestSpirit { void* vptr; CThingCreatureBase* parent; };
int main()
{
    int ok = 1;
    static CThingCreatureBase creature;
    static unsigned char physRaw[sizeof(CTCPhysicsBase)];
    CTCPhysicsBase& physics = *(CTCPhysicsBase*)physRaw;
    memset(&creature, 0, sizeof creature);
    creature.m_presentBits[0] = 1u << 2;
    g_entries[0].type = 2; g_entries[0].component = (CTCBase*)&physics;
    creature.m_interfaces.first = g_entries; creature.m_interfaces.last = g_entries + 1;
    STestSpirit raw; raw.vptr = 0; raw.parent = &creature;
    CTCCoopSpirit* spirit = (CTCCoopSpirit*)&raw;
    void (CTCCoopSpirit::*update)() = &CTCCoopSpirit::UpdateHeightAboveGround;
    // 1: below the hover height by 0.5 -> diff 0.5, e = exp(0.5625) - 1
    physics.Position = C3DVector(1.0f, 2.0f, 0.0f); g_ground = 0.3f;
    (spirit->*update)();
    if (g_applied != 1 || !Near(g_acc.X, 0.0f) || !Near(g_acc.Y, 0.0f) || !Near(g_acc.Z, (float)(exp(0.5625) - 1.0))) ok = 0;
    // 2: far above -> diff -3, clamped to -1
    physics.Position.Z = 3.5f;
    (spirit->*update)();
    if (g_applied != 2 || !Near(g_acc.Z, -1.0f)) ok = 0;
    // 3: exactly at the hover height -> 0
    physics.Position.Z = 0.5f;
    (spirit->*update)();
    if (!Near(g_acc.Z, 0.0f)) ok = 0;
    printf("00670470_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}