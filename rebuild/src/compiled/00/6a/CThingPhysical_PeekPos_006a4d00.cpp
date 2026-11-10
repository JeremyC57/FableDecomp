// CThingPhysical::PeekPos() const -- the thing's position: its physics component's Position
// Leaf: ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4D00, 7 bytes: `mov eax,[ecx+60h] ; add eax,0Ch ; ret` (CThing::PhysicsTC +0x60,
//   CTCPhysicsBase::Position +0x0C).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ; engine/CThingPhysical.h (retail) declares
//   it, and CThing::PhysicsTC +0x60 (static assert in engine/CThing.h) / CTCPhysicsBase::Position +0x0C are the words read.
// Name fix (agentNF5 2026-11-10): was the stand-in `C006a4d00::M` (?M@C006a4d00@@QAEJXZ) over a padded local struct; now the shared
//   retail headers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
const C3DVector& CThingPhysical::PeekPos() const
{
    return GetPhysicsTC()->GetPosition();
}
