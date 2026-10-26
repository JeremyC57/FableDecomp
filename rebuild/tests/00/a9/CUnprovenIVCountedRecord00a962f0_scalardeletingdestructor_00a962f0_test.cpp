// behaviour test for 0x00A962F0 CUnprovenIVCountedRecord_00a962f0::`scalar deleting destructor': the pointee's count at +0x2C drops by
// one, Delete runs only when it reaches zero, the slot is cleared, a null slot is left alone, the head words are untouched, and the
// record is freed only when bit 0 of the flags is set
#include <cstdio>
struct Pointee { unsigned int refs; int deleted; };
struct Record { unsigned int head[11]; Pointee* object; };
static int freed = 0;
static Record* ScalarDeletingDtor(Record* self, unsigned int flags)
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
    Record r; for (int i = 0; i < 11; ++i) r.head[i] = 7u + i; r.object = &a;
    if (ScalarDeletingDtor(&r, 0) != &r) fail |= 1;
    if (a.refs != 1 || a.deleted != 0 || r.object != 0 || freed != 0) fail |= 2;
    r.object = &a;
    ScalarDeletingDtor(&r, 1);
    if (a.refs != 0 || a.deleted != 1 || r.object != 0 || freed != 1) fail |= 4;
    ScalarDeletingDtor(&r, 3);
    if (a.deleted != 1 || freed != 2) fail |= 8;
    for (int i = 0; i < 11; ++i) if (r.head[i] != 7u + i) fail |= 16;
    if (fail) { std::printf("00a962f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a962f0_TEST PASS\n");
    return 0;
}