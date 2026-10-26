// Behaviour test for 0x006D4680 vector<pair<CCharString,long> >::reserve: growing copies every (string, value) pair
// into the new block (copy constructors run), destroys the old elements and frees the old block; size is kept.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;
class CCharString { public:
    char* p;
    CCharString(const char* s) { p = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(p, s); ++g_live; }
    CCharString(const CCharString& o) { p = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(p, o.p); ++g_live; }
    ~CCharString() { std::free(p); --g_live; }
};
struct Pair { CCharString first; long second; Pair(const char* s, long v) : first(s), second(v) {} };

struct Vec {
    Pair* s; Pair* f; Pair* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    static Pair* Alloc(unsigned int n) { return n ? (Pair*)std::malloc(n * sizeof(Pair)) : 0; }
    static Pair* UninitCopy(Pair* first, Pair* last, Pair* out) { for (; first != last; ++first, ++out) new (out) Pair(*first); return out; }
    void Clear() { for (Pair* p = s; p != f; ++p) p->~Pair(); if (s) std::free(s); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old = size();
            Pair* tmp;
            if (s) { tmp = Alloc(n); UninitCopy(s, f, tmp); Clear(); }
            else tmp = Alloc(n);
            s = tmp; f = tmp + old; e = tmp + n;
        }
    }
};

int main()
{
    Vec v; v.s = v.f = v.e = 0;
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0) { std::printf("006d4680_TEST FAIL\n"); return 1; }
    new (v.f++) Pair("alpha", 1);
    new (v.f++) Pair("beta", 2);
    v.reserve(5);
    if (v.capacity() != 5 || v.size() != 2 || std::strcmp(v.s[0].first.p, "alpha") || v.s[1].second != 2 || g_live != 2) { std::printf("006d4680_TEST FAIL\n"); return 1; }
    Pair* keep = v.s; v.reserve(3);
    if (v.s != keep) { std::printf("006d4680_TEST FAIL\n"); return 1; }
    v.Clear();
    if (g_live != 0) { std::printf("006d4680_TEST FAIL\n"); return 1; }
    std::printf("006d4680_TEST PASS\n");
    return 0;
}