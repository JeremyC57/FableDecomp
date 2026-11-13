#ifndef FABLE_ENGINE_CTCUSERACTIONPOINTBASE_H
#define FABLE_ENGINE_CTCUSERACTIONPOINTBASE_H
// New shared header (chunk 2029, row 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods), opt-in FABLE_RETAIL_CTCUSERACTIONPOINTBASE:
// the retail CTCUserActionPointBase over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCUserActionPointBase@@ : CTCBase) and its single-user
// derived component CTCSingleUserActionPoints (RTTI : CTCUserActionPointBase). Members are not modelled; only what the rows call, with
// the Ego_r.pdb / FableWin.pdb spellings: ?GetFreeActionPoint@CTCUserActionPointBase@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ@Z
// (out of line at 0x007FEDE0; 0x00915C84 pushes &index, &rh_set, &position, the creature) and
// ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z (0x007FEF40). FableWin.pdb ??$GetTC@VCTCSingleUserActionPoints@@@CThing@@ and
// ?StatGetInterfaceType@CTCSingleUserActionPoints@@SI?AW4ETCInterfaceType@@XZ: TCI_SINGLE_USER_ACTION_POINTS (45; retail tests bit 13 of
// the +0x24 bitset word and looks up key 0x2D). (engine/campfire_types.h carries a generated declaration list of the same class for its
// own TU; do not include both.)
#ifdef FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class C3DVector;
class CRightHandedSet;
class CTCUserActionPointBase : public CTCBase {
public:
    bool GetFreeActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& rh_set, long& index);   // 0x007FEDE0
    void SetActionPointAsFree(long index);                               // 0x007FEF40
};
class CTCSingleUserActionPoints : public CTCUserActionPointBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SINGLE_USER_ACTION_POINTS; }
};
#endif
#endif // FABLE_ENGINE_CTCUSERACTIONPOINTBASE_H
