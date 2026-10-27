// Test for 0x009F2AC0 CMemoryAllocator::InternalAllocate(size, type, file, line, function): allocates through the
// three-argument InternalAllocate(size, type, true) and restamps the trailer record 0x14 bytes before the end of the
// block's allocation with {-1, CurrentTrackingID, size}; returns the block.
#include <stdio.h>
#include <string.h>
struct CDebugRecord { unsigned int Checksum; unsigned int TrackingID; unsigned int OriginalAllocationSize; unsigned int StackTrackingID; };
static unsigned char g_heap[256];
static int g_inner = 0, g_innerFlag = -1, g_sizeCalls = 0;
class CMemoryAllocator {
public:
    unsigned char CurrentTrackingID;
    unsigned int GetAllocationSize(void* p) { ++g_sizeCalls; return 64; }
    void* InternalAllocate(int size, unsigned int type, bool init) { ++g_inner; g_innerFlag = init ? 1 : 0; return g_heap + 16; }
    void* InternalAllocate(int size, unsigned int type, const char* file, int line, const char* function)
    {
        void* p = InternalAllocate(size, type, true);
        GetAllocationSize(p);
        CDebugRecord* r = (CDebugRecord*)((char*)p + GetAllocationSize(p) - 0x14);
        r->Checksum = 0xFFFFFFFFu; r->TrackingID = CurrentTrackingID; r->OriginalAllocationSize = size;
        return p;
    }
};
int main()
{
    memset(g_heap, 0, sizeof(g_heap));
    CMemoryAllocator m; m.CurrentTrackingID = 6;
    void* p = m.InternalAllocate(40, 0, "x.cpp", 12, "F");
    CDebugRecord* r = (CDebugRecord*)(g_heap + 16 + 64 - 0x14);
    bool ok = p == g_heap + 16 && g_inner == 1 && g_innerFlag == 1 && g_sizeCalls == 2 &&
              r->Checksum == 0xFFFFFFFFu && r->TrackingID == 6 && r->OriginalAllocationSize == 40 && r->StackTrackingID == 0;
    puts(ok ? "009f2ac0_TEST PASS" : "009f2ac0_TEST FAIL");
    return ok ? 0 : 1;
}