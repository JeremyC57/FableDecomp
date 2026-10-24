// Behaviour test for 0x006E4950 vector<44-byte record>::_M_allocate_and_copy: n records are allocated and [first, last)
// is copied into the front of the block in order; n == 0 gives a null block.
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
#define FAIL { std::printf("006e4950_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(Rec) != 0x2c) FAIL;
    Rec src[2]; for (int i = 0; i < 11; ++i) { src[0].Words[i] = i; src[1].Words[i] = 100 + i; }
    Vec v; v.s = v.f = v.e = 0;
    Rec* block = v.AllocateAndCopy(5, src, src + 2);
    if (!block || block[0].Words[10] != 10 || block[1].Words[0] != 100 || block[1].Words[10] != 110) FAIL;
    std::free(block);
    if (v.AllocateAndCopy(0, src, src) != 0) FAIL;
    std::printf("006e4950_TEST PASS\n");
    return 0;
}