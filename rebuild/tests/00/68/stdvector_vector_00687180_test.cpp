// behaviour test for 0x00687180 std::vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> >::~vector(): every element's two destructible members are destroyed (the
// later member first, elements in order), then the block is freed only when there is one
#include <cstdio>
#include <cstdlib>
static int gLog[16];
static int gLogN = 0;
static int gFreed = 0;
struct Elem { int first; int second; };
struct Vec { Elem* start; Elem* finish; Elem* eos; };
static void Destroy(Vec* v)                                  // the function under test
{
    for (Elem* p = v->start; p != v->finish; ++p) {
        if (gLogN < 16) gLog[gLogN++] = p->second;
        if (gLogN < 16) gLog[gLogN++] = p->first;
    }
    if (v->start != 0) { std::free(v->start); ++gFreed; }
}
int main()
{
    int fail = 0;
    Vec empty; empty.start = empty.finish = empty.eos = 0;
    Destroy(&empty);
    if (gFreed != 0 || gLogN != 0) fail |= 1;
    Vec v; v.start = (Elem*)std::malloc(3 * sizeof(Elem)); v.finish = v.start + 2; v.eos = v.start + 3;
    v.start[0].first = 1; v.start[0].second = 2; v.start[1].first = 3; v.start[1].second = 4;
    Destroy(&v);
    if (gFreed != 1 || gLogN != 4) fail |= 2;
    if (gLog[0] != 2 || gLog[1] != 1 || gLog[2] != 4 || gLog[3] != 3) fail |= 4;
    if (sizeof(Vec) != (unsigned int)(3 * sizeof(void*))) fail |= 8;
    if (fail) { std::printf("00687180_TEST FAIL %d\n", fail); return 1; }
    std::printf("00687180_TEST PASS\n");
    return 0;
}