// Behaviour test for 0x0056D490 std::vector<T*>::vector(const vector&): the copy owns a fresh buffer of exactly
// size() pointers with the same values; an empty source gives an all-null vector (no allocation).
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_allocs = 0;
struct V {
    int** start; int** finish; int** eos;
    V() : start(0), finish(0), eos(0) { }
    V(const V& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (++g_allocs, (int**)std::malloc(n * sizeof(int*))) : 0;
        finish = start;
        eos = start + n;
        const char* f = (const char*)x.start; const char* l = (const char*)x.finish;
        finish = (int**)(l == f ? (void*)start : (void*)((char*)std::memmove(start, f, l - f) + (l - f)));
    }
};

int main()
{
    int a = 1, b = 2, c = 3;
    int* buf[3] = { &a, &b, &c };
    V src; src.start = buf; src.finish = buf + 3; src.eos = buf + 3;
    V cp(src);
    bool ok = cp.start != buf && cp.finish - cp.start == 3 && cp.eos == cp.finish &&
              cp.start[0] == &a && cp.start[1] == &b && cp.start[2] == &c && g_allocs == 1;
    V empty;
    V cp2(empty);
    ok = ok && cp2.start == 0 && cp2.finish == 0 && cp2.eos == 0 && g_allocs == 1;
    std::free(cp.start);
    std::printf(ok ? "0056d490_TEST PASS\n" : "0056d490_TEST FAIL\n");
    return ok ? 0 : 1;
}