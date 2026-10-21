// CHandleBasedMemoryPool::Defragment() -- full defragment: unless shut down, disabled or locked, slide every allocated area down to the next free offset (moving its resource bytes through the virtual MoveResource and re-pointing its handle), rebuild the header chain, then turn the tail into one free area and re-insert it
// Leaf: Defragment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02730, length 380 bytes (Ego_r.pdb `?Defragment@CHandleBasedMemoryPool@@QAEXXZ`).
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C) / LeaveCriticalSection (IAT 0x0143FE10), this pool's virtual
//   slot 1 MoveResource(void* resource, long handle, void* from, void* to, unsigned long size), InsertFreeArea(CHeader*)
//   @ 0x00A01DA0.
// Layout: Ego_r.pdb CHandleBasedMemoryPool / CHeader names as in the landed MoveArea 0x00A028F0 and InternalAllocate
//   0x00A030D0 (retail CArray<CHandleInfo> is 12 bytes, critical section at +0x44C; engine/CHandleBasedMemoryPool.h has
//   +0x450 and a flat vptr word, so `this` is a local polymorphic model).
// Shape: BeginCritical(false)/EndCritical inline; the walk keeps the running offset and the last moved header; an area
//   that is already in place is skipped without becoming the new predecessor; header fields are written in retail's order; the free lists are cleared with memset (a
//   for loop also gives `rep stosd` but loads edi before ecx).
#include <string.h>
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE10
struct CHeader {                                   // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    void*     Resource;                            // +0x00
    long      Handle;                              // +0x04
    long      Size;                                // +0x08
    short     Padding;                             // +0x0c
    unsigned char Status;                          // +0x0e ('N' free, 'T' taken)
    unsigned char Locked;                          // +0x0f
    CHeader** RefAvailable;                        // +0x10 (the link that points at this free area)
    CHeader*  NextAvailable;                       // +0x14
    CHeader*  PrevContiguous;                      // +0x18
    CHeader*  NextContiguous;                      // +0x1c
};
union CHandleInfo {                                // CHandleBasedMemoryPool::CHandleInfo: the area, or the next free slot
    CHeader* Header;
    long     NextFree;
};
struct CFailedAllocationHandler;
struct CHandleBasedMemoryPool {
    virtual void v00();
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 1 (+0x04)
    CHeader*                     Buffer;                     // +0x04
    void*                        UnalignedBuffer;            // +0x08
    long                         PoolSize;                   // +0x0c
    long                         AllocatedSize;              // +0x10
    long                         NumAllocations;             // +0x14
    long                         LockCount;                  // +0x18
    long                         LargestFreeArea;            // +0x1c
    unsigned long                MemoryFlags;                // +0x20
    bool                         MultiThreaded;              // +0x24
    bool                         ShutdownFlag;               // +0x25
    bool                         EnableDefrag;               // +0x26
    bool                         EnableFullDefragIfPartialFails; // +0x27
    unsigned long                MaxFreeSizeBeforeDefrag;    // +0x28
    unsigned long                MaxFreeSizeBeforeFailure;   // +0x2c
    long                         MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler*    FailureHandler;             // +0x34
    CHeader*                     FreeLists[256];             // +0x38
    CHandleInfo*                 Handles;                    // +0x438 CArray<CHandleInfo> (retail 12 bytes)
    CHandleInfo*                 HandlesLast;                // +0x43c
    CHandleInfo*                 HandlesEnd;                 // +0x440
    long                         FirstUnusedHandle;          // +0x444
    void*                        Name;                       // +0x448 CCharString
    _RTL_CRITICAL_SECTION_Retail CriticalSection;            // +0x44c

    bool BeginCritical(bool tryOnly)
    {
        if (!MultiThreaded)
            return true;
        if (tryOnly) {
            if (!TryEnterCriticalSection(&CriticalSection))
                return false;
        } else {
            EnterCriticalSection(&CriticalSection);
        }
        return true;
    }
    void EndCritical()
    {
        if (MultiThreaded)
            LeaveCriticalSection(&CriticalSection);
    }
    void ClearFreeLists() { memset(FreeLists, 0, sizeof(FreeLists)); }
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void Defragment();
};

void CHandleBasedMemoryPool::Defragment()
{
    if (ShutdownFlag || !EnableDefrag)
        return;
    BeginCritical(false);
    if (LockCount <= 0) {
        CHeader* prev = 0;
        unsigned long offset = 0;
        CHeader* area = Buffer;
        while (area) {
            CHeader* next = area->NextContiguous;
            if (!area->RefAvailable) {
                CHeader* dest = (CHeader*)((char*)Buffer + offset);
                long size = area->Size;
                unsigned char locked = area->Locked;
                if (area != dest || area->Padding != 0) {
                    long handle = area->Handle;
                    void* resource = area->Resource;
                    Handles[handle].Header = 0;
                    MoveResource(resource, handle, area + 1, dest + 1, size);
                    Handles[handle].Header = dest;
                    dest->Size = size;
                    dest->Resource = resource;
                    dest->Handle = handle;
                    dest->Padding = 0;
                    dest->Status = 'T';
                    dest->Locked = locked;
                    dest->RefAvailable = 0;
                    dest->NextAvailable = 0;
                    dest->PrevContiguous = prev;
                    dest->NextContiguous = 0;
                    if (prev)
                        prev->NextContiguous = dest;
                    prev = dest;
                }
                offset += size + sizeof(CHeader);
            }
            area = next;
        }
        AllocatedSize = offset;
        ClearFreeLists();
        unsigned long remaining = PoolSize - offset;
        if (remaining > sizeof(CHeader)) {
            CHeader* freeArea = (CHeader*)((char*)Buffer + offset);
            freeArea->Size = remaining - sizeof(CHeader);
            freeArea->Padding = 0;
            freeArea->Status = 'N';
            freeArea->Locked = 0;
            freeArea->PrevContiguous = prev;
            freeArea->NextContiguous = 0;
            freeArea->Resource = 0;
            if (prev)
                prev->NextContiguous = freeArea;
            InsertFreeArea(freeArea);
        }
    }
    EndCritical();
}