// Behaviour test for 0069fce0: src/0069fce0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCreatureAction_UseObject::FrameUpdate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_UseObject::FrameUpdate() -- finish at once when the used thing is gone; while using the dummy point: keep the base FrameUpdate running until the creature has stopped moving/turning to it, then (with no proximity left and a usable CTCActionUse on the thing) snap the creature's physics to the dummy facing and position; finally the per-frame slot-4 and slot-3 virtuals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FCE0, length 369 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PUsedThing +0xa8, PCreature +8), this object's virtual slots 4 (+0x10) and
// 3 (+0x0c), CCreatureActionBase::FrameUpdate @ 0x00694020 (direct base call), CVectorMap::LowerBound (const instantiation) @ 0x004365B0
// (inlined const GetTC 0xD = CTCActionUse, bit 13 of +0x20), CTCActionUse::CanBeUsedFromDummy @ 0x007EABE0 (manifest
// "CTCShotDefinitionBase::CanBeHitByMeleeStrike" is propagated), _CIfmod @ 0x00BFEB28, the physics component's virtual slots 66
// (+0x108: facing fraction) and 30 (+0x78: position).
// Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1/(2pi) 0x01237F08, the double 1.0 0x01237F00.
// Layout: engine/CCreatureAction_UseObject.h minus the PDB's 12 extra base bytes: PUsedThing +0xa8, Dummy +0xb4 (the facing axis at
// +0xc0), DummyPos +0xcc, Proximity +0xd8, UsingDummy +0xdc, MovingToDummy +0xdd, TurningToDummy +0xde.
// Shape: IS_THING_ALIVE on PUsedThing; UsingDummy is read as the member in both `if`s (one CSE'd load in al, two tests: VC7.1 does not thread the second);
// `Proximity == 0.0f` is `fucompp ; test ah,44h ; jp`; the facing fraction is the DuckAndCover 0x008B6EE0 atan2/fmod wrap.
// The CTCActionUse test reads the used thing ONCE (one GetPItem for both the presence bit and the lookup): a forceinline
// CanUseFromDummy(const CThing*) helper. The final SetPosition goes through a named creature local, so the physics
// pointer loads straight into ecx. Both fix the this/&PUsedThing esi/edi assignment. The physics vtable has
// SetFacingAngle at slot 66 (+0x108); the previous model declared one dummy slot too many.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    virtual void SetPosition(const C3DVector& pos);   // slot 30 (+0x78)
    VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64)
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CTCActionUse {                           // interface type 0xD
    bool CanBeUsedFromDummy() const;            // 0x007EABE0
};
__forceinline bool CanUseFromDummy(const CThing* thing)
{
    return thing->HasTC(0xd) && thing->GetTC<CTCActionUse>(0xd)->CanBeUsedFromDummy();
}
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CRightHandedSet {                        // 24 bytes: two axes
    C3DVector XAxis;                            // +0x00
    C3DVector YAxis;                            // +0x0c
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void UpdateMovement();              // slot 3 (+0x0c)
    virtual void FinishAction();                // slot 4 (+0x10)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    void FrameUpdate();                         // 0x00694020
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    CBaseIntelligentPointer PUsedThing;         // +0xa8
    CTCPhysicsBase*         PreviousPhysicsComponent;   // +0xb0
    CRightHandedSet         Dummy;              // +0xb4
    C3DVector               DummyPos;           // +0xcc
    float                   Proximity;          // +0xd8
    bool                    UsingDummy;         // +0xdc
    bool                    MovingToDummy;      // +0xdd
    bool                    TurningToDummy;     // +0xde
    bool                    AllowTurn;          // +0xdf
    virtual void FrameUpdate();
};

