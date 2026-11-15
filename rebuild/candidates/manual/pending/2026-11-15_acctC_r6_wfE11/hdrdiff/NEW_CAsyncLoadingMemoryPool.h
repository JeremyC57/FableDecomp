#ifndef FABLE_ENGINE_CASYNCLOADINGMEMORYPOOL_H
#define FABLE_ENGINE_CASYNCLOADINGMEMORYPOOL_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CASYNCLOADINGMEMORYPOOL.
// Ego_r.pdb ?ProcessLoading@CAsyncLoadingMemoryPool@@SIXXZ (static __fastcall; retail 0x00A14900, called with no arguments at 0x004A4CEC).
// Members are not modelled (no sizeof assertion).
#ifdef FABLE_RETAIL_CASYNCLOADINGMEMORYPOOL
#include "rebuild_abi.h"
class CAsyncLoadingMemoryPool {
public:
    static void FABLE_FASTCALL ProcessLoading();                         // 0x00A14900
};
#endif
#endif // FABLE_ENGINE_CASYNCLOADINGMEMORYPOOL_H
