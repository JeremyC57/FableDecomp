// Behaviour test for 0x005806DF std::vector<CHeroMarriageInfo>::reserve (model copy)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
struct Rec { unsigned int w[6]; };
static int g_freed = 0;
struct Vec {
    Rec* start; Rec* finish; Rec* eos;
    size_t size() const { return size_t(finish - start); }
    size_t capacity() const { return size_t(eos - start); }
    Rec* allocate(size_t n) { return n ? (Rec*)malloc(n * sizeof(Rec)) : 0; }
    Rec* allocate_and_copy(size_t n, const Rec* f, const Rec* l) { Rec* r = allocate(n); Rec* d = r; for (; f != l; ++f, ++d) *d = *f; return r; }
    void clear_storage() { if (start) { free(start); ++g_freed; } }
    void reserve(size_t n)
    {
        if (capacity() < n) {
            const size_t old = size();
            Rec* tmp;
            if (start) { tmp = allocate_and_copy(n, start, finish); clear_storage(); }
            else tmp = allocate(n);
            start = tmp; finish = tmp + old; eos = tmp + n;
        }
    }
};
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    v.reserve(4);
    if (v.capacity() != 4 || v.size() != 0 || g_freed != 0) { std::printf("005806df_TEST FAIL\n"); return 1; }
    v.finish->w[0] = 7; ++v.finish; v.finish->w[0] = 9; ++v.finish;
    v.reserve(2);                                   // no-op
    if (v.capacity() != 4 || v.size() != 2) { std::printf("005806df_TEST FAIL\n"); return 1; }
    v.reserve(10);
    if (v.capacity() != 10 || v.size() != 2 || v.start[1].w[0] != 9 || g_freed != 1) { std::printf("005806df_TEST FAIL\n"); return 1; }
    std::printf("005806df_TEST PASS\n");
    return 0;
}