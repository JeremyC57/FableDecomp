// behaviour test for 0x00442d50 NPlayerGui::CDrawDamage scalar deleting destructor (non-virtual ??_G, `ret 4`): destroy DamageValues (+0x28, _List_base<SDamageValue>::~_List_base 0x0043FCF0),
// then the CDrawBase PrimitiveHandles vector (each handle's virtual destructor over [+0x1C,+0x20) in 8-byte steps, then free of the
// block only when it is non-null), then operator delete of the object only when bit 0 of the flags is set; returns this.
#include <cstdio>
#include <cstdlib>
#include <new>
static int gMemberDtors = 0, gHandleDtors = 0, gBlockFrees = 0, gObjDeletes = 0, gOrderBad = 0;
static void* gLastBlock = 0;
struct Handle { virtual ~Handle() { if (gMemberDtors == 0) gOrderBad = 1; ++gHandleDtors; } void* data; };
struct ListBase { ~ListBase() { if (gHandleDtors != 0 || gBlockFrees != 0) gOrderBad = 1; ++gMemberDtors; } void* p; };
static void TrackFree(void* p) { ++gBlockFrees; gLastBlock = p; std::free(p); }
struct DrawBase { void* vptr; void* fields[5]; bool Drawing; Handle* hs; Handle* hf; Handle* he;
    ~DrawBase() { for (Handle* p = hs; p != hf; ++p) p->~Handle(); if (hs) TrackFree(hs); } };
struct Draw : DrawBase { ListBase DamageValues; long tail; };
// The row: Draw::~Draw() (implicit), then operator delete when (flags & 1).
static void* ScalarDeletingDtor(Draw* d, unsigned int flags)
{
    d->~Draw();
    if (flags & 1) { ++gObjDeletes; std::free(d); }
    return d;
}
static Draw* Make(int handles, int capacity = -1)
{
    if (capacity < handles) capacity = handles;
    Draw* d = new (std::malloc(sizeof(Draw))) Draw();
    d->hs = capacity ? (Handle*)std::malloc(capacity * sizeof(Handle)) : 0;
    for (int i = 0; i < capacity; ++i) new (&d->hs[i]) Handle();   // spare slots constructed too, so a wrong bound counts, not crashes
    d->hf = d->hs + handles; d->he = d->hs + capacity;
    return d;
}
static void Reset() { gMemberDtors = gHandleDtors = gBlockFrees = gObjDeletes = 0; gLastBlock = 0; }
int main()
{
    int fails = 0;
    if (sizeof(Handle) != 8) fails |= 0x100;
    // flags 1: everything destroyed, three handles, block freed, object deleted, this returned
    Reset(); Draw* a = Make(3); Handle* blk = a->hs;
    if (ScalarDeletingDtor(a, 1) != a) fails |= 1;
    if (gMemberDtors != 1 || gHandleDtors != 3 || gBlockFrees != 1 || gLastBlock != blk || gObjDeletes != 1) fails |= 2;
    // capacity 5, size 2: only [start,finish) destroyed (the loop bound is +0x20 finish, not +0x24 end of storage)
    Reset(); Draw* g = Make(2, 5); Handle* gblk = g->hs; ScalarDeletingDtor(g, 1);
    if (gHandleDtors != 2 || gBlockFrees != 1 || gLastBlock != gblk) fails |= 0x400;
    // size 0 but storage allocated (start == finish != 0): no handle destructor, the block is still freed (test is on start)
    Reset(); Draw* h = Make(0, 2); Handle* hblk = h->hs; ScalarDeletingDtor(h, 1);
    if (gHandleDtors != 0 || gBlockFrees != 1 || gLastBlock != hblk || gObjDeletes != 1) fails |= 0x800;
    // flags 0: destroyed but not deleted
    Reset(); Draw* b = Make(1);
    if (ScalarDeletingDtor(b, 0) != b) fails |= 4;
    if (gMemberDtors != 1 || gHandleDtors != 1 || gBlockFrees != 1 || gObjDeletes != 0) fails |= 8;
    std::free(b);
    // flags 2 (bit 0 clear, other bits set): not deleted; flags 3: deleted
    Reset(); Draw* c = Make(2); ScalarDeletingDtor(c, 2);
    if (gObjDeletes != 0 || gHandleDtors != 2) fails |= 0x10;
    std::free(c);
    Reset(); Draw* e = Make(2); ScalarDeletingDtor(e, 3);
    if (gObjDeletes != 1) fails |= 0x20;
    // empty vector (null start): no handle destructor, no free of the block
    Reset(); Draw* f = Make(0); ScalarDeletingDtor(f, 1);
    if (gHandleDtors != 0 || gBlockFrees != 0 || gMemberDtors != 1 || gObjDeletes != 1) fails |= 0x40;
    if (gOrderBad) fails |= 0x80;
    if (fails) { std::printf("00442d50_TEST FAIL %d\n", fails); return 1; }
    std::printf("00442d50_TEST PASS\n");
    return 0;
}