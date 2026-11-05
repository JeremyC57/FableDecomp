// CTCFishingSpot::~CTCFishingSpot() -- store the fishing-spot vtable, remove the spot's particles, destroy the fish and the two splash particle pointers, then ~CTCBase
// Leaf: ??1CTCFishingSpot@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0300, 46 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830; no callers (the
//   slot-0 `??_G` 0x007B06C0 of vtable 0x0126D51C holds the same body inline).
// Identity (proven): the first store is vtable 0x0126D51C (RTTI .?AVCTCFishingSpot@@ : CTCBase : CBaseClassNonCopyable : CBaseClass);
//   Ego_r.pdb / FableWin.pdb ??1CTCFishingSpot@@UAE@XZ (a user dtor: the store and the RemoveParticles body); the body is the one
//   ??_GCTCFishingSpot 0x007B06C0 inlines (landed). FableWin.pdb puts ??_G 0x026D4490 then ??1 0x026D44D0; retail emits the ??1
//   COMDAT just before CTCFishingSpot::OnSerialise 0x007B0330 (landed).
// Callees: CTCFishingSpot::RemoveParticles 0x007B01C0 (?RemoveParticles@CTCFishingSpot@@IAEXXZ: protected in both PDBs; the landed
//   row and binding say QAEXXZ, NAME_FIXES.tsv), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (x3),
//   CTCBase::~CTCBase 0x00686830 (tail).
// Layout = Ego_r.pdb CTCFishingSpot (0x28, as the landed ctor 0x007B0150 / RemoveParticles 0x007B01C0): PParticle1 +0xc,
//   PParticle2 +0x14, PParticleFish +0x1c (CIntelligentPointer<CThing>, 8 bytes: vptr + PData), Enabled +0x24.
// Shape: own vtable store, the user body, then the member dtors in reverse order (CIntelligentPointer<T> declares no dtor, so the
//   implicit one is the base call) and the base dtor as the tail. Base: engine/CTCBase.h retail opt-in (vtable 0x0125BE0C).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable; ~CTCBase 0x00686830

class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CCharString;
class CPersistContext;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCFishingSpot : public CTCBase {                     // vtable 0x0126D51C, 0x28 bytes
public:
    CTCFishingSpot(CThing& thing);                          // 0x007B0150 ??0CTCFishingSpot@@QAE@AAVCThing@@@Z
    virtual ~CTCFishingSpot();
    virtual void OnSerialise(CPersistContext& context, bool loading);   //  1 0x007B0330
    virtual void OnInitialActivate();                       //  6 0x007B0140
    virtual bool HasFrameUpdate() const;                    //  8 0x007B06A0
    virtual void FrameUpdate();                             //  9 0x007B0360
    virtual ETCInterfaceType GetInterfaceType() const;      // 21 0x007B0670
    virtual CCharString GetClassTypeName() const;           // 22 0x007B0680
    virtual bool IsComponentSerialisable() const;           // 23 0x007B06B0
    bool IsEnabled() const;
    void SetAsEnabled(bool enabled);
protected:
    void RemoveParticles();                                 // 0x007B01C0
public:
    CIntelligentPointer<CThing> PParticle1;                 // +0x0c
    CIntelligentPointer<CThing> PParticle2;                 // +0x14
    CIntelligentPointer<CThing> PParticleFish;              // +0x1c
    bool                        Enabled;                    // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CTCFishingSpot) == 0x28);

CTCFishingSpot::~CTCFishingSpot()
{
    RemoveParticles();
}