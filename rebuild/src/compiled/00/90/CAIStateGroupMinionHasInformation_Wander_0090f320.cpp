// CAIStateGroup_MinionHasInformation::Wander() -- wander; when the information component (0xAA) is in state 4, idle,
// Leaf: ?Wander@CAIStateGroup_MinionHasInformation@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// then unless the creature is already within the component's radius (XY) of its spot, walk up to it and idle
// again.
// Returns true to keep the group going, false when the wander failed or (after terminating) the approach failed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F320, length 313 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xAA, bit 10 of +0x34, the bitset test's bool
// materialised to a frame byte), CAIStateGroupBase::AIAction_MinionWander(EMovementType, CThingBuilding*) @ 0x0097F870,
// AIAction_MinionIdle @ 0x0097FA00 (x2), CThing::GetPosition @ 0x004C73D0, AIAction_MoveToPosProximity @ 0x0097E890
// (C3DVector by value through the user copy constructor), Terminate @ 0x008FCF40.
// Shape: the spot and radius are named copies taken before the position call; the XY delta stays on the FPU and its
// squared length and radius*radius are NAMED float locals (`fcomp st(1) ... fstp st(0)`; the inline compare gives
// fcompp) tested as `!(d2 < r2)`; the two failure exits are separate `{ Terminate(); return false; }` blocks that
// tail-merge; the reached flag reuses the materialised test byte.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
public:
    void Terminate();   // 0x008FCF40
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};
class C2DVector { public:
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};
struct CThingPos : CThing {
};
struct CTCInformation {                         // interface 0xAA
    unsigned char _pad[0x14];
    C3DVector m_spot;                           // +0x14
    unsigned char _pad20[4];
    float m_radius;                             // +0x24
    unsigned char _pad28[0x38 - 0x28];
    long m_state;                               // +0x38
};
class CThingBuilding;

struct CAIStateGroup_MinionHasInformation : CAIStateGroupBase {
    long LastFrameWaved;                        // +0x14
    long LastFrameWandered;                     // +0x18
    long FrameInitialised;                      // +0x1c
    long WavesWithoutCalling;                   // +0x20
    long AIAction_MinionWander(long movement, CThingBuilding* building);   // 0x0097F870
    long AIAction_MinionIdle();                 // 0x0097FA00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    bool Wander();
};

bool CAIStateGroup_MinionHasInformation::Wander()
{
    CTCInformation* info;
    PBrain->PCreature->GetTC(TCI_AI_SCRATCHPAD, &info);
    if (AIAction_MinionWander(3, 0) != 2)
        return false;
    if (info->m_state != 4)
        return true;
    if (AIAction_MinionIdle() != 2) {
        Terminate();
        return false;
    }
    C3DVector spot = info->m_spot;
    float radius = info->m_radius;
    const C3DVector& pos = ((CThingPos*)PBrain->PCreature)->GetPos();
    C2DVector delta;
    delta.x = pos.x - spot.x;
    delta.y = pos.y - spot.y;
    float distanceSquared = delta.GetLengthSquared();
    float radiusSquared = radius * radius;
    if (!(distanceSquared < radiusSquared)) {
        bool reached = false;
        if (AIAction_MoveToPosProximity(1, spot, radius, &reached, 3, true, false, true, 0, true, 0.0f) != 2) {
            Terminate();
            return false;
        }
        if (AIAction_MinionIdle() != 2) {
            Terminate();
            return false;
        }
    }
    return true;
}