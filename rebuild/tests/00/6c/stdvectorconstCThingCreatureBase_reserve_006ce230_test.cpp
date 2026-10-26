// Behaviour test for 0x006CE230 vector<T*>::reserve: a smaller capacity is grown to exactly n keeping the elements
// (copy + free of the old block when one exists, a plain allocation otherwise); a large enough capacity is untouched.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CThingCreatureBase { public: int m_id; };
typedef const CThingCreatureBase* Elem;

struct Vec {
    Elem* s; Elem* f; Elem* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    Elem* AllocateAndCopy(unsigned int n, Elem* first, Elem* last)
    {
        Elem* r = n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
        if (last != first) std::memmove(r, first, (last - first) * sizeof(Elem));
        return r;
    }
    void Clear() { if (s) std::free(s); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old = size();
            Elem* tmp;
            if (s) { tmp = AllocateAndCopy(n, s, f); Clear(); }
            else tmp = n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
            s = tmp; f = tmp + old; e = tmp + n;
        }
    }
};

int main()
{
    Vec v; v.s = v.f = v.e = 0;
    v.reserve(8);
    if (v.capacity() != 8 || v.size() != 0 || v.s == 0) { std::printf("006ce230_TEST FAIL\n"); return 1; }
    CThingCreatureBase a, b;
    *v.f++ = &a; *v.f++ = &b;
    Elem* old = v.s;
    v.reserve(4);                                      // already big enough
    if (v.s != old || v.capacity() != 8) { std::printf("006ce230_TEST FAIL\n"); return 1; }
    v.reserve(20);
    if (v.capacity() != 20 || v.size() != 2 || v.s[0] != &a || v.s[1] != &b) { std::printf("006ce230_TEST FAIL\n"); return 1; }
    std::free(v.s);
    std::printf("006ce230_TEST PASS\n");
    return 0;
}