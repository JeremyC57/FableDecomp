// behaviour test for 0x0084B150 NEntityEvents::CEventTalkedToAnyone::`scalar deleting destructor': the member is destroyed once, then the base part; the storage is freed only when bit 0 of the flags is set; this is returned
#include <cstdio>
#include <cstdlib>
#include <new>
static int gMember = 0, gBase = 0, gFreed = 0;
struct Member { ~Member() { ++gMember; } };
struct Base { int word; ~Base() { if (gMember != gBase + 1) gBase = -100; ++gBase; } };
struct Derived : Base { Member member; };
static void* ScalarDeletingDestructor(Derived* p, unsigned int flags)   // the function under test
{
    p->~Derived();
    if (flags & 1) { ++gFreed; std::free(p); }
    return p;
}
int main()
{
    int fail = 0;
    void* mem = std::malloc(sizeof(Derived));
    Derived* d = new (mem) Derived;
    if (ScalarDeletingDestructor(d, 1) != d) fail |= 1;
    if (gMember != 1 || gBase != 1 || gFreed != 1) fail |= 2;
    Derived local;
    ScalarDeletingDestructor(&local, 0);
    if (gMember != 2 || gBase != 2 || gFreed != 1) fail |= 4;
    new (&local) Derived;                                    // re-construct so the automatic destructor stays balanced
    if (fail) { std::printf("0084b150_TEST FAIL %d\n", fail); return 1; }
    std::printf("0084b150_TEST PASS\n");
    return 0;
}