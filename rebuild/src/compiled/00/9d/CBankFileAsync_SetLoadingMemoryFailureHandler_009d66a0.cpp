// CBankFileAsync::SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler) -- detach this bank from the old memory-failure handler, attach it to the new one and keep the new handler
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D66A0, 136 bytes (+8 int3), __fastcall, ret 8 (the counted pointer by value).
// Callees: CLoadingMemoryFailureHandler member 0x009D5A30 (remove bank; labelled CBankFile::PopLoadErrorQueue) and
// 0x009D6420 (add bank; labelled vector_push_back_4byte_list), both taking the bank; the count block's
// destroy function (`call [blk+4]` with the object in ecx: __fastcall), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CBankFileAsync::MemoryFailureHandler at its retail offset +0x120 (PDB +0x184; the retail
// CBankFile base is smaller): CCountedPointer = {object, count block}, block = {count, destroy, object}.
// Shape: the old handler (if any) forgets the bank, the new one (if any) learns it; then the inline counted-
// pointer assignment (same-block test, release with the member re-read after the decrement, as the landed
// CCountedPointer::operator= 0x009A9BF0 model, then the two stores and the add-ref) and the by-value
// parameter's inline destructor (its block kept in edi).
void operator delete(void* p);                      // 0x00BFE9BC

#include "engine/CCountedPointer.h"


class CBankFileAsync;

class CLoadingMemoryFailureHandler {
public:
    void RemoveBank(CBankFileAsync* bank);          // 0x009D5A30
    void AddBank(CBankFileAsync* bank);             // 0x009D6420
};

class CBankFileAsync {
public:
    void* __vftable;                                // +0x000
    unsigned char _body[0x11c];                     // +0x004 .. +0x11F (CBankFile, ThreadedFile, LoadingMemoryPool)
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;   // +0x120
    void SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler);
};

void CBankFileAsync::SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler)
{
    if (MemoryFailureHandler.Data)
        MemoryFailureHandler->RemoveBank(this);
    if (handler.Data)
        handler->AddBank(this);
    MemoryFailureHandler = handler;
}