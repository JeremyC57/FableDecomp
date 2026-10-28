// CFileInstaller::FreeBuffers() -- under the installer's critical section: release the buffer-chunk array, then GlobalFree and clear the read and write buffers
// Leaf: FreeBuffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C5110, length 85 bytes (+ int3 padding).
// Callees: CCriticalSectionLock::CCriticalSectionLock(RTL_CRITICAL_SECTION*, bool) @ 0x00A62920 and its destructor
// @ 0x00A629A0, CArray<CFileInstaller::CBufferChunk>::DeallocateMemory @ 0x009C76D0 (labelled for the CSymbolMap pair
// array, propagated), GlobalFree through the import slot 0x0143FDF8 (twice, the slot loaded once into edi).
// Evidence: called first by CFileInstaller::InitBuffers @ 0x009C5310 and by the CFileInstaller destructor body
// @ 0x009C5170 (vtable 0x0129B43C = CFileInstaller); the catalog label "~CPatchTesselationEdgeStrip" is propagated and
// the real name is not known (named for what it does).
// Layout: Ego_r.pdb CFileInstaller with the retail 12-byte CArray: CriticalSection +0x04, BufferChunks +0x1c,
// PReadBuffer +0x28, PWriteBuffer +0x2c. No engine header.
// Shape: an 8-byte stack lock (no EH frame: the module is built without exceptions), `push 0 ; push &cs`; the
// dllimport GlobalFree used twice is loaded into edi once (between the first test and its branch); each buffer is
// cleared inside its own `if`; the lock destructor runs last; `ret`.
#define CCriticalSectionLock CCriticalSectionLockPdb
#include "engine/CCriticalSectionLock.h"
#undef CCriticalSectionLock

struct _RTL_CRITICAL_SECTION {                     // Win32 RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};

extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);   // import slot 0x0143FDF8

// The stack lock: engine/CCriticalSectionLock.h (CriticalSection +0, Locked +4, 8 bytes) with its out-of-line
// constructor and destructor.
struct CCriticalSectionLock : CCriticalSectionLockPdb {
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920 ??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z
    ~CCriticalSectionLock();                                              // 0x00A629A0
};

struct CBufferChunkArray {                         // retail CArray<CFileInstaller::CBufferChunk>: first / last / end
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void DeallocateMemory();                       // 0x009C76D0
};

class CFileInstaller { public:                            // vtable 0x0129B43C
    virtual void v00();
    _RTL_CRITICAL_SECTION      CriticalSection;    // +0x04
    CBufferChunkArray          BufferChunks;       // +0x1c
    unsigned char*             PReadBuffer;        // +0x28
    unsigned char*             PWriteBuffer;       // +0x2c
    void FreeBuffers();
};

void CFileInstaller::FreeBuffers()
{
    CCriticalSectionLock lock(CriticalSection, false);
    BufferChunks.DeallocateMemory();
    if (PReadBuffer) {
        GlobalFree(PReadBuffer);
        PReadBuffer = 0;
    }
    if (PWriteBuffer) {
        GlobalFree(PWriteBuffer);
        PWriteBuffer = 0;
    }
}