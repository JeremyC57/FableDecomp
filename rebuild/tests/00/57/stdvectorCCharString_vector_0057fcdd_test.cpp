// Behaviour test for 0x0057FCDD vector<CCharString>(size_type n): n copies of an empty string, finish = start + n.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Str { const char* p; Str() : p("") { ++g_live; } Str(const Str& o) : p(o.p) { ++g_live; } ~Str() { --g_live; } };
struct Vec {
    Str* start; Str* finish; Str* eos;
    explicit Vec(unsigned int n)
    {
        start = n ? (Str*)std::malloc(n * sizeof(Str)) : 0;
        finish = start; eos = start + n;
        Str def;
        Str* cur = start;
        for (unsigned int i = n; i > 0; --i, ++cur) new (cur) Str(def);
        finish = cur;
    }
};
int main()
{
    Vec v(3);
    if (v.finish != v.start + 3 || v.eos != v.start + 3 || g_live != 3 || v.start[2].p[0] != 0) { std::printf("0057fcdd_TEST FAIL\n"); return 1; }
    Vec e(0);
    if (e.start != 0 || e.finish != 0 || g_live != 3) { std::printf("0057fcdd_TEST FAIL\n"); return 1; }
    std::printf("0057fcdd_TEST PASS\n");
    return 0;
}