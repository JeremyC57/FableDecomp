// CAIStateGroup_MinionReactToCorpse::IsRunnable() -- find the nearest dead creature, latch it, and react when it is
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToCorpse@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// still around, its corpse component (0x40) is not yet handled, it was an ally, and the creature can see its spot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00936A60, length 304 bytes (+ int3 padding).
// Callees: the physics-position accessor @ 0x006A4D00, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestLeafThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature> @ 0x00936950,
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (vtable 0x0128B080), CVectorMap::LowerBound @ 0x0040F020
// (0x40 = bit 0 of +0x28, entry form), AreThingsAllies @ 0x0076D4F0 (__fastcall), CThing::GetPosition @ 0x004C73D0,
// the creature's look test (const C3DVector&, bool) @ 0x00665260 (manifest label propagated).
// Shape: the dead creature is zeroed up front and filled through the out slot; the empty CThingFilter_None is one
// frame byte; the final call's bool is returned directly.
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

#include "engine/C3DVector.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
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

class CThingSearchTools;
class CAIStateGroupBase {
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)
private: public:
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
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;      // 0x006A4D00
};
struct CThingPos : CThing {
};
struct CThingCreatureLook : CThing {
    bool CanSeePosition(const C3DVector& pos, bool checkLineOfSight);  // 0x00665260
};
bool __fastcall AreThingsAllies(CThing* a, CThing* b);   // 0x0076D4F0
class CThingFilter_None { public:
    CThingFilter_None() { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingDeadCreature;
class CThingSearchTools { public:
    bool GetPBestLeafThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare,
        CThingDeadCreature** out) const;              // 0x00936950
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the search GetPBestLeafThing runs (returns the best leaf thing or 0)
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestLeafThing @ 0x00936950 (compiled in retail's TU): clears the out pointer, stores the best
// thing found, reads the comparator through its const reference; no argument escapes.
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingDeadCreature** out) const
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
struct CTCCorpse {                              // interface 0x40
    unsigned char _pad[0x0c];
    bool m_handled;                             // +0x0c
};
struct CIntelligentPointerDead : CBaseIntelligentPointer {   // CIntelligentPointer<CThingDeadCreature> (vtable 0x0128B080)
    virtual void v00();
    CIntelligentPointerDead(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerDead& operator=(const CIntelligentPointerDead& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};

struct CAIStateGroup_MinionReactToCorpse : CAIStateGroupBase {
    CIntelligentPointerDead PCorpse;            // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToCorpse::IsRunnable()
{
    CThingDeadCreature* corpse = 0;
    if (PeekThingSearchTools().GetPBestLeafThing(CThingFilter_None(),
            CThingCompare_Nearest(((CThingPhysical*)PBrain->PCreature)->PeekPos()), &corpse)) {
        PCorpse = CIntelligentPointerDead((CThing*)corpse);
        if (IS_THING_ALIVE(PCorpse)) {
            CThing* dead = ((CThing*)PCorpse.GetPItem());
            CTCCorpse* state;
            if (dead->HasTC(TCI_DISCOVERY) && (dead->GetTC(TCI_DISCOVERY, &state), !state->m_handled)) {
                CThing* creature = PBrain->PCreature;
                if (AreThingsAllies(creature, ((CThing*)PCorpse.GetPItem()))) {
                    CThingCreatureLook* looker = (CThingCreatureLook*)PBrain->PCreature;
                    return looker->CanSeePosition(((CThingPos*)PCorpse.GetPItem())->GetPos(), true);
                }
            }
        }
    }
    return false;
}