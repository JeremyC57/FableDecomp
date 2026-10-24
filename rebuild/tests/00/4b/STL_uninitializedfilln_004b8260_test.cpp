#include <cstdio>
#include <new>

// Behaviour model of __uninitialized_fill_n of pair<CCharString, pair<CCharString, CCharString> > (0x004B8260): copy x
// into n slots (three string copies each), return the end.
static int g_copies = 0;
struct CCharString {
    const char* p;
    CCharString(const char* s) : p(s) { }
    CCharString(const CCharString& o) : p(o.p) { ++g_copies; }
};
template <class A, class B> struct pair { A first; B second; pair(const A& a, const B& b) : first(a), second(b) { } pair(const pair& o) : first(o.first), second(o.second) { } };
typedef pair<CCharString, pair<CCharString, CCharString> > Rec;
struct __false_type { __false_type() { } };
Rec* __uninitialized_fill_n(Rec* first, unsigned int n, const Rec& x, const __false_type&)
{
    Rec* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) Rec(x);
    return cur;
}
static unsigned char g_buf[sizeof(Rec) * 4];

int main()
{
    CCharString a("NAME"), b("OVERHEARD"), c("REQUESTED");
    Rec x(a, pair<CCharString, CCharString>(b, c));
    g_copies = 0;
    Rec* first = (Rec*)g_buf;
    Rec* end = __uninitialized_fill_n(first, 4u, x, __false_type());
    if (end != first + 4 || g_copies != 12) { std::printf("004b8260_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (first[i].first.p != a.p || first[i].second.first.p != b.p || first[i].second.second.p != c.p) { std::printf("004b8260_TEST FAIL\n"); return 1; }
    if (__uninitialized_fill_n(first, 0u, x, __false_type()) != first || g_copies != 12) { std::printf("004b8260_TEST FAIL\n"); return 1; }
    std::printf("004b8260_TEST PASS\n");
    return 0;
}