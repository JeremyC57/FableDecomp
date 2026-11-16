// CRenderQueue_00c279a0::Flush() -- under the profile lock at 0x0143BAFC, clear the pending count and, unless the queue is suspended, play its source handle with CODA_SourcePlay @ 0x00C35DD0 (not looping)
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C279A0, length 59 bytes.
// Callees: PKCProtectionInstance::PKCProtectionInstance(void*) @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (the 8-byte scoped lock, landed shape), the `__stdcall`
// submit @ 0x00C35DD0 (buffer at +0xc, flag 0; manifest "CDrawAutoPickups_ShouldRender" is propagated). Shape: the flag byte at +0x24 is tested
// before the zero store at +0x10 is emitted (the scheduler sinks the store next to the branch). The class is named by address (the chunk label
// "CDraw2DText::ClearPrimitivesIfNecessary" is propagated).
#include "libpk/PKCProtectionInstance.h"

extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
extern "C" long __stdcall CODA_SourcePlay(void* source, long looping);  // 0x00C35DD0 (landed self row _CODA_SourcePlay@8)

struct CRenderQueue_00c279a0 {
    unsigned char _pad[0x0c];
    void*         m_pBuffer;                       // +0x0c
    long          m_pendingCount;                  // +0x10
    unsigned char _pad14[0x10];
    unsigned char m_suspended;                     // +0x24
    void Flush();
};

void CRenderQueue_00c279a0::Flush()
{
    PKCProtectionInstance lock(&g_soundLock);
    m_pendingCount = 0;
    if (!m_suspended)
        CODA_SourcePlay(m_pBuffer, 0);
}