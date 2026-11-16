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
#elif defined(FABLE_RETAIL_CASYNCLOADINGMEMORYPOOL_CTOR)
// Opt-in (2026-11-16, row 0x00A148D0 CAsyncLoadingMemoryPool::CAsyncLoadingMemoryPool): the class with its RTTI base
// (.?AVCAsyncLoadingMemoryPool@@ : CHandleBasedMemoryPool : CBaseClassNonCopyable : CBaseClass, rtti_bases.tsv) for the default
// constructor (Ego_r.pdb ??0CAsyncLoadingMemoryPool@@QAE@XZ; retail calls ??0CHandleBasedMemoryPool@@QAE@XZ 0x00A02D20, then stores
// vtable 0x0129CA6C). Needs FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL (the polymorphic pool model). Declarations only.
#include "rebuild_abi.h"
#include "engine/CHandleBasedMemoryPool.h"
class CAsyncLoadingMemoryPool : public CHandleBasedMemoryPool {
public:
    CAsyncLoadingMemoryPool();                                           // 0x00A148D0
    virtual ~CAsyncLoadingMemoryPool();
    static void FABLE_FASTCALL ProcessLoading();                         // 0x00A14900
};
#endif
#endif // FABLE_ENGINE_CASYNCLOADINGMEMORYPOOL_H
