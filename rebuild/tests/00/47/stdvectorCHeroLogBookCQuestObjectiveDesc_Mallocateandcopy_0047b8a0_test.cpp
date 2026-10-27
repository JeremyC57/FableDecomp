// behaviour test for 0x0047b8a0 vector<CQuestObjectiveDesc>::_M_allocate_and_copy(n, first, last): allocate n elements
// (null for 0), copy-construct [first, last) into the front, return the block
#include <cstdio>
#include <cstdlib>
#include <new>
struct Obj { unsigned int id; int text; bool done; };
static int gAllocs = 0;
static Obj* AllocateAndCopy(unsigned int n, const Obj* first, const Obj* last)
{
    Obj* result = n ? (++gAllocs, (Obj*)std::malloc(n * sizeof(Obj))) : 0;
    Obj* r = result;
    for (; first != last; ++first, ++r) new (r) Obj(*first);
    return result;
}
int main()
{
    int fails = 0;
    Obj src[2] = { { 1, 10, true }, { 2, 20, false } };
    Obj* p = AllocateAndCopy(5, src, src + 2);
    if (!p || gAllocs != 1) fails |= 1;
    if (p[0].id != 1 || p[0].text != 10 || !p[0].done || p[1].id != 2 || p[1].done) fails |= 2;
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0 || gAllocs != 1) fails |= 4;
    if (fails) { std::printf("0047b8a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047b8a0_TEST PASS\n");
    return 0;
}