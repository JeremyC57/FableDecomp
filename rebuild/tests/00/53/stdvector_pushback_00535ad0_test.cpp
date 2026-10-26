// behaviour test for 0x00535AD0 std::vector<CCountedPointer<NUISystem::CComponent> >::push_back: with spare capacity the pointer is copied to the end and its block gains a reference; when full the overflow path grows the storage, keeps the old elements and appends x
#include <cstdio>
struct Info { int RefCount; };
struct CP { void* Data; Info* PInfo; };
struct Vec { CP* Start; CP* Finish; CP* End; int Overflows; CP Store[8]; };
static void Overflow(Vec& v, const CP& x)                   // stands for _M_insert_overflow(_M_finish, x, __false_type(), 1, true)
{
    ++v.Overflows; int n = (int)(v.Finish - v.Start); v.End = v.Start + n * 2 + 1;
    *v.Finish = x; if (x.PInfo) ++x.PInfo->RefCount; ++v.Finish;
}
static void PushBack(Vec& v, const CP& x)                   // the function under test
{
    if (v.Finish != v.End) { *v.Finish = x; if (v.Finish->PInfo) ++v.Finish->PInfo->RefCount; ++v.Finish; }
    else Overflow(v, x);
}
int main()
{
    int fail = 0;
    Info i = { 1 };
    Vec v; v.Start = v.Store; v.Finish = v.Store; v.End = v.Store + 1; v.Overflows = 0;
    CP a; a.Data = &i; a.PInfo = &i;
    CP b; b.Data = 0; b.PInfo = 0;
    PushBack(v, a);
    if (v.Finish - v.Start != 1 || v.Store[0].PInfo != &i || i.RefCount != 2 || v.Overflows != 0) fail |= 1;
    PushBack(v, b);
    if (v.Finish - v.Start != 2 || v.Overflows != 1 || v.Store[1].PInfo != 0) fail |= 2;
    PushBack(v, a);
    if (v.Finish - v.Start != 3 || v.Overflows != 1 || i.RefCount != 3) fail |= 4;
    if (fail) { std::printf("00535ad0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00535ad0_TEST PASS\n");
    return 0;
}