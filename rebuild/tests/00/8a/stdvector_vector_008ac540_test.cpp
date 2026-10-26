// behaviour test for 0x008AC540 vector<CIntelligentPointer<CThingAICreature> >::vector(const vector& x): an empty source gives three null words and no
// allocation; otherwise one block of x.size() elements, finish = start + size = end of storage, and every element
// copy-constructed in order from the source (the out-of-line uninitialized copy)
#include <cstdio>
#include <cstdlib>
#include <new>
static int gAllocs = 0, gCopies = 0;
struct Elem {                                  // a non-POD 8-byte stand-in for CIntelligentPointer<CThingAICreature>
    int v;
    Elem(int x) : v(x) { }
    Elem(const Elem& o) : v(o.v) { ++gCopies; }
};
struct Vec { Elem* s; Elem* f; Elem* e; };
static Elem* UninitCopy(const Elem* first, const Elem* last, Elem* result)
{
    for (; first != last; ++first, ++result) new (result) Elem(*first);
    return result;
}
static void CopyConstruct(Vec& self, const Vec& x)
{
    unsigned int n = (unsigned int)(x.f - x.s);
    self.s = 0; self.f = 0; self.e = 0;
    self.s = n != 0 ? (++gAllocs, (Elem*)std::malloc(n * sizeof(Elem))) : 0;
    self.f = self.s;
    self.e = self.s + n;
    self.f = UninitCopy(x.s, x.f, self.s);
}
int main()
{
    int fails = 0;
    Vec empty; empty.s = 0; empty.f = 0; empty.e = 0;
    Vec a; CopyConstruct(a, empty);
    if (a.s != 0 || a.f != 0 || a.e != 0 || gAllocs != 0 || gCopies != 0) fails |= 1;
    Elem* src = (Elem*)std::malloc(sizeof(Elem) * 5);
    for (int k = 0; k < 3; ++k) new (src + k) Elem(k * 7 + 1);
    Vec x; x.s = src; x.f = src + 3; x.e = src + 5;
    Vec b; CopyConstruct(b, x);
    if (b.s == 0 || b.s == src || b.f != b.s + 3 || b.e != b.s + 3) fails |= 2;
    if (gAllocs != 1 || gCopies != 3) fails |= 4;
    if (b.s[0].v != 1 || b.s[1].v != 8 || b.s[2].v != 15) fails |= 8;
    std::free(b.s); std::free(src);
    if (fails) { std::printf("008ac540_TEST FAIL %d\n", fails); return 1; }
    std::printf("008ac540_TEST PASS\n");
    return 0;
}