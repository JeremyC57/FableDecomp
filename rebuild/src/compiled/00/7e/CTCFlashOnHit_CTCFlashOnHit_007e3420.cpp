// CTCFlashOnHit::~CTCFlashOnHit() -- if the parent thing is alive, take this component's tint off its graphic appearance; then ~CTCBase (tail jump)
// Leaf: ??1CTCFlashOnHit@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3420, 42 bytes (int3 padding 0x007E344A..F), thiscall, stores vtable 0x0123E034 first,
//   tail jmp ~CTCBase.
// Identity (proven, two sources): (1) the vtable it stores, 0x0123E034, is RTTI .?AVCTCFlashOnHit@@ (rtti-vtables.tsv); the only
//   caller is that vtable's slot-0 scalar deleting destructor 0x004D62D1 (call at 0x004D62D4); (2) Ego_r.pdb and FableWin.pdb
//   both list a user ??1CTCFlashOnHit@@UAE@XZ (Ego_r donor 0x007DECA0 right after FrameUpdate 0x007DEC30, as retail
//   0x007E3420 follows FrameUpdate 0x007E33B0, slot 9). FrameUpdate's last frame calls the same RemoveTintColour(this).
// Callees: NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void*) 0x004BFAE0 (Ego_r 0x004C49F0 / FableWin 0x01D0A000
//   ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z; the tint id is this component), CTCBase::~CTCBase
//   0x00686830 (tail).
// Layout: engine/CTCBase.h retail branch (FABLE_RETAIL_CTCBASE) and engine/CClass.h retail branch (FABLE_RETAIL_CCLASS, 0x14);
//   CThing is restated over CClass with every Ego_r.pdb member at its retail offset (Ego_r less 0x10: bitset 0x24, CVectorMap
//   0x10, list 4), non-touched members as opaque storage: PTCGraphicAppearance +0x64, Dead = bit 0 of +0x91, sizeof 0x98.
//   CTCFlashOnHit members (Ego_r.pdb, no containers): FadeInTime +0x0C, FadeOutTime +0x10, LifeTime +0x14, Timer +0x18.
// Shape: if (ParentThing && !ParentThing->IsDead()) through the inline PDB accessors IsDead() / GetTCGraphicAppearance()
//   (FableWin). The PDB template IS_THING_ALIVE(ParentThing) (by const reference) gives the same instructions but keeps the
//   vtable store ahead of the ParentThing load (42v42); retail loads and tests ParentThing before the store, which the plain
//   member test reproduces.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"                               // CClass : CBaseObject : CBaseClass, sizeof 0x14

namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase {
public:
    void RemoveTintColour(void* id);                     // 0x004BFAE0
};
}
class CTCDriverBase;
class CTCPhysicsBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);

class CTCFlashOnHit : public CTCBase {                   // vtable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@ : CTCBase)
public:
    CTCFlashOnHit(CThing& thing);
    virtual ~CTCFlashOnHit();
    virtual bool HasFrameUpdate() const;                 // slot 8  0x004D62CE
    virtual void FrameUpdate();                          // slot 9  0x007E33B0
    virtual ETCInterfaceType GetInterfaceType() const;   // slot 21 0x004D62B8
    virtual CCharString GetClassTypeName() const;        // slot 22 0x004D62BE
    void SetLifeTime(long time);                         // 0x007E33A0
    long FadeInTime;                                     // +0x0c
    long FadeOutTime;                                    // +0x10
    long LifeTime;                                       // +0x14
    long Timer;                                          // +0x18
};

CTCFlashOnHit::~CTCFlashOnHit()
{
    if (ParentThing && !ParentThing->IsDead())
        ParentThing->GetTCGraphicAppearance()->RemoveTintColour(this);
}