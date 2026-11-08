// CTCFishingSpot::`scalar deleting destructor'(unsigned int) -- stores the CTCFishingSpot vtable, removes the spot's particles, destroys the three intelligent particle pointers (+0x1c, +0x14, +0x0c), runs CTCBase::~CTCBase and frees on bit 0
// Leaf: ??_GCTCFishingSpot
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B06C0, length 67 bytes (vtable 0x0126D51C slot 0).
// Callees: CTCFishingSpot::RemoveParticles @ 0x007B01C0, CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10 (three times), CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk", propagated),
// operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CTCFishingSpot vtable 0x0126D51C.
// Layout: PDB CTCFishingSpot (40 bytes): PParticle1 +0x0c, PParticle2 +0x14, PParticleFish +0x1c
// (CIntelligentPointer<CThing>, 8 bytes each: vptr + item), Enabled +0x24.
// Shape: own vtable store first, the user destructor body (RemoveParticles) before the member
// destructors, which run in reverse declaration order; a `new` factory pulls the `??_G` into the TU.
#include "fable_compiler.h"
class CThing;

class CTCBase { public:                                   // engine/CTCBase.h offsets, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& owner);                        // 0x00686800
    virtual ~CTCBase();                                    // 0x00686830
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCFishingSpot : public CTCBase { public:                  // vtable 0x0126D51C
    CBaseIntelligentPointer PParticle1;                // +0x0c
    CBaseIntelligentPointer PParticle2;                // +0x14
    CBaseIntelligentPointer PParticleFish;             // +0x1c
    bool                Enabled;                   // +0x24
protected:
    void RemoveParticles();                        // 0x007B01C0  // protected (?RemoveParticles@CTCFishingSpot@@IAEXXZ, Ego_r + FableWin; respelled at the chunk 712 landing)
public:
    CTCFishingSpot(CThing& owner) : CTCBase(owner) { }
    FABLE_FORCEINLINE virtual ~CTCFishingSpot() { RemoveParticles(); }
};

CTCFishingSpot* Create_CTCFishingSpot(CThing& owner)
{
    return new CTCFishingSpot(owner);
}