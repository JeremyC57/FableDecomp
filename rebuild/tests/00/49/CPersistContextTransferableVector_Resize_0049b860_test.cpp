#include <cstdio>
static int g_erase = -1, g_insert = -1;
struct Str { void* p; };
struct Vec {
    Str* s; Str* f; Str* e;
    unsigned long size() const { return (unsigned long)(f - s); }
    void erase(Str* first, Str* last) { g_erase = (int)(first - s); f = first; }
    void insert(Str* pos, unsigned long n, const Str&) { g_insert = (int)n; f = pos + n; }
    void resize(unsigned long n, const Str& x) { if (n < size()) erase(s + n, f); else insert(f, n - size(), x); }
};
struct TV { Vec* v; void Resize(unsigned long count) { Str d; d.p = 0; v->resize(count, d); } };
int main()
{
    static Str buf[16]; Vec v; v.s = buf; v.f = buf + 5; v.e = buf + 16; TV t; t.v = &v;
    t.Resize(2); int ok = g_erase == 2 && v.size() == 2;
    t.Resize(7); ok = ok && g_insert == 5 && v.size() == 7;
    t.Resize(7); ok = ok && g_insert == 0;
    if (!ok) { std::printf("0049b860_TEST FAIL\n"); return 1; }
    std::printf("0049b860_TEST PASS\n");
    return 0;
}