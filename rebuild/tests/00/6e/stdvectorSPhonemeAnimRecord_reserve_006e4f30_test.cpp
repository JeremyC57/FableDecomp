// Behaviour test for 0x006E4F30 vector<44-byte record>::reserve: grows only when the capacity is below n, keeps the
// elements and the size, and allocates directly when the vector has no block.
#include <cstdio>
#include <cstdlib>
#include <new>

struct Rec { int Words[11]; };
struct Vec {
    Rec* s; Rec* f; Rec* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    Rec* AllocateAndCopy(unsigned int n, const Rec* first, const Rec* last)
    {
        Rec* result = n ? (Rec*)std::malloc(n * sizeof(Rec)) : 0;
        Rec* cur = result;
        for (; first != last; ++first, ++cur) if (cur) new (cur) Rec(*first);
        return result;
    }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            unsigned int old = size();
            Rec* tmp;
            if (s) { tmp = AllocateAndCopy(n, s, f); std::free(s); }
            else tmp = n ? (Rec*)std::malloc(n * sizeof(Rec)) : 0;
            s = tmp; f = tmp + old; e = tmp + n;
        }
    }
};
#define FAIL { std::printf("006e4f30_TEST FAIL\n"); return 1; }
int main()
{
    Vec v; v.s = v.f = v.e = 0;
    v.reserve(3);
    if (!v.s || v.size() != 0 || v.capacity() != 3) FAIL;
    v.s[0].Words[0] = 7; v.s[1].Words[5] = 9; v.f = v.s + 2;
    Rec* old = v.s;
    v.reserve(2);
    if (v.s != old || v.capacity() != 3) FAIL;
    v.reserve(8);
    if (v.capacity() != 8 || v.size() != 2 || v.s[0].Words[0] != 7 || v.s[1].Words[5] != 9) FAIL;
    std::free(v.s);
    std::printf("006e4f30_TEST PASS\n");
    return 0;
}