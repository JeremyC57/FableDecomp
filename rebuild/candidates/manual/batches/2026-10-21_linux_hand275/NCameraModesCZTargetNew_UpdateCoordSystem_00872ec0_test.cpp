// Behaviour test for retail 0x00872ec0: candidate source embedded verbatim, then stubs and a main.
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// NCameraModes::CZTargetNew::UpdateCoordSystem() -- when the observed thing targets a live thing, point the X axis from the target to the observed thing (flattened, normalised) with the Y axis perpendicular, restart the transition on a target change, and remember the targeted thing
// Leaf: UpdateCoordSystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872EC0, 365 bytes (+ int3 padding), plain `ret`; virtual slot 24 of
//   NCameraModes::CZTargetNew (called through +0x60 by the landed OnInit 0x00872A20).
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0 (twice), the const CVectorMap::LowerBound 0x004365B0
//   (interface lookup, key 8 = the targeting TC), the TC's virtual slot 36 (+0x90) GetTargetedThing, CThing::GetPosition
//   0x004C73D0 (const C3DVector&, twice), C3DVector::Normalise 0x00A14440, CZTargetNew::InitialiseTransitionVariables
//   0x00872490, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10;
//   CIntelligentPointer<const CThing> vtable 0x01237F1C; 1e-4f @ 0x0129BA3C (squared at run time).
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8): CZTargetNew PTargetedThing +0xA4 (PDB +0xC8),
//   XAxis +0xD4, YAxis +0xE0 (as the landed OnInit). CThing: TC bit mask +0x20, interface map +0x44, dead bit +0x91.
// Shape: HasTC is a forced-inline `unsigned long` mask (`test ch,1`); the TC is the one-ternary entry pick over a
//   template constant key; the flattened direction is built as C3DVector(dx, dy, 0) (dy evaluated first), its XY
//   length squared compared against epsilon squared; the axes are stored through the implicit operator= and the inline
//   (Y, -X, 0) perpendicular; `PTargetedThing = target` builds a CIntelligentPointer temporary (base ctor, vtable store,
//   SetPItem) and assigns it through `SetPItem(other.GetPItem())`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                                   // 0x00A14440
    float GetLengthSquaredXY() const { return X * X + Y * Y; }
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct CThing;
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual const CThing* GetTargetedThing() const;     // slot 36 (+0x90)
};

struct SInterfaceEntry { long type; CTCBase* component; };
struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>, 12 bytes
    SInterfaceEntry* _M_start; SInterfaceEntry* _M_finish; SInterfaceEntry* _M_end_of_storage;
    const SInterfaceEntry* LowerBound(const long& key) const;   // 0x004365B0
    const SInterfaceEntry* end() const { return _M_finish; }
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long TCFlags;                              // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return TCFlags & (1 << type); }
    template<long TYPE> __forceinline CTCBase* GetTC() const
    {
        long type = TYPE;
        const CInterfaceMap* map = &Interfaces;
        const SInterfaceEntry* it = map->LowerBound(type);
        const SInterfaceEntry* last = map->end();
        return (it == last || TYPE < it->type ? last : it)->component;
    }
};

template<class T> inline bool IS_THING_ALIVE(T* thing) { return thing != 0 && !thing->Dead; }

struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* PItem;                                // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    const CThing* GetPItem() const;                     // 0x00A01B50
    void SetPItem(const CThing* item);                  // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        const CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void UpdateCoordSystem();                   // slot 24 (+0x60)
    unsigned char _pad04[0xa0];                         // +0x04..+0xa3
    const CThing* GetObservedThing() const;             // 0x0084F1B0
};
class CZTargetNew : public CBase {
public:
    CIntelligentPointerThing PTargetedThing;            // +0xa4
    float BaseOffsetXY;                                 // +0xac
    float BaseHeightOffset;                             // +0xb0
    float Theta;                                        // +0xb4
    float DeltaBaseOffsetXY;                            // +0xb8
    float DeltaBaseHeightOffset;                        // +0xbc
    float DeltaTheta;                                   // +0xc0
    float TargetBaseOffsetXY;                           // +0xc4
    float TargetBaseHeightOffset;                       // +0xc8
    float StringLengthXY;                               // +0xcc
    float CageRadius;                                   // +0xd0
    C3DVector XAxis;                                    // +0xd4
    C3DVector YAxis;                                    // +0xe0
    long FramesForTransition;                           // +0xec
    void InitialiseTransitionVariables();               // 0x00872490
    virtual void UpdateCoordSystem();
};
}

