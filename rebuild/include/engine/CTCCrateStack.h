#ifndef FABLE_ENGINE_CTCCRATESTACK_H
#define FABLE_ENGINE_CTCCRATESTACK_H
// New shared header (chunk 2029, row 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods), opt-in FABLE_RETAIL_CTCCRATESTACK: the retail
// CTCCrateStack over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCCrateStack@@ : CTCBase). Members are not modelled; only what the rows call, with
// the Ego_r.pdb / FableWin.pdb spellings: ?GetNumCrates@CTCCrateStack@@QBEJXZ (out of line at 0x007B6660), ?GetNextCrate@CTCCrateStack@@QBEPAVCThing@@XZ
// (0x007B6750) and ?RemoveNextCrate@CTCCrateStack@@QAEPAVCThing@@XZ (0x007B6790; both called by the landed FetchGoods 0x00915890).
// StatGetInterfaceType: TCI_CRATE_STACK (181; retail tests bit 21 of the +0x34 bitset word and looks up key 0xB5).
#ifdef FABLE_RETAIL_CTCCRATESTACK
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCCrateStack : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CRATE_STACK; }
    long GetNumCrates() const;                                           // 0x007B6660
    CThing* GetNextCrate() const;                                        // 0x007B6750
    CThing* RemoveNextCrate();                                           // 0x007B6790
};
#endif
#endif // FABLE_ENGINE_CTCCRATESTACK_H
