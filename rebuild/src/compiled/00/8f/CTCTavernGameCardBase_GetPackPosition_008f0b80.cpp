// CTCTavernGameCardBase::GetPackPosition() -- the card pack's table position: the def's pack offset scaled by the card size, on the table plane (z 0)
// Leaf: ?GetPackPosition@CTCTavernGameCardBase@@QAE?AVC3DVector@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0B80, length 71 bytes (ret 4: hidden C3DVector result). Name: Ego_r.pdb
// `?GetPackPosition@CTCTavernGameCardBase@@QAE?AVC3DVector@@XZ`; the catalog label "CTCTavernGame::GetBanter" is
// propagated over this body and rejected.
// Callee: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20 (on `this`), then the
// def's virtual slot 1 (destroy) when the local reference is the last one.
// Ego_r.pdb CTavernGameCardBaseDef: CardWidth +0x2c, CardHeight +0x30, PackOffset (C2DVector) +0x64.
// Shape: `fld [a] ; fmul [b]` is `b * a`; the constructor arguments are evaluated right to left (Y before X) and the 0.0f
// is an immediate store; the local def pointer is null-initialised in the `push ecx` slot after its address is pushed and
// released unconditionally after the read (load/dec/store of the count, no null test, flags from the `dec`) through a
// local pointer and a local count -- the member form re-reads the slot after the store (82v73).
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase) with the define / include set of the
//   landed same-TU ShuffleCards 0x008F10B0 (which carries this body and the real GetCardDef), CTavernGameCardBaseDef.h (CardWidth /
//   CardHeight / PackOffset) and CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 is declared only and binds its PDB
//   spelling (the call stays out of line, as retail; GetCardDef's real body is in this TU, as in 0x008F0EB0 / 0x008F10B0: with the
//   declaration alone VC7.1 assumes the def local escapes and the release re-reads it, DIFFER 78v73) (was a local CDefPointer<CTavernGameCardBaseDef> (non-const) model with a same-TU stand-in GetCardDef body over a made-up
//   callee FindRef_CTCTavernGameCardBase_GetCardDef_0, and local def / C2DVector models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE          // count-store schedule of the expanded GetPackPosition (0x008F116B)
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"
// 0x008F0B20 (landed row; real body, same TU in retail; strong here as in 0x008F0BD0 / 0x008F0EB0 / 0x008F10B0: one-definition item, NOTES)
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    *pdef = def;
}

C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
}
