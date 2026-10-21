// CMemAllocator::ReallyFree() -- DirectShow strmbase (amfilter.cpp): delete every sample on the free list, zero the allocated count and release the VirtualAlloc'd buffer block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA3C0, length 69 bytes.
// Leaf: ?ReallyFree@CMemAllocator
// Identity: vtables 0x012BF83C / 0x012BF810 are CMemAllocator's (the landed CMemAllocator::CreateInstance 0x00CAC9D0
// builds them), and the destructor 0x00CAA410 calls Decommit through the IMemAllocator base at +0xc and then this
// function, as strmbase's ~CMemAllocator() { Decommit(); ReallyFree(); } does. The manifest name
// CScriptDefinitionManager::DeallocateScriptEntries is a forge-autoname3 label that labels_forge_autoname3_revert.tsv
// reverts (the real ~CScriptDefinitionManager is 0x00C54110).
// Retail bytes: push esi ; mov esi,ecx ; L: mov ecx,[esi+28h] ; test ecx,ecx ; je D ; mov edx,[esi+2Ch] ; mov eax,[ecx+1Ch] ;
//               dec edx ; mov [esi+28h],eax ; mov [esi+2Ch],edx ; mov edx,[ecx] ; push 1 ; call [edx+54h] ; jmp L ;
//               D: mov eax,[esi+60h] ; test eax,eax ; mov dword ptr [esi+3Ch],0 ; je E ; push 8000h ; push 0 ; push eax ;
//               call [VirtualFree] ; mov dword ptr [esi+60h],0 ; E: pop esi ; ret
// Shape: strmbase's loop over the inline CSampleList::RemoveHead (null-safe pop that returns the old head). An open-coded
// pop in `for (;;)` or `while` gets the loop rotated (80 v 69); through RemoveHead it stays top-tested with `jmp L`.
// Model: no shared header for the strmbase classes; the layout follows the DirectX 9 SDK baseclasses (amfilter.h) and the
// landed CreateInstance. `delete pSample` is the virtual scalar deleting destructor, slot 21 (+0x54) after the 21
// IUnknown / IMediaSample / IMediaSample2 slots.
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);   // import 0x0143FD40

struct CMediaSample {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetPointer(unsigned char** buffer);
    virtual long __stdcall GetSize();
    virtual long __stdcall GetTime(__int64* start, __int64* end);
    virtual long __stdcall SetTime(__int64* start, __int64* end);
    virtual long __stdcall IsSyncPoint();
    virtual long __stdcall SetSyncPoint(int isSyncPoint);
    virtual long __stdcall IsPreroll();
    virtual long __stdcall SetPreroll(int isPreroll);
    virtual long __stdcall GetActualDataLength();
    virtual long __stdcall SetActualDataLength(long actual);
    virtual long __stdcall GetMediaType(void** mediaType);
    virtual long __stdcall SetMediaType(void* mediaType);
    virtual long __stdcall IsDiscontinuity();
    virtual long __stdcall SetDiscontinuity(int discontinuity);
    virtual long __stdcall GetMediaTime(__int64* start, __int64* end);
    virtual long __stdcall SetMediaTime(__int64* start, __int64* end);
    virtual long __stdcall GetProperties(unsigned long size, unsigned char* properties);
    virtual long __stdcall SetProperties(unsigned long size, const unsigned char* properties);
    virtual ~CMediaSample();                       // slot 21 (+0x54)
    unsigned long  m_dwFlags;                      // +0x04
    unsigned long  m_dwTypeSpecificFlags;          // +0x08
    unsigned char* m_pBuffer;                      // +0x0c
    long           m_lActual;                      // +0x10
    long           m_cbBuffer;                     // +0x14
    void*          m_pAllocator;                   // +0x18 CBaseAllocator*
    CMediaSample*  m_pNext;                        // +0x1c
};

class CSampleList {                                // CBaseAllocator::CSampleList
public:
    CMediaSample* m_List;                          // +0x00
    int           m_nOnList;                       // +0x04
    CMediaSample* RemoveHead()
    {
        CMediaSample* pSample = m_List;
        if (pSample != 0) {
            m_List = m_List->m_pNext;
            m_nOnList--;
        }
        return pSample;
    }
};

struct CMemAllocator {
    unsigned char  _bases[0x28];                   // +0x00 CUnknown, +0x0c IMemAllocatorCallbackTemp, +0x10 CCritSec
    CSampleList    m_lFree;                        // +0x28
    void*          m_hSem;                         // +0x30
    long           m_lWaiting;                     // +0x34
    long           m_lCount;                       // +0x38
    long           m_lAllocated;                   // +0x3c
    long           m_lSize;                        // +0x40
    long           m_lAlignment;                   // +0x44
    long           m_lPrefix;                      // +0x48
    int            m_bChanged;                     // +0x4c
    int            m_bCommitted;                   // +0x50
    int            m_bDecommitInProgress;          // +0x54
    void*          m_pNotify;                      // +0x58
    int            m_fEnableReleaseCallback;       // +0x5c
    unsigned char* m_pBuffer;                      // +0x60 (CMemAllocator)
    void ReallyFree();
};

void CMemAllocator::ReallyFree()
{
    CMediaSample* pSample;
    for (;;) {
        pSample = m_lFree.RemoveHead();
        if (pSample != 0) {
            delete pSample;
        } else {
            break;
        }
    }
    m_lAllocated = 0;
    if (m_pBuffer) {
        VirtualFree(m_pBuffer, 0, 0x8000);         // MEM_RELEASE
        m_pBuffer = 0;
    }
}