void CCreatureAction_UseObject::FrameUpdate()
{
    if (!IS_THING_ALIVE(PUsedThing)) {
        FinishAction();
        return;
    }
    if (UsingDummy) {
        if (MovingToDummy || TurningToDummy) {
            CCreatureActionBase::FrameUpdate();
            return;
        }
    }
    if (UsingDummy && Proximity == 0.0f && CanUseFromDummy(PUsedThing.GetPItem())) {
        CTCPhysicsBase* physics = PCreature.GetPItem()->m_pPhysics;
        float fraction = (float)atan2(Dummy.YAxis.X, Dummy.YAxis.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod(fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        physics->SetFacingAngle(fraction);
        CThing* creature = PCreature.GetPItem();
        creature->m_pPhysics->SetPosition(DummyPos);
    }
    FinishAction();
    UpdateMovement();
}

// ---- stubs and driver ----
#include <new>
#define VD(n) void CTCPhysicsBase::v##n() { }
#define VD10(p) VD(p##0) VD(p##1) VD(p##2) VD(p##3) VD(p##4) VD(p##5) VD(p##6) VD(p##7) VD(p##8) VD(p##9)
VD10(0) VD10(1) VD(20) VD(21) VD(22) VD(23) VD(24) VD(25) VD(26) VD(27) VD(28) VD(29)
VD10(3) VD10(4) VD10(5) VD(60) VD(61) VD(62) VD(63) VD(64)
static int g_facings = 0, g_positions = 0, g_finish = 0, g_update = 0, g_baseUpdate = 0, g_canUse = 0, g_lookups = 0;
static float g_lastFacing = -1.0f; static const C3DVector* g_lastPos = 0;
void CTCPhysicsBase::SetPosition(const C3DVector& pos) { ++g_positions; g_lastPos = &pos; }
void CTCPhysicsBase::SetFacingAngle(float fraction) { ++g_facings; g_lastFacing = fraction; }
static bool g_canUseResult = true;
bool CTCActionUse::CanBeUsedFromDummy() const { ++g_canUse; return g_canUseResult; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    ++g_lookups;
    const SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCreatureActionBase::v00() { } void CCreatureActionBase::v01() { } void CCreatureActionBase::v02() { }
void CCreatureActionBase::UpdateMovement() { ++g_update; }
void CCreatureActionBase::FinishAction() { ++g_finish; }
void CCreatureActionBase::FrameUpdate() { ++g_baseUpdate; }
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    static unsigned char usedRaw[sizeof(CThing)], creatureRaw[sizeof(CThing)];
    CThing* used = (CThing*)usedRaw; CThing* creature = (CThing*)creatureRaw;
    static unsigned char physRaw[sizeof(CTCPhysicsBase)];
    CTCPhysicsBase* phys = new (physRaw) CTCPhysicsBase;
    creature->m_pPhysics = phys;
    static CTCActionUse use;
    static CInterfaceMap::SEntry entries[2] = { { 3, 0 }, { 0xd, (CTCBase*)&use } };
    used->m_interfaces.first = entries; used->m_interfaces.last = entries + 2;
    used->m_presentBits[0] = 1u << 13;
    used->m_flags91 = 0;
    static unsigned char actRaw[sizeof(CCreatureAction_UseObject)];
    CCreatureAction_UseObject* a = new (actRaw) CCreatureAction_UseObject;
    a->PCreature.m_pItem = creature;
    a->PUsedThing.m_pItem = 0;
    a->FrameUpdate();                                    // no used thing: finish only
    CHECK(g_finish == 1 && g_update == 0 && g_baseUpdate == 0);
    a->PUsedThing.m_pItem = used; used->m_flags91 = 1;
    a->FrameUpdate();                                    // dead used thing: finish only
    CHECK(g_finish == 2 && g_update == 0);
    used->m_flags91 = 0;
    a->UsingDummy = true; a->MovingToDummy = true; a->TurningToDummy = false;
    a->FrameUpdate();                                    // still moving to the dummy: base update only
    CHECK(g_baseUpdate == 1 && g_finish == 2 && g_update == 0);
    a->MovingToDummy = false; a->Proximity = 0.0f;
    a->Dummy.YAxis.X = -1.0f; a->Dummy.YAxis.Y = 0.0f;  // atan2(-1, 0) = -pi/2 -> -0.25 -> wrapped to 0.75
    a->FrameUpdate();
    CHECK(g_lookups == 1 && g_canUse == 1 && g_facings == 1 && Near(g_lastFacing, 0.75f) && g_positions == 1 && g_lastPos == &a->DummyPos);
    CHECK(g_finish == 3 && g_update == 1);
    a->Dummy.YAxis.X = 1.0f; a->Dummy.YAxis.Y = 0.0f;   // pi/2 -> 0.25, no wrap
    a->FrameUpdate();
    CHECK(g_facings == 2 && Near(g_lastFacing, 0.25f) && g_finish == 4 && g_update == 2);
    a->Proximity = 1.0f;                                 // a proximity: no facing/position, just finish + update
    a->FrameUpdate();
    CHECK(g_facings == 2 && g_lookups == 2 - 0 && g_finish == 5 && g_update == 3);
    a->Proximity = 0.0f; used->m_presentBits[0] = 0;     // no CTCActionUse
    a->FrameUpdate();
    CHECK(g_facings == 2 && g_canUse == 2 && g_finish == 6 && g_update == 4);
    used->m_presentBits[0] = 1u << 13; g_canUseResult = false;
    a->FrameUpdate();
    CHECK(g_facings == 2 && g_canUse == 3 && g_finish == 7 && g_update == 5);
    a->UsingDummy = false; g_canUseResult = true;
    a->FrameUpdate();
    CHECK(g_facings == 2 && g_finish == 8 && g_update == 6 && g_baseUpdate == 1);
    if (fails == 0) printf("0069fce0_TEST PASS\n");
    return fails;
}
