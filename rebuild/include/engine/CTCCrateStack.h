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
#ifdef FABLE_RETAIL_CTCCRATESTACK_FILLSTACK
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#endif
class CThing;
class CTCCrateStack : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CRATE_STACK; }
#ifdef FABLE_RETAIL_WAREHOUSE_MOVE_CRATES_PASS29
    void AddCrate(CThing*);
    long GetNumCrates() const;
#else
    long GetNumCrates() const;
#endif                                           // 0x007B6660
    CThing* GetNextCrate() const;                                        // 0x007B6750
    CThing* RemoveNextCrate();                                           // 0x007B6790
#ifdef FABLE_RETAIL_CTCCRATESTACK_FILLSTACK
    // e05 opt-in (egod lane, row 0x007B6810): Ego_r.pdb ?FillStack@CTCCrateStack@@QAEXJABVC3DVector@@0@Z (public; Ego_d parameter names
    // height / marker_pos / marker_dir) and the three Ego_d / Ego_r data members (FirstCrate +0x0C, SecondCrate +0x14, CrateDefIndex
    // +0x1C; private; retail FillStack reads +0x1C as the def index and stores the crates at +0x0C / +0x14). Needs engine/CIntelligentPointer.h
    // (FABLE_RETAIL_CINTELLIGENTPOINTER) and engine/C3DVector.h. Retail sizeof 0x20.
    void FillStack(long height, const C3DVector& marker_pos, const C3DVector& marker_dir);   // 0x007B6810
private:
    CIntelligentPointer<CThing> FirstCrate;                              // +0x0C
    CIntelligentPointer<CThing> SecondCrate;                             // +0x14
    long CrateDefIndex;                                                  // +0x1C
#endif
};
#endif
#endif // FABLE_ENGINE_CTCCRATESTACK_H