void NCameraModes::CZTargetNew::UpdateCoordSystem()
{
    const CThing* thing = GetObservedThing();
    if (thing->HasTC(8)) {
        const CThing* target = thing->GetTC<8>()->GetTargetedThing();
        if (IS_THING_ALIVE(target)) {
            const C3DVector& targetPos = target->GetPosition();
            const C3DVector& pos = GetObservedThing()->GetPosition();
            C3DVector axis(pos.X - targetPos.X, pos.Y - targetPos.Y, 0.0f);
            if (axis.GetLengthSquaredXY() > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
                axis.Normalise();
                XAxis = axis;
                YAxis = XAxis.GetPerpendicularXY();
            }
            if (PTargetedThing.GetPItem() != target)
                InitialiseTransitionVariables();
        }
        if (PTargetedThing.GetPItem() != target)
            PTargetedThing = target;
    }
}

// ===== harness =====
const float g_epsilon_0129ba3c = 1e-4f;
#define V(n) void CTCBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17)
V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35)
#undef V
const CThing* CTCBase::GetTargetedThing() const { return 0; }
#define V(n) void NCameraModes::CBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17)
V(18) V(19) V(20) V(21) V(22) V(23)
#undef V
void NCameraModes::CBase::UpdateCoordSystem() {}
struct CTargetingTC : CTCBase { const CThing* Target; const CThing* GetTargetedThing() const { return Target; } };
static const CThing* g_observed;
const CThing* NCameraModes::CBase::GetObservedThing() const { return g_observed; }
static C3DVector g_pos[2]; static const CThing* g_things[2];
const C3DVector& CThing::GetPosition() const { return this == g_things[0] ? g_pos[0] : g_pos[1]; }
void C3DVector::Normalise() { float l = (float)sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
const SInterfaceEntry* CInterfaceMap::LowerBound(const long& key) const
{ const SInterfaceEntry* p = _M_start; while (p != _M_finish && p->type < key) ++p; return p; }
static int g_inits, g_sets, g_live;
void NCameraModes::CZTargetNew::InitialiseTransitionVariables() { ++g_inits; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) { ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
const CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(const CThing* item) { PItem = item; ++g_sets; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
struct CTestMode : NCameraModes::CZTargetNew { };
static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
static int Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
int main()
{
    static CThing observed, target;
    CTargetingTC tc; tc.Target = &target;
    SInterfaceEntry entries[1]; entries[0].type = 8; entries[0].component = &tc;
    observed.Interfaces._M_start = entries; observed.Interfaces._M_finish = entries + 1;
    g_things[0] = &observed; g_things[1] = &target;
    g_pos[0] = C3DVector(3.0f, 4.0f, 9.0f); g_pos[1] = C3DVector(0.0f, 0.0f, -5.0f);
    g_observed = &observed;
    static unsigned char storage[sizeof(NCameraModes::CZTargetNew)];
    NCameraModes::CZTargetNew& mode = *(NCameraModes::CZTargetNew*)storage;   /* fields only, no vtable needed */
    mode.PTargetedThing.PItem = 0;
    mode.XAxis = C3DVector(9, 9, 9); mode.YAxis = C3DVector(9, 9, 9);
    int ok = 1;
    observed.TCFlags = 0;                          /* no targeting TC: nothing changes */
    mode.NCameraModes::CZTargetNew::UpdateCoordSystem();
    ok = ok && g_inits == 0 && g_sets == 0 && Is(mode.XAxis, 9, 9, 9);
    observed.TCFlags = 1 << 8;                     /* live new target: axes, transition, remembered */
    mode.NCameraModes::CZTargetNew::UpdateCoordSystem();
    ok = ok && g_inits == 1 && mode.PTargetedThing.PItem == &target && Is(mode.XAxis, 0.6f, 0.8f, 0.0f)
        && Is(mode.YAxis, 0.8f, -0.6f, 0.0f) && g_live == 0;
    g_pos[0] = C3DVector(0.0f, 0.0f, 1.0f);         /* same target, degenerate direction: axes kept, no restart */
    mode.NCameraModes::CZTargetNew::UpdateCoordSystem();
    ok = ok && g_inits == 1 && Is(mode.XAxis, 0.6f, 0.8f, 0.0f);
    int sets = g_sets;
    target.Dead = 1;                               /* dead target: still the targeted thing, nothing else */
    mode.NCameraModes::CZTargetNew::UpdateCoordSystem();
    ok = ok && g_inits == 1 && g_sets == sets;
    tc.Target = 0;                                 /* target lost: pointer cleared */
    mode.NCameraModes::CZTargetNew::UpdateCoordSystem();
    ok = ok && g_inits == 1 && mode.PTargetedThing.PItem == 0 && g_live == 0;
    if (!ok) { printf("00872ec0_TEST FAIL\n"); return 1; }
    printf("00872ec0_TEST PASS\n");
    return 0;
}
