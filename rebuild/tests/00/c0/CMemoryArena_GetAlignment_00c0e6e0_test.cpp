// behaviour test for 0x00C0E6E0
#include <stdio.h>
class IAllocator {
public:
    virtual ~IAllocator() {}
    virtual void* Allocate(unsigned long) { return 0; }
    virtual unsigned long GetAlignment() const { return 16; }
};
class CMemoryArena {
public:
    void*            _vptr_pad;                          // +0x00
    IAllocator*      Allocator;                          // +0x04
    unsigned long GetAlignment() const;
};
unsigned long CMemoryArena::GetAlignment() const
{
    if (!Allocator)
        return 1;
    return Allocator->GetAlignment();
}
int main()
{
    IAllocator al; CMemoryArena m; m.Allocator = 0; int ok = 1;
    ok &= m.GetAlignment() == 1;
    m.Allocator = &al; ok &= m.GetAlignment() == 16;
    printf(ok ? "00c0e6e0_TEST PASS\n" : "00c0e6e0_TEST FAIL\n");
    return ok ? 0 : 1;
}