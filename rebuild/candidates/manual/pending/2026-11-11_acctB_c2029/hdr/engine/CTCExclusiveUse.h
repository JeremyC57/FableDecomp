#ifndef FABLE_ENGINE_CTCEXCLUSIVEUSE_H
#define FABLE_ENGINE_CTCEXCLUSIVEUSE_H
// New shared header (chunk 2029, row 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods), opt-in FABLE_RETAIL_CTCEXCLUSIVEUSE: the retail
// CTCExclusiveUse over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCExclusiveUse@@ : CTCBase). Ego_r.pdb TPI (struct_layouts_egor.tsv): sizeof 20,
// User (CIntelligentPointer<CThing>) +0x0C right after the 0x0C-byte CTCBase -- retail reads it at +0x0C (0x00915F57 `lea ebx,[esi+0Ch]`,
// the landed SetUser 0x00916640 `lea ecx,[esi+0Ch]`). Methods (Ego_r.pdb / FableWin.pdb spellings): ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z
// (out of line at 0x00916640, landed; Ego_r donor 0x0080C470) and ?GetUser@CTCExclusiveUse@@QBEPAVCThing@@XZ (FableWin.pdb only, the
// debug copy: inline in the release build, the caller calls GetPItem on +0x0C in place). StatGetInterfaceType: TCI_EXCLUSIVE_USE (47;
// retail tests bit 15 of the +0x24 bitset word and looks up key 0x2F). Members after User are not modelled (none exist in Ego_r).
#ifdef FABLE_RETAIL_CTCEXCLUSIVEUSE
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include <stddef.h>
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CTCExclusiveUse : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_EXCLUSIVE_USE; }
    CThing* GetUser() const { return User.Get(); }
    void SetUser(CThing* user);                                          // 0x00916640
private:
    CIntelligentPointer<CThing> User;                                    // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CTCExclusiveUse) == 0x14);
#endif
#endif // FABLE_ENGINE_CTCEXCLUSIVEUSE_H
