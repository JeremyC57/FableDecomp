// behaviour test for 0x00737DB0 CIVCountedPointer<CUnprovenPointee_00737db0>::`scalar deleting destructor': the pointee's count drops by one, Delete runs only when it reaches zero, the slot is cleared, a null slot is left alone, and the wrapper itself is freed only when bit 0 of the flags is set
#include <cstdio>
struct Pointee { unsigned long refs; int deleted; };
struct Wrapper { Pointee* object; };
static int freed = 0;
static Wrapper* ScalarDeletingDtor(Wrapper* self, unsigned int flags)   // the function under test
{
    Pointee* p = self->object;
    if (p) {
        if (--p->refs == 0) p->deleted++;
        self->object = 0;
    }
    if (flags & 1) freed++;
    return self;
}
int main()
{
    int fail = 0;
    Pointee a; a.refs = 2; a.deleted = 0;
    Wrapper w; w.object = &a;
    if (ScalarDeletingDtor(&w, 0) != &w) fail |= 1;
    if (a.refs != 1 || a.deleted != 0 || w.object != 0 || freed != 0) fail |= 2;
    w.object = &a;
    ScalarDeletingDtor(&w, 1);
    if (a.refs != 0 || a.deleted != 1 || w.object != 0 || freed != 1) fail |= 4;
    w.object = 0;
    ScalarDeletingDtor(&w, 3);
    if (a.deleted != 1 || freed != 2) fail |= 8;
    if (fail) { std::printf("00737db0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00737db0_TEST PASS\n");
    return 0;
}