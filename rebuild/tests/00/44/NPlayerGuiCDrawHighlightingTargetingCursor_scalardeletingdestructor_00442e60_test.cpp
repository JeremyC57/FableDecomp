// behaviour test for 0x00442e60 NPlayerGui::CDrawHighlightingTargetingCursor scalar deleting destructor (non-virtual
// ??_G): destroy the targeted-thing pointer and the CDrawBase handles, free the object only when bit 0 of the flags is set
#include <cstdio>
#include <cstdlib>
#include <new>
static int gPtrDtors = 0, gHandleDtors = 0, gObjFrees = 0;
struct Handle { virtual ~Handle() { ++gHandleDtors; } void* data; };
struct IntelPtr { virtual ~IntelPtr() { ++gPtrDtors; } void* PData; };
struct DrawBase { void* fields[6]; Handle* hs; Handle* hf; Handle* he;
    ~DrawBase() { for (Handle* p = hs; p != hf; ++p) p->~Handle(); std::free(hs); } };
struct Cursor : DrawBase { IntelPtr PTargetedThing; unsigned long Colour; float W1, W2; long Fade, Width, Type; };
static void* ScalarDeletingDtor(Cursor* c, unsigned int flags)
{
    c->~Cursor();
    if (flags & 1) { ++gObjFrees; std::free(c); }
    return c;
}
static Cursor* Make()
{
    Cursor* c = new (std::malloc(sizeof(Cursor))) Cursor();
    c->hs = (Handle*)std::malloc(2 * sizeof(Handle)); new (&c->hs[0]) Handle(); new (&c->hs[1]) Handle(); c->hf = c->he = c->hs + 2;
    return c;
}
int main()
{
    int fails = 0;
    Cursor* c = Make();
    if (ScalarDeletingDtor(c, 1) != c) fails |= 1;
    if (gPtrDtors != 1 || gHandleDtors != 2 || gObjFrees != 1) fails |= 2;
    Cursor* d = Make();
    ScalarDeletingDtor(d, 0);
    if (gPtrDtors != 2 || gHandleDtors != 4 || gObjFrees != 1) fails |= 4;
    std::free(d);
    if (fails) { std::printf("00442e60_TEST FAIL %d\n", fails); return 1; }
    std::printf("00442e60_TEST PASS\n");
    return 0;
}