// CCreatureActionBase::CanAnimationTransitionOut() const -- true unless bit 0 of the current animation entry's flags is set
// Leaf: ?CanAnimationTransitionOut@CCreatureActionBase@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006928B0, 14 bytes: `mov ecx,[ecx+74h] ; call 0x005DC210 ; not eax ; and eax,1 ; ret` (PAnimEntry's
//   Data at +0x74; 0x005DC210 = ?GetFlags@CAnimationEntry@@QBEJXZ, Ego_r.pdb 0x0063BE40 + FableWin.pdb 0x01FA0DF0).
// Identity: Ego_r.pdb 0x00519210 + FableWin.pdb 0x01DB9570 ?CanAnimationTransitionOut@CCreatureActionBase@@MBE_NXZ (protected virtual const);
//   retail CCreatureActionBase vftable 0x01259D24 slot 67 (+0x10C) = 0x006928B0, bound under this spelling by the landed vtable emitters.
// NF259 (2026-11-22, q12730): re-spelled from the free __fastcall stand-in
//   ?CCreatureActionBase_CanAnimationTransitionOut@@YI_NPAVCCreatureActionBase@@@Z (which called GetFlags as the made-up ?cat_probe@@YIHPAVCAnimationEntry@@@Z)
//   onto the shared engine/CCreatureActionBase.h (retail branch, FULL_MEMBERS: PAnimEntry +0x74) and engine/CAnimationEntry.h (GETFLAGS opt-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CAnimationEntry.h"

bool CCreatureActionBase::CanAnimationTransitionOut() const
{
    return (PAnimEntry->GetFlags() & 1) == 0;
}
