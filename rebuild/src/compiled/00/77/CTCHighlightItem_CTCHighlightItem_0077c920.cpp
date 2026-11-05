// CTCHighlightItem::CTCHighlightItem(CThing*) -- CTCBase(owner), own vtable, an empty particle-emitter pointer, then the running flags and the life timer cleared
// Leaf: ??0CTCHighlightItem
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C920, length 52 bytes.
// Label: the catalog name "CTCVolumeContainmentTracker::CTCVolumeContainmentTracker" is propagated: the vtable stored
// is 0x0126AF14 = CTCHighlightItem (rtti-vtables.tsv), and its factory 0x004D4548 allocates 0x28 bytes = sizeof
// CTCHighlightItem in Ego_r.pdb (the landed factory carries the same wrong class name).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Shape: PParticleEmitter is the only constructed member (its inline constructor right after the vtable store);
// Running, StartRunning and LifeTimer are stored after it in that order, so they are body assignments; StartPos
// (C3DVector, no constructor body) is left alone.
// Layout: Ego_r.pdb CTCHighlightItem (no containers, retail = PDB): StartRunning +0x0C, Running +0x0D, LifeTimer
// +0x10, StartPos +0x14, PParticleEmitter +0x20. No engine header.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
};

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


#include "engine/C3DVector.h"

struct CTCHighlightItem : CTCBase {
    bool                StartRunning;       // +0x0C
    bool                Running;            // +0x0D
    long                LifeTimer;          // +0x10
    C3DVector           StartPos;           // +0x14
    CIntelligentPointer<CThing> PParticleEmitter;   // +0x20
    virtual void v00();
    CTCHighlightItem(CThing* thing);
};

CTCHighlightItem::CTCHighlightItem(CThing* thing)
    : CTCBase(thing)
{
    Running = false;
    StartRunning = false;
    LifeTimer = 0;
}