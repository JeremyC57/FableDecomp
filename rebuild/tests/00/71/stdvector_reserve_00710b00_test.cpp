// behaviour test for 0x00710B00 vector<_List_iterator<CAnimSet, _Const_traits<CAnimSet> > >::reserve: below the capacity nothing changes; above it a non-empty vector moves its elements into a new block of exactly n and frees the old one, an empty one just allocates n (none for 0); the size is kept
#include <cstdio>
#include <cstdlib>
struct Elem { int a; int b; };                                       // a 4-byte element (only its copy matters)
static int gFreed = 0;
struct Vec {
    Elem* start; Elem* finish; Elem* eos;
    void Reserve(unsigned int n)                                     // the function under test
    {
        if ((unsigned int)(eos - start) < n) {
            unsigned int old_size = (unsigned int)(finish - start);
            Elem* tmp;
            if (start) {
                tmp = (Elem*)std::malloc(n * sizeof(Elem));
                for (unsigned int i = 0; i < old_size; ++i) tmp[i] = start[i];
                std::free(start); ++gFreed;
            } else
                tmp = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
            start = tmp; finish = tmp + old_size; eos = tmp + n;
        }
    }
};
int main()
{
    int fail = 0;
    Vec v; v.start = v.finish = v.eos = 0;
    v.Reserve(0);
    if (v.start != 0 || gFreed != 0) fail |= 1;
    v.Reserve(2);
    if (v.start == 0 || v.finish != v.start || v.eos != v.start + 2) fail |= 2;
    v.start[0].a = 7; v.start[0].b = 8; v.finish = v.start + 1;
    Elem* old = v.start;
    v.Reserve(2);                                                    // capacity already 2: untouched
    if (v.start != old || gFreed != 0) fail |= 4;
    v.Reserve(5);
    if (v.eos - v.start != 5 || v.finish - v.start != 1 || v.start[0].a != 7 || v.start[0].b != 8 || gFreed != 1) fail |= 8;
    std::free(v.start);
    if (fail) { std::printf("00710b00_TEST FAIL %d\n", fail); return 1; }
    std::printf("00710b00_TEST PASS\n");
    return 0;
}