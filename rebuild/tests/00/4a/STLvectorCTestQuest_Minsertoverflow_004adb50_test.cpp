#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour model of vector<CTestQuest>::_M_insert_overflow (0x004ADB50): len = old + max(old, n); copy the head,
// construct n copies of x (a single placement copy when n == 1), copy the tail unless appending, destroy + free the old
// block, adopt the new one.
static int g_alive = 0;
struct __false_type { __false_type() { } };
class CTestQuest { public:
    int id;
    CTestQuest(int i) : id(i) { ++g_alive; }
    CTestQuest(const CTestQuest& o) : id(o.id) { ++g_alive; }
    ~CTestQuest() { --g_alive; }
};
CTestQuest* ucopy(CTestQuest* f, CTestQuest* l, CTestQuest* r, const __false_type&) { for ( ; f != l; ++f, ++r) new (r) CTestQuest(*f); return r; }
CTestQuest* ufill(CTestQuest* f, unsigned int n, const CTestQuest& x, const __false_type&) { for ( ; n > 0; --n, ++f) new (f) CTestQuest(x); return f; }
void destroy(CTestQuest* f, CTestQuest* l, const __false_type&) { for ( ; f != l; ++f) f->~CTestQuest(); }
template <class T> const T& max_(const T& a, const T& b) { return a < b ? b : a; }
struct Vec {
    CTestQuest* start; CTestQuest* finish; CTestQuest* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CTestQuest* pos, const CTestQuest& x, const __false_type&, unsigned int fill, bool atend)
    {
        const unsigned int old = size();
        const unsigned int len = old + max_(old, fill);
        CTestQuest* ns = len ? (CTestQuest*)std::malloc(len * sizeof(CTestQuest)) : 0;
        CTestQuest* nf = ucopy(start, pos, ns, __false_type());
        if (fill == 1) { new (nf) CTestQuest(x); ++nf; }
        else nf = ufill(nf, fill, x, __false_type());
        if (!atend) nf = ucopy(pos, finish, nf, __false_type());
        destroy(start, finish, __false_type());
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v = { 0, 0, 0 };
    v.insert_overflow(v.finish, CTestQuest(1), __false_type(), 1, true);          // empty -> 1 (len 0 + max(0,1) = 1)
    if (v.size() != 1 || v.eos != v.start + 1 || v.start[0].id != 1 || g_alive != 1) { std::printf("004adb50_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, CTestQuest(2), __false_type(), 1, true);          // 1 -> 2, capacity 2
    if (v.size() != 2 || v.eos != v.start + 2 || v.start[1].id != 2) { std::printf("004adb50_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, CTestQuest(9), __false_type(), 3, false);      // middle insert of 3: capacity 2 + 3
    if (v.size() != 5 || v.eos != v.start + 5 || g_alive != 5) { std::printf("004adb50_TEST FAIL\n"); return 1; }
    int want[5] = { 1, 9, 9, 9, 2 };
    for (int i = 0; i < 5; ++i) if (v.start[i].id != want[i]) { std::printf("004adb50_TEST FAIL\n"); return 1; }
    destroy(v.start, v.finish, __false_type()); std::free(v.start);
    if (g_alive != 0) { std::printf("004adb50_TEST FAIL\n"); return 1; }
    std::printf("004adb50_TEST PASS\n");
    return 0;
}