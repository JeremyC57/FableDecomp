// CTCBase::UpdateGlobalTCs(bool paused) -- run GlobalUpdate(paused) on every registered global-update TC that is not marked for removal, stepping the iterator before each call
// Leaf: ?UpdateGlobalTCs@CTCBase@@SIX_N@Z
//
// Retail 0x00686DC0, 46 bytes (+ int3 padding to 48), plain `ret` (static __fastcall: paused in cl, kept whole in edi).
// Callee: the TC's virtual GlobalUpdate(bool) (slot 26, `call [eax+68h]`; CTCInventoryExperience::GlobalUpdate
//   0x0056FC5D sits in slot 26 of its vtable 0x0124F11C).
// Data (NF96 q11449): the list is CTCBase::GlobalUpdateTCs, Ego_d.pdb PUB32
//   ?GlobalUpdateTCs@CTCBase@@1V?$list@PAVCTCBase@@V?$allocator@PAVCTCBase@@@std@@@std@@A (protected static); Ego_r.pdb 0x01010A70 / FableWin.pdb
//   0x03FE50A0 name its `dynamic initializer for 'CTCBase::GlobalUpdateTCs'` (the landed 0x0121E010); engine/CTCBase.h
//   FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS (NF95) declares it; the ported 0x00686DF0 RemoveFromGlobalTCs binds the same symbol.
// Identity: Ego_r.pdb ?UpdateGlobalTCs@CTCBase@@SIX_N@Z (static, __fastcall, bool); one caller 0x004A5CC0.
// Shared headers: engine/CTCBase.h (FABLE_RETAIL_CTCBASE: GlobalUpdate slot 26, ToBeRemoved +8, UpdateGlobalTCs) and the STLport list of
//   stlport/fable_containers.h; was a local CTCBase model with a public one-parameter local std::list (bound
//   ?GlobalUpdateTCs@CTCBase@@2V?$list@PAVCTCBase@@@std@@A).
// Shape: `for (it = list.begin(); it != list.end(); ) { CTCBase* tc = *it; ++it; if (!tc->ToBeRemoved)
//   tc->GlobalUpdate(paused); }` -- end() re-read from the global after each call.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_GLOBALUPDATETCS
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"

void __fastcall CTCBase::UpdateGlobalTCs(bool paused)
{
    for (std::list<CTCBase*>::iterator it = GlobalUpdateTCs.begin(); it != GlobalUpdateTCs.end(); ) {
        CTCBase* tc = *it;
        ++it;
        if (!tc->ToBeRemoved)
            tc->GlobalUpdate(paused);
    }
}
