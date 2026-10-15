// Behaviour test for 0x00A03040 (CHandleBasedMemoryPool::InternalAllocateHandle): free-chain pop, else append a null slot.
#include <cstdio>
struct Pool {
    long slots[16]; long count; long FirstUnusedHandle;
    Pool() : count(0), FirstUnusedHandle(-1) {}
    long InternalAllocateHandle()
    {
        long index = FirstUnusedHandle;
        if (index != -1) {
            long& slot = slots[index];
            FirstUnusedHandle = slot;
            slot = -1;
            return index;
        }
        long n = count;
        slots[count++] = 0;
        return n;
    }
};
int main()
{
    Pool p; int ok = 1;
    if (p.InternalAllocateHandle() != 0 || p.InternalAllocateHandle() != 1 || p.InternalAllocateHandle() != 2) ok = 0;
    if (p.count != 3 || p.slots[2] != 0) ok = 0;
    p.slots[1] = 0; p.slots[0] = 1;  p.FirstUnusedHandle = 0;   // chain 0 -> 1 -> 0? make 1 terminate
    p.slots[1] = -1;
    if (p.InternalAllocateHandle() != 0 || p.slots[0] != -1 || p.FirstUnusedHandle != 1) ok = 0;
    if (p.InternalAllocateHandle() != 1 || p.FirstUnusedHandle != -1) ok = 0;
    if (p.InternalAllocateHandle() != 3 || p.count != 4) ok = 0;
    if (!ok) { std::printf("00a03040_TEST FAIL\n"); return 1; }
    std::printf("00a03040_TEST PASS\n");
    return 0;
}