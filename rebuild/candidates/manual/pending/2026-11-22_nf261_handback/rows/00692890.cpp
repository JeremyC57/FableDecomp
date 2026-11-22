// CCreatureActionBase::AllowsCombinationAnims() const -- true unless bit 3 (0x8) of the current animation entry's flags is set
// Leaf: ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692890, 18 bytes: `mov ecx,[ecx+74h] ; call 0x005DC210 ; shr eax,3 ; not eax ; and eax,1 ; ret`
//   (PAnimEntry's Data at +0x74; 0x005DC210 = ?GetFlags@CAnimationEntry@@QBEJXZ, Ego_r.pdb 0x0063BE40 + FableWin.pdb 0x01FA0DF0).
// Identity: Ego_r.pdb 0x005191F0 + FableWin.pdb 0x01DB9540 ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ (protected virtual const);
//   retail CCreatureActionBase vftable 0x01259D24 slot 66 (+0x108) = 0x00692890, between OverrideFaceWithDefault (slot 65, 0x00692880) and
//   CanAnimationTransitionOut (slot 67, 0x006928B0), bound under this spelling by the landed vtable emitter 0x006DF800
//   (CCreatureAction_UnsheatheToShowToSheatheTrophy; 0x00693B30 binds the public UBE form, 0x006F5C10 binds this symbol at 0x0062EA10: NF261 NOTES).
// NF261 (2026-11-22, q12741): re-spelled from the stand-in ?AllowsCombinationAnims@CCreatureActionBase_Methods@@QAEDXZ (a struct derived from
//   CCreatureActionBase, a local CAnimationEntry model and a cast of PAnimEntry's Data) onto the shared engine/CCreatureActionBase.h (retail
//   branch, FULL_MEMBERS) and engine/CAnimationEntry.h (GETFLAGS opt-in), as the landed siblings 0x00692880 / 0x006928B0 (nf259).
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

bool CCreatureActionBase::AllowsCombinationAnims() const
{
    return (PAnimEntry->GetFlags() & 0x8) == 0;
}
