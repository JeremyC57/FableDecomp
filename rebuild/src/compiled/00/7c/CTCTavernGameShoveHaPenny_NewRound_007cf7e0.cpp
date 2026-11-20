// CTCTavernGameShoveHaPenny::NewRound() -- coin-base NewRound, clear the board, find the obstacles, add the first coin at the board's centre line on the def's minimum coin position, then go to state 15
// Grade: RELOCATION_MATCH (header ctor landed 2026-10-31; was PENDING HEADER) (wave 2 sec3): retail def model (CSubDefClassBase chain, PDB CThing::PeekBaseDef, CDefPointer<const CTavernGameShoveHaPennyDef>) plus the landed GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 body as a same-TU noinline specialization: VC7.1 then keeps the handle's pointee in edi and drops the null test of the closing release (was 127v127 with an invented no-null-test holder). Graded RELOCATION_MATCH with engine/CDefPointer.h's text inlined plus CDefPointer(ENullDef) (007cf7e0.verify.cpp); this file compiles once the header declares that ctor.
// Leaf: ?NewRound@CTCTavernGameShoveHaPenny@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x007CF7E0, 127 bytes.
// Callees: CTCTavernGameCoinBase::NewRound() 0x008EF180 (non-virtual base call; manifest "CXMVPlaybackInfo" ctor label is a
//          propagation); CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer*) 0x007D1260 on the owner's
//          def holder (CThing+0x70; manifest "<CAICreatureWillPowerIndicatorDef>" is a propagated template label);
//          CTCTavernGameCoinBase::ClearBoard() 0x008EF950, ::FindObstacles() 0x008EFB00, ::AddNewCoin(const C3DVector&)
//          0x008EFA00; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's slot 1 on release. Float pool
//          0x0122F59C = 0.5.
// Layout: retail CTCTavernGameCoinBase::BoardSize +0x130 (PDB +0x14c; retail BoardPos +0x10c, BoardRHSet +0x118);
//         CTavernGameShoveHaPennyDef::PositionCoinDepth +0x1c8 (engine/CTavernGameShoveHaPennyDef.h; the old local model called it PositionCoinMinimum).
// Shape: the sub-def is copied once out of the escaped out slot (after the two board calls) into a holder that releases it
//        (`dec [edi+4] ; jne`, no null test); the start position is a 3-float temporary.
// GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 (landed): its retail body in the TU (same-TU mod-ref, out of line)
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameShoveHaPenny.h / CTavernGameShoveHaPennyDef.h (retail branches) from the
//   local CTCTavernGameCoinBase / ShoveHaPenny / def stand-ins (the 0x007CF9A0 recipe): SetState is CTCTavernGame's
//   (?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z, Ego_r.pdb 0x00647550 / FableWin.pdb 0x022D5AD0; was ?SetState@CTCTavernGameCoinBase@@QAEXJ@Z);
//   15 is ETGS_POSITION_COIN. Own leaf: the virtual override (vtable 0x0126F2F4 slot 32; Ego_r.pdb 0x007CDD20 / FableWin.pdb 0x0273AEB0
//   ?NewRound@CTCTavernGameShoveHaPenny@@UAEXXZ; was QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameShoveHaPenny.h"

template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer<const CTavernGameShoveHaPennyDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameShoveHaPennyDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCTavernGameShoveHaPenny::NewRound()
{
    CTCTavernGameCoinBase::NewRound();
    CDefPointer<const CTavernGameShoveHaPennyDef> def(NULL_DEF);
    ParentThing->PeekBaseDef().GetSubDef(&def);
    ClearBoard();
    FindObstacles();
    AddNewCoin(C3DVector(0.5f * BoardSize.X, def->PositionCoinDepth, 0.0f));
    SetState(ETGS_POSITION_COIN);
}
