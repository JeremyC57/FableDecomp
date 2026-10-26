// Behaviour test for 0x004BB400 vector<pair<CCharString, CPersistInfo> >::_M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CCharString { public:
    int id;
    CCharString(int v) : id(v) { ++g_live; }
    CCharString(const CCharString& o) : id(o.id) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CPersistInfo {
    CCharString QuestName;
    CCharString PersistScript;
    CPersistInfo(int q, int p) : QuestName(q), PersistScript(p) { }
};
struct Pair {
    CCharString  first;
    CPersistInfo second;
    Pair(int k, int q, int p) : first(k), second(q, p) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};

struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Pair* pos, const Pair& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Pair* ns = len ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ns;
        for (Pair* p = start; p != pos; ++p, ++nf) new (nf) Pair(*p);
        if (fill_len == 1) { new (nf) Pair(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) Pair(x);
        if (!atend) for (Pair* p = pos; p != finish; ++p, ++nf) new (nf) Pair(*p);
        for (Pair* p = start; p != finish; ++p) p->~Pair();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    Pair a(1, 10, 100), b(2, 20, 200), c(3, 30, 300);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.start, b, 1, false);              // insert at front
    v.insert_overflow(v.finish, c, 3, true);              // len = 2 + max(2,3) = 5
    if (v.size() != 5 || v.eos - v.start != 5) ok = 0;
    int keys[5] = { 2, 1, 3, 3, 3 };
    for (int i = 0; i < 5; ++i)
        if (v.start[i].first.id != keys[i] || v.start[i].second.QuestName.id != keys[i] * 10
            || v.start[i].second.PersistScript.id != keys[i] * 100) ok = 0;
    if (g_live != 9 + 15) ok = 0;
    for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    std::free(v.start);
    if (g_live != 9) ok = 0;
    if (!ok) { std::printf("004bb400_TEST FAIL\n"); return 1; }
    std::printf("004bb400_TEST PASS\n");
    return 0;
}