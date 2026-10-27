// Test for 0x00A01D40 CHandleBasedMemoryPool::GetHandleFromPointer: the handle lives in the 0x20-byte header just before the payload.
#include <stdio.h>
class CHandleBasedMemoryPool {
public:
    class CHeader {
    public:
        void* Resource; int Handle; int Size; short Padding; unsigned char Status; unsigned char Locked;
        CHeader** RefAvailable; CHeader* NextAvailable; CHeader* PrevContiguous; CHeader* NextContiguous;
    };
    int GetHandleFromPointer(void* p) { return (static_cast<CHeader*>(p) - 1)->Handle; }
};
int main()
{
    CHandleBasedMemoryPool::CHeader blocks[3];
    blocks[0].Handle = 11; blocks[1].Handle = 22; blocks[2].Handle = 33;
    CHandleBasedMemoryPool pool;
    bool ok = pool.GetHandleFromPointer(&blocks[1]) == 11 && pool.GetHandleFromPointer(&blocks[2]) == 22;
    puts(ok ? "00a01d40_TEST PASS" : "00a01d40_TEST FAIL");
    return ok ? 0 : 1;
}