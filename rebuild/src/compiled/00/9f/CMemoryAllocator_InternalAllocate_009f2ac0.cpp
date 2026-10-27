// CMemoryAllocator::InternalAllocate(long size, unsigned long type, const char* file, long line, const char* function) -- tracked allocation: InternalAllocate(size, type, true), then restamp the block's debug record {-1, CurrentTrackingID, size}
// Leaf: ?InternalAllocate@CMemoryAllocator@@IAEPAXJKPBDJ0@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2AC0, length 91 bytes, thiscall `ret 14h`, /GS (cookie 0x0139C8A8 in
//   the `push ecx` word, __security_check_cookie 0x00BFE9F9).
// Identity: Ego_r.pdb ?InternalAllocate@CMemoryAllocator@@IAEPAXJKPBDJ0@Z (the five-argument overload; the three-argument
//   ?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z is 0x009F2A40, landed). Single caller 0x009F2BC6 in the static
//   __fastcall CMemoryAllocator::Allocate(size, type, file, line, function) 0x009F2B90 (Ego_r ?Allocate@CMemoryAllocator@@SIPAXKKPBDK0@Z),
//   which forwards its five arguments to the singleton under the critical section 0x013CAA70.
// Callees: InternalAllocate(long, unsigned long, bool) 0x009F2A40, GetAllocationSize(void*) 0x009F2390 (called twice, the
//   first result discarded, as in the landed InitDebugInfo 0x009F28D0).
// Layout (Ego_r.pdb CMemoryAllocator): CurrentTrackingID byte +0x414; CDebugRecord {Checksum, TrackingID,
//   OriginalAllocationSize, StackTrackingID} at block + GetAllocationSize(block) - 0x14 (the 0x14-byte trailer).
// Shape: the file/line/function tracking record of the debug build compiles out in retail; its dead filename buffer
//   (CTrackingDebugRecord::Filename is char[48]) still gives the function its /GS cookie.
class CMemoryAllocator {
public:
    class CDebugRecord {
    public:
        unsigned long Checksum;                         // +0x00
        unsigned long TrackingID;                       // +0x04
        unsigned long OriginalAllocationSize;           // +0x08
        unsigned long StackTrackingID;                  // +0x0C
    };
    unsigned long MemoryAllocatedPerTrackingID[256];    // +0x000
    unsigned long MemoryAllocated;                      // +0x400
    void**        MemoryAuditBuffer;                    // +0x404
    void*         Heap;                                 // +0x408
    unsigned long NoAllocations;                        // +0x40C
    unsigned long NoAllocators;                         // +0x410
    unsigned char CurrentTrackingID;                    // +0x414
    unsigned long GetAllocationSize(void* p);           // 0x009F2390
protected:
    void* InternalAllocate(long size, unsigned long type, bool initDebugInfo);   // 0x009F2A40
    void* InternalAllocate(long size, unsigned long type, const char* file, long line, const char* function);
};

void* CMemoryAllocator::InternalAllocate(long size, unsigned long type, const char* file, long line, const char* function)
{
    char filename[48];
    void* p = InternalAllocate(size, type, true);
    GetAllocationSize(p);
    CDebugRecord* record = (CDebugRecord*)((char*)p + GetAllocationSize(p) - 0x14);
    record->Checksum = 0xFFFFFFFF;
    record->TrackingID = CurrentTrackingID;
    record->OriginalAllocationSize = size;
    return p;
}