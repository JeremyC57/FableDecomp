// DestroyNamedBufferOwner_0085b090(CNamedBufferOwner_0085b090* object) -- `delete object` with the non-virtual destructor inlined: free the raw buffer at +0x1c, destroy the intelligent pointer at +0x14 and the CCharString at +4, then operator delete
// Leaf: DestroyNamedBufferOwner_0085b090
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085B090, length 50 bytes (the oracle row carries 14 bytes of int3 padding).
// Callees: free @ 0x00BFEA14, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (on +0x14), CCharString::~CCharString @ 0x0099EAE0 (on +4),
// operator delete @ 0x00BFE9BC. Shape: the object arrives in ecx (`__fastcall` free helper) and is null-tested first (the delete-expression's
// test), the body statement `if (m_pBuffer) free(m_pBuffer)` runs before the member destructors in reverse declaration order, and the
// storage is released with `push esi ; call operator delete ; add esp,4`. No vtable store: the class is not polymorphic. The manifest label
// "CEngineWaterRenderer::DestroyWaterGenerator" is propagated (the water renderer has no such members); names are descriptive.
#include <stdlib.h>
#include "fable_string.h"

struct CBaseIntelligentPointer_0085b090 {          // 8 bytes: vptr + the shared link block
    virtual void v00();
    void* m_pLink;
    ~CBaseIntelligentPointer_0085b090();           // 0x00A01C10
};

struct CNamedBufferOwner_0085b090 {
    long                             m_0;          // +0x00
    CCharString                      m_name;       // +0x04
    long                             m_8;          // +0x08
    long                             m_c;          // +0x0c
    long                             m_10;         // +0x10
    CBaseIntelligentPointer_0085b090 m_pTarget;    // +0x14
    void*                            m_pBuffer;    // +0x1c
    ~CNamedBufferOwner_0085b090() { if (m_pBuffer) free(m_pBuffer); }
};

void __fastcall DestroyNamedBufferOwner_0085b090(CNamedBufferOwner_0085b090* object)
{
    delete object;
}