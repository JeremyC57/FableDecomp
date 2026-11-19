// CTCTavernGameCardPairs::EnterGame() -- enter the card game, then create the pointer game piece from the card def and place it at the game position, raised by the def's pointer height
// Leaf: ?EnterGame@CTCTavernGameCardPairs@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D60C0, length 143 bytes (padded row 144); CTCTavernGameCardPairs vtable 0x0126F6FC slot 29
// (an EnterGame override: it starts with CTCTavernGameCardBase::EnterGame).
// Callees: CTCTavernGameCardBase::EnterGame @ 0x008F1E30, CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20,
// CTCTavernGame::CreateGamePiece(long) @ 0x005EA9B0, CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float)
// __fastcall @ 0x005EABC0, the def's virtual slot 1 (destroy).
// Layout (retail CTCTavernGameCardBase, see 0x008F1CD0): GamePositionRHSet +0xF0, GamePositionPos +0x108, Pointer +0x140. Card def: pointer
// piece def index +0x3C, pointer height +0x48.
// Shape: null-tested CDefPointer dtor with the def's inline ReleaseTemporary(); the offset is a `C3DVector(0, 0, height)` temporary (Z via
// x87 before the pushes, the zero words stored after); the new piece goes from eax both into Pointer and into ecx.
// PositionMesh (sweep3, FINDINGS chunk 481 "PositionMesh's 5th parameter is float"): Ego_r/FableWin
//   ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z -- a static __fastcall member taking
//   (CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float); C3DVector, CRightHandedSet and CThing
//   are classes (V in every PDB signature).
// Shared headers (NF96 q11339): engine/CTCTavernGameCardBase.h / CTCTavernGameCardPairs.h (the 0x007D6870 recipe; overlay opt-ins
//   FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENTERGAME and FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES declare the PDB overrides),
//   CTavernGameCardBaseDef.h (PointerModel / PointerHeightOffset, the PDB names of the old model's PointerDef / PointerHeight) and
//   CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its PDB spelling; the row's own name was the placeholder
//   CTCTavernGameCardPairs::vfunc_29 (Ego_r.pdb 0x007D3410 / FableWin.pdb 0x0274A260 ?EnterGame@CTCTavernGameCardPairs@@UAEXXZ; vtable
//   0x0126F6FC slot 29 = 0x007D60C0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENTERGAME
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardPairs.h"
void CTCTavernGameCardPairs::EnterGame()
{
    CTCTavernGameCardBase::EnterGame();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    Pointer = CreateGamePiece(def->PointerModel);
    CTCTavernGame::PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}
