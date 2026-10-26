// behaviour test for 0x00B496E0 CDynamicObjectHeap<72, 16>::`scalar deleting destructor': the class's own clean-up runs first, then the base destructor, then the object is freed only when bit 0 of the flags is set, and this is returned
#include <cstdio>
static int gStep = 0, gCleanAt = -1, gBaseAt = -1, gFreeAt = -1;
struct Object { int dummy; };
static Object* ScalarDeletingDtor(Object* self, unsigned int flags)   // the function under test
{
    gCleanAt = gStep++;                  // Release()
    gBaseAt = gStep++;                   // ~CDynamicObjectHeapBase
    if (flags & 1) gFreeAt = gStep++;    // operator delete
    return self;
}
int main()
{
    int fail = 0;
    Object o;
    if (ScalarDeletingDtor(&o, 0) != &o) fail |= 1;
    if (gCleanAt != 0 || gBaseAt != 1 || gFreeAt != -1) fail |= 2;
    gStep = 0;
    ScalarDeletingDtor(&o, 1);
    if (gCleanAt != 0 || gBaseAt != 1 || gFreeAt != 2) fail |= 4;
    if (fail) { std::printf("00b496e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b496e0_TEST PASS\n");
    return 0;
}