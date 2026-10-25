// Behaviour test for 0x009B8DD0 vector<pair<unsigned long, CDefClassInfo> >::_M_insert_overflow (STLport grow path).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct Info {                                   // stands in for CDefClassInfo (counted copies)
    unsigned int Name;
    unsigned int AllocFunc;
    Info(unsigned int n) : Name(n), AllocFunc(n + 1) { ++g_live; }
    Info(const Info& o) : Name(o.Name), AllocFunc(o.AllocFunc) { ++g_live; }
    ~Info() { --g_live; }
};
struct Pair {
    unsigned int first;
    Info second;
    Pair(unsigned int k) : first(k), second(k * 10) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};

static Pair* UninitCopy(const Pair* first, const Pair* last, Pair* result)
{
    for (; first != last; ++first, ++result) new (result) Pair(*first);
    return result;
}
static Pair* UninitFillN(Pair* first, unsigned int n, const Pair& x)
{
    for (; n > 0; --n, ++first) new (first) Pair(x);
    return first;
}
static void DestroyRange(Pair* first, Pair* last) { for (; first != last; ++first) first->~Pair(); }

struct Vec {
    Pair* _M_start; Pair* _M_finish; Pair* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(Pair* pos, const Pair& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Pair* new_start = len != 0 ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* new_finish = UninitCopy(_M_start, pos, new_start);
        if (fill_len == 1) {
            new (new_finish) Pair(x);
            ++new_finish;
        } else
            new_finish = UninitFillN(new_finish, fill_len, x);
        if (!atend)
            new_finish = UninitCopy(pos, _M_finish, new_finish);
        DestroyRange(_M_start, _M_finish);
        if (_M_start) std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

static int Check(const Vec& v, const unsigned int* want, unsigned int n, unsigned int cap)
{
    if (v.size() != n || (unsigned int)(v._M_end_of_storage - v._M_start) != cap || g_live != (int)n) return 0;
    for (unsigned int i = 0; i < n; ++i)
        if (v._M_start[i].first != want[i] || v._M_start[i].second.Name != want[i] * 10 ||
            v._M_start[i].second.AllocFunc != want[i] * 10 + 1) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    Vec v;
    v._M_start = (Pair*)std::malloc(2 * sizeof(Pair));
    new (v._M_start) Pair(1);
    new (v._M_start + 1) Pair(2);
    v._M_finish = v._M_end_of_storage = v._M_start + 2;
    {
        Pair x(9);
        v._M_insert_overflow(v._M_finish, x, 1, true);          // push_back path: 2 + max(2, 1) = 4
        { unsigned int w[] = { 1, 2, 9 }; g_live -= 1; ok &= Check(v, w, 3, 4); g_live += 1; }
        Pair y(5);
        v._M_insert_overflow(v._M_start + 1, y, 3, false);      // fill 3 in the middle: 3 + max(3, 3) = 6
        { unsigned int w[] = { 1, 5, 5, 5, 2, 9 }; g_live -= 2; ok &= Check(v, w, 6, 6); g_live += 2; }
    }
    DestroyRange(v._M_start, v._M_finish);
    std::free(v._M_start);
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("009b8dd0_TEST FAIL\n"); return 1; }
    std::printf("009b8dd0_TEST PASS\n");
    return 0;
}