// behaviour test for 0x0047b7b0 std::_Vector_base<CQuestObjectiveDesc>(n, a): allocate n 12-byte elements (nothing for
// n == 0), start = finish = block, end of storage = start + n
#include <cstdio>
#include <cstdlib>
struct Obj { unsigned int id; void* text; bool done; };
static int gAllocs = 0; static unsigned int gBytes = 0;
struct VBase { Obj* start; Obj* finish; Obj* eos;
    VBase(unsigned int n) : start(0), finish(0), eos(0)
    { start = n ? (++gAllocs, gBytes = n * 12, (Obj*)std::malloc(n * sizeof(Obj))) : 0; finish = start; eos = start + n; } };
int main()
{
    int fails = 0;
    VBase v(4);
    if (gAllocs != 1 || gBytes != 48 || !v.start || v.finish != v.start || v.eos != v.start + 4) fails |= 1;
    std::free(v.start);
    VBase e(0);
    if (gAllocs != 1 || e.start || e.finish || e.eos) fails |= 2;
    if (fails) { std::printf("0047b7b0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0047b7b0_TEST PASS\n");
    return 0;
}