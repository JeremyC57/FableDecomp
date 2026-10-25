// behaviour test for 0x0085EA40 vector<std::vector<NThingMap::CThingMapCell> >::erase(first, last): the tail is assigned down onto first in order (deep
// copies of each element's owned block), every vacated slot past the new finish frees its block once, finish moves back
// by last - first, first is returned; erase(begin, end) frees every block
#include <cstdio>
#include <cstdlib>
#include <new>
static int gFreed = 0;
struct Elem {                                  // 12-byte element owning one heap block at +0 (like std::vector<NThingMap::CThingMapCell>)
    int* block;
    explicit Elem(int v) { block = (int*)std::malloc(sizeof(int)); *block = v; }
    ~Elem() { if (block) { std::free(block); ++gFreed; } }
    Elem& operator=(const Elem& other)
    {
        if (!block) block = (int*)std::malloc(sizeof(int));
        *block = *other.block;
        return *this;
    }
};
struct Vec { Elem* s; Elem* f; Elem* e; };
static Elem* Copy(Elem* first, Elem* last, Elem* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
static Elem* Erase(Vec& v, Elem* first, Elem* last)
{
    Elem* i = Copy(last, v.f, first);
    for (Elem* p = i; p != v.f; ++p) p->~Elem();
    v.f = i;
    return first;
}
int main()
{
    int fails = 0;
    Elem* p = (Elem*)std::malloc(sizeof(Elem) * 8);
    for (int k = 0; k < 6; ++k) new (p + k) Elem(k * 10);
    Vec v; v.s = p; v.f = p + 6; v.e = p + 8;
    Elem* r = Erase(v, p + 1, p + 3);                  // drop 10, 20
    if (r != p + 1 || v.f != p + 4) fails |= 1;
    if (*p[0].block != 0 || *p[1].block != 30 || *p[2].block != 40 || *p[3].block != 50) fails |= 2;
    if (gFreed != 2) fails |= 4;                       // the two vacated slots freed their blocks
    gFreed = 0;
    r = Erase(v, p + 2, p + 2);                        // empty range
    if (r != p + 2 || v.f != p + 4 || gFreed != 0 || *p[2].block != 40) fails |= 8;
    r = Erase(v, v.s, v.f);                            // clear()
    if (r != p || v.f != p || gFreed != 4) fails |= 16;
    std::free(p);
    if (fails) { std::printf("0085ea40_TEST FAIL %d\n", fails); return 1; }
    std::printf("0085ea40_TEST PASS\n");
    return 0;
}