// CTCTavernGameCardBase::CardsMoving() -- card-phase step while cards move: wait for the pack to settle, then advance the state (20 -> 21, 27 -> 6, anything else -> 0), or in state 26 gather the dealt cards back onto the pack position (stacked above the remaining pack), drop that many cards from the front of the pack, restack the pack and put the gathered cards on its end, then go to 27
// Leaf: ?CardsMoving@CTCTavernGameCardBase@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2280, length 766 bytes (ret). Name: Ego_r.pdb; vtable slot 52 of
// CTCTavernGameCardBase (Blackjack's 0x007DA070 jumps here; the landed CTCTavernGameCardBase::FrameUpdate @ 0x008F1D40 calls
// it for states 20/26/27). The catalog label "CTCTavernGameCardBase_OnFrameUpdate" is a descriptive gap-scan name.
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20 (twice: the outer def
//          and the one inside the inlined GetPackPosition 0x008F0B80); CTCTavernGame::SetState(ETavernGameStates)
//          0x005E9B20; memmove 0x00BFEAE6 (vector::erase's __copy_trivial); vector<CCard*>::_M_range_insert<CCard**>(pos,
//          first, last, const forward_iterator_tag&) 0x008F28A0 (the out-of-line half of Pack.insert); the def's virtual
//          slot 1 (Destroy) on the last release. CCard::Move 0x008F0660 is inlined (immediately == false).
// Layout: CTCTavernGame::TavernGameState +0xc0; CTCTavernGameCardBase Cards +0x128, Pack +0x134 (STLport vector<CCard*>,
//         as the landed FrameUpdate); CCard (Ego_r.pdb, byte-packed) Moving +0x26, MoveFraction +0x20, LocalPos +0x27,
//         NewLocalPos +0x33, CurrentPos +0x3f; CTavernGameCardBaseDef (Ego_r.pdb) CardWidth +0x2c, CardHeight +0x30,
//         CardSeparation +0x5c, TableSeparation +0x60, PackOffset +0x64.
// Shape: the settle test is `Moving == true` (cmp byte, dl=1). Every state branch ends in its own `return`, which gives
//        retail's duplicated release + epilogue per branch (an if/else chain cross-jumps the SetState calls). GetPackPosition
//        and Move are same-TU inlines; GetCardDef is the real same-TU body, kept a call, with the GetSubDef body visible
//        (engine/CDefClassBase_GetSubDef.h, as CardsTurning 0x008F0EB0): the inner def's release has no null test
//        (load/dec/store), the outer def keeps its null test. The restack height is `53 - i` over a second counter that VC strength-reduces into the decrementing
//        memory counter fild reads; its 53 is stored after the hoisted pack-position copies (a plain `height = 53; --height`
//        counter is stored before the loop guard). The erase loop is STLport `erase(begin())` (memmove of the tail in bytes,
//        then `_M_finish -= 1` with the -4 hoisted into ebx); the insert passes a random_access_iterator_tag temporary.
// Port (NF96 part 2, q11339 / the GetCardDef one-definition item): the row defined the PDB GetCardDef symbol with a stand-in body
//   (made-up callee FindCardDef) over local std::vector / CCard / CDefClassBase / CTavernGameCardBaseDef / CDefPointer / CTCTavernGame /
//   CTCTavernGameCardBase models, and itself as non-virtual ?CardsMoving@CTCTavernGameCardBase@@QAEXXZ. It now sits on the shared
//   headers with the define / include set of its landed TU-mate CardsTurning 0x008F0EB0 (engine/card_sorting_types.h, the Ego_r.pdb
//   class: CardsMoving is virtual slot 52, ?CardsMoving@CTCTavernGameCardBase@@UAEXXZ in Ego_r.pdb + FableWin.pdb), with the real
//   same-TU bodies of GetCardDef 0x008F0B20, CCard::Move 0x008F0660 and GetPackPosition 0x008F0B80, and STLport's vector insert
//   through FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT (the out-of-line _M_range_insert 0x008F28A0, now bound with its allocator argument).
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
#define FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
#include "stlport/fable_containers.h"                
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"

// 0x008F0B20 (landed row; its body verbatim, same TU in retail)
// Fidelity: same-TU strong definition of 0x008F0B20 (retail TU-mate)
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> new_pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&new_pdef);
    *pdef = new_pdef;
}

// 0x008F0660 (landed row; real body, same TU in retail)
void CCard::Move(const C3DVector& position, bool immediately)
{
    if (immediately) {
        LocalPos = position;
        NewLocalPos = position;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = position;
    MoveFraction = 0.0f;
}

// 0x008F0B80 (landed row; its body verbatim, same TU in retail)
// Fidelity: same-TU strong definition of 0x008F0B80 (retail TU-mate)
C3DVector CTCTavernGameCardBase::GetPackPosition()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
}

void CTCTavernGameCardBase::CardsMoving()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    for (std::vector<CCard*>::iterator it = Pack.begin(); it != Pack.end(); ++it)
        if ((*it)->Moving == true)
            return;
    if (TavernGameState == 20) {
        SetState((ETavernGameStates)21);
        return;
    }
    if (TavernGameState == 26) {
        C3DVector pack_pos = GetPackPosition();
        for (unsigned int i = 0; i < Cards.size(); ++i)
            Cards[i]->Move(C3DVector(pack_pos.X, pack_pos.Y, (Cards.size() - i) * def->CardSeparation + def->TableSeparation), false);
        for (unsigned int j = 0; j < Cards.size(); ++j)
            Pack.erase(Pack.begin());
        long i = 0;
        for (std::vector<CCard*>::iterator card = Pack.begin(); card != Pack.end(); ++card, ++i)
            (*card)->Move(C3DVector(pack_pos.X, pack_pos.Y, (53 - i) * def->CardSeparation + def->TableSeparation), false);
        Pack.insert(Pack.end(), Cards.begin(), Cards.end());
        SetState((ETavernGameStates)27);
        return;
    }
    if (TavernGameState == 27) {
        SetState((ETavernGameStates)6);
        return;
    }
    SetState((ETavernGameStates)0);
}
