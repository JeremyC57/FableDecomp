// CTCDParticleEmitter::OnKill() -- kill the live emitter, and register this emitter's thing as a dependent of the (living) attached thing's interface-0xD8 component
// Leaf: OnKill
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90). Spellings unified: 00a01b50: ?GetPItem@CBaseIntelligentPointer@@QBEPAXXZ -> ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0A60, length 137 bytes (+ int3 padding). Vtable 0x0125FEEC slot 14 (catalog
// "vfunc_14"; slot 14 of the CTCBase vtables is OnKill).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x5), NParticleEngine::CParticleEmitter::Kill @ 0x00AEE8C0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0xD8, word 6 bit 24 at +0x38),
// the component's AddDependentThing(CThing*) @ 0x0071A8F0 (pushes a CIntelligentPointer<CThing> into its list at +0xC; the
// manifest attributes the body to CCreatureActionBase, a byte-twin label).
// Layout: CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590: PEmitter +0x14, PAttachedToThing +0x1C.
// Shape: inline IS_THING_ALIVE then a checked GetInterface; the parent is pushed before the component moves into ecx.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCDependentThings {                     // interface 0xD8 (class not established)
    void AddDependentThing(CThing* thing);      // 0x0071A8F0
};
namespace NParticleEngine {
class CParticleEmitter { public:
    void Kill();                                // 0x00AEE8C0
};
}
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CEmitterPointer : CBaseIntelligentPointer {
    NParticleEngine::CParticleEmitter* GetPItem() const { return (NParticleEngine::CParticleEmitter*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThing* GetPItem() const { return (CThing*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IS_THING_ALIVE(const CThingPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void OnKill();                      // slot 14
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCDParticleEmitter : public CTCBase { public:
    void*           EmitterName;                // +0x0c CCharString
    long            ParticleTypeID;             // +0x10
    CEmitterPointer PEmitter;                   // +0x14
    CThingPointer   PAttachedToThing;           // +0x1c
    virtual void OnKill();
};

void CTCDParticleEmitter::OnKill()
{
    if (PEmitter.GetPItem())
        PEmitter.GetPItem()->Kill();
    if (IS_THING_ALIVE(PAttachedToThing)) {
        CTCDependentThings* dependents;
        if (PAttachedToThing.GetPItem()->GetTC(TCI_PARTICLE_ATTACHMENT_TRACKER, &dependents))
            dependents->AddDependentThing(ParentThing);
    }
}