// CTCHighlightItem::`scalar deleting destructor'(unsigned int) -- its vtable, DoStopRunning, ~PParticleEmitter, ~CTCBase, free for flag bit 0
// Leaf: ??_GCTCHighlightItem
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe 0x0077D140, 51 bytes; CTCHighlightItem vtable 0x0126AF14 slot 0 (rtti-vtables.tsv). The
// manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Callees: CTCHighlightItem::DoStopRunning 0x0077CCA0 (the destructor body), CBaseIntelligentPointer::
// ~CBaseIntelligentPointer 0x00A01C10 (+0x20), CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCHighlightItem (0x28 bytes, same in retail). engine/CTCHighlightItem.h is a flat dump
// (vptr as a data word, the intelligent pointer as bytes) that cannot carry the base destructor and the
// member's destructor, so the class is restated with the same member names and offsets.
// Shape: a user virtual destructor whose body calls DoStopRunning after the vtable store; the intelligent
// pointer's generated destructor calls the base destructor directly; then the out-of-line CTCBase destructor.
class CThing;

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


#include "engine/C3DVector.h"

struct CTCHighlightItem : CTCBase {
    bool                StartRunning;                    // +0x0c
    bool                Running;                         // +0x0d
    long                LifeTimer;                       // +0x10
    C3DVector           StartPos;                        // +0x14
    CIntelligentPointer<CThing> PParticleEmitter;                // +0x20
    void DoStopRunning();                                // 0x0077CCA0
    virtual ~CTCHighlightItem() { DoStopRunning(); }
};

CTCHighlightItem* NewCTCHighlightItem()
{
    return new CTCHighlightItem;
}