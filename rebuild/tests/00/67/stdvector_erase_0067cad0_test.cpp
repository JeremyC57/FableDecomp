// behaviour test for 0x0067CAD0 vector<CBlurredAttitudesDef >::erase(first, last): the tail is assigned down onto first in order, every vacated slot past the new finish is destroyed once through the virtual destructor, finish moves back by last - first, first is returned; an empty range only self-assigns the tail and erase(begin, end) empties the vector
#include <cstdio>
#include <cstdlib>
#include <new>
static int gDestroyed = 0;
static int gAssigned = 0;
class Elem {                                   // polymorphic like CBlurredAttitudesDef (virtual destructor in slot 0)
public:
    explicit Elem(int v) : Id(v) { }
    virtual ~Elem() { ++gDestroyed; Id = -1; }
    Elem& operator=(const Elem& other) { Id = other.Id; ++gAssigned; return *this; }
    int Id;
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
static Elem* Make(Vec& v, int n)
{
    Elem* p = (Elem*)std::malloc(sizeof(Elem) * 8);
    for (int k = 0; k < n; ++k) new (p + k) Elem(k * 10);
    v.s = p; v.f = p + n; v.e = p + 8;
    return p;
}
int main()
{
    int fails = 0;
    Vec v;
    Elem* p = Make(v, 6);
    Elem* r = Erase(v, p + 1, p + 3);                  // drop ids 10, 20
    if (r != p + 1) fails |= 1;
    if (v.f != p + 4 || v.s != p || v.e != p + 8) fails |= 2;
    if (p[0].Id != 0 || p[1].Id != 30 || p[2].Id != 40 || p[3].Id != 50) fails |= 4;
    if (gAssigned != 3 || gDestroyed != 2) fails |= 8;
    if (p[4].Id != -1 || p[5].Id != -1) fails |= 16;   // the two vacated slots were destroyed
    gAssigned = 0; gDestroyed = 0;
    r = Erase(v, p + 2, p + 2);                        // empty range: no early-out, the tail self-assigns
    if (r != p + 2 || v.f != p + 4 || gAssigned != 2 || gDestroyed != 0 || p[2].Id != 40 || p[3].Id != 50) fails |= 32;
    gAssigned = 0; gDestroyed = 0;
    r = Erase(v, p + 3, v.f);                          // erase the last element: no copy, one destroy
    if (r != p + 3 || v.f != p + 3 || gAssigned != 0 || gDestroyed != 1) fails |= 64;
    gAssigned = 0; gDestroyed = 0;
    r = Erase(v, v.s, v.f);                            // clear()
    if (r != p || v.f != p || gAssigned != 0 || gDestroyed != 3) fails |= 128;
    std::free(p);
    if (fails) { std::printf("0067cad0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0067cad0_TEST PASS\n");
    return 0;
}