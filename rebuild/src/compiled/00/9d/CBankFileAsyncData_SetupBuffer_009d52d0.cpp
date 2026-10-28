// CBankFileAsyncData::SetupBuffer(bool) -- take the bank lock and acquire the read buffer from the movable pool or fallback allocator
// Retail: Fable.exe 0x009D52D0, 310 bytes. Callees include CCriticalSectionLock @ 0x00A62920 / 0x00A629A0, CBankFile::Align @ 0x009CBB90, CMovableResourceMemoryPool::AllocateResource @ 0x00A62E40, CHandleBasedMemoryPool::GetPointerFromHandle @ 0x00A026B0, and CMemoryAllocator::InternalAllocate @ 0x009F2A40.
// The input-stream object has no generated header; its +0x00 primary view, +0x04 CMovableResource secondary base, +0x10 length, +0x14 skipped bytes, +0x1c pool, +0x20 handle, +0x24 internal allocation size, and +0x28 bank follow landed CBankFileAsyncData siblings. Shared engine headers describe the actual pool, movable resource, CBankFile, and lock layouts.
#include "engine/CBankFile.h"
#define CCriticalSectionLock CCriticalSectionLockPdb
#include "engine/CCriticalSectionLock.h"
#undef CCriticalSectionLock
#include "engine/CHandleBasedMemoryPool.h"
#include "engine/CMovableResource.h"
#include "engine/CMovableResourceMemoryPool.h"
struct _RTL_CRITICAL_SECTION { unsigned char bytes[0x18]; };
struct CCriticalSectionLock : CCriticalSectionLockPdb {
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly); // 0x00A62920 ??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z
    ~CCriticalSectionLock(); // 0x00A629A0
};
struct CBankFileAlign : CBankFile {
    unsigned long Align(unsigned long size); // 0x009CBB90
};
struct CMovableResourcePoolCalls : CMovableResourceMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long flags); // 0x00A62E40
    void* GetPointerFromHandle(long handle); // inherited CHandleBasedMemoryPool method @ 0x00A026B0
};
struct CMemoryAllocatorCalls {
    void* InternalAllocate(long size, unsigned long alignment, bool flag); // 0x009F2A40
};
extern CMemoryAllocatorCalls* g_pGlobalAllocator; // 0x013CAA90
extern unsigned long g_GlobalMemoryFlags; // 0x013CA868
void __cdecl GFCreateGlobalAllocator(); // 0x009F2870
void __cdecl GFEnterAllocatorLock(); // 0x009F2000
void __cdecl GFLeaveAllocatorLock(); // 0x009F2010
struct CBankFileAsyncRetail {
    unsigned char _pad[0x14c];
    _RTL_CRITICAL_SECTION CriticalSection; // +0x14c, retail async-bank view
    unsigned long Align(unsigned long size); // 0x009CBB90, inherited CBankFile method
};
class CBankFileAsyncData { public:
    void* __primaryVftable; // +0, primary object view evidenced by native constructor
    CMovableResource MovableResource; // +4, secondary base passed to AllocateResource
    void* Data; // +8
    void* Buffer; // +0xc
    unsigned long Length; // +0x10
    unsigned long SkipBytes; // +0x14
    unsigned long ThreadedFileUID; // +0x18
    CMovableResourcePoolCalls* MemoryPool; // +0x1c, the relocatable pool is also the handle pool
    long MemoryHandle; // +0x20
    unsigned long InternalAllocatedBufferSize; // +0x24, native fallback allocation size
    CBankFileAsyncRetail* BankFile; // +0x28
    unsigned char LoadDesc[0x18];
    bool Finished; // +0x44
    bool BufferLocked; // +0x45
    bool AllowRestart; // +0x46
    bool Aborted; // +0x47
    bool SetupBuffer(bool allocate);
};
bool CBankFileAsyncData::SetupBuffer(bool allocate)
{
    CCriticalSectionLock lock(BankFile->CriticalSection, !allocate);
    if (!lock.Locked)
        return false;
    if (Buffer)
        return true;
    if (MemoryPool) {
        unsigned long size = BankFile->Align(Length + SkipBytes);
        unsigned long flags = g_GlobalMemoryFlags;
        if (!allocate)
            flags |= 3;
        if (MemoryPool->AllocateResource(&MemoryHandle, &MovableResource, size, flags)) {
            Buffer = MemoryPool->GetPointerFromHandle(MemoryHandle);
            Data = (char*)Buffer + SkipBytes;
            return true;
        }
        if (!allocate)
            return false;
    }
    unsigned long fallbackSize = BankFile->Align(Length + SkipBytes);
    InternalAllocatedBufferSize = fallbackSize;
    if (!g_pGlobalAllocator)
        GFCreateGlobalAllocator();
    GFEnterAllocatorLock();
    void* buffer = g_pGlobalAllocator->InternalAllocate(fallbackSize, 0, false);
    GFLeaveAllocatorLock();
    Data = (char*)buffer + SkipBytes;
    Buffer = buffer;
    return true;
}