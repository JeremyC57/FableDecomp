// Behaviour test for 0x00A34950 CVectorMap<unsigned long, C3DHelperPoint>::Insert: append through push_back (in place while capacity lasts, else the overflow path) and mark the map dirty.
#include <cstdio>
#include <new>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
struct C3DHelperPoint { C3DVector Position; int BoneIndex; };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, C3DHelperPoint> TEntry;
static void SetE(TEntry& e, unsigned int key, int tag) { e.first = key; e.second.BoneIndex = tag; e.second.Position.X = (float)tag; e.second.Position.Y = 0.5f; e.second.Position.Z = (float)(tag * 2); }
static bool TagOk(const TEntry& e) { float d = e.second.Position.Z - (float)(e.second.BoneIndex * 2); return d < 0.001f && d > -0.001f; }
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
static TEntry* __uninitialized_copy(const TEntry* first, const TEntry* last, TEntry* result, const __false_type&)
{
    TEntry* cur = result;
    for (; first != last; ++first, ++cur) _Construct(cur, *first);
    return cur;
}
static TEntry* __uninitialized_fill_n(TEntry* first, unsigned int n, const TEntry& x, const __false_type&)
{
    TEntry* cur = first;
    for (; n > 0; --n, ++cur) _Construct(cur, x);
    return cur;
}
static int g_allocs = 0, g_frees = 0;
class TVector {
public:
    TEntry* _M_start;
    TEntry* _M_finish;
    TEntry* _M_end_of_storage;
    TVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(TEntry* position, const TEntry& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (max)(old_size, fill_len);
        TEntry* new_start = len != 0 ? (TEntry*)::operator new(len * sizeof(TEntry)) : 0;
        ++g_allocs;
        TEntry* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) { _Construct(new_finish, x); ++new_finish; }
        else new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend) new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        if (_M_start != 0) { ::operator delete(_M_start); ++g_frees; }
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
    void push_back(const TEntry& x)
    {
        if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CVectorMap : public TVector {
public:
    unsigned char Compare;
    bool Dirty;
    CVectorMap() : Compare(0), Dirty(false) { }
    void Insert(const TEntry& x) { push_back(x); Dirty = true; }
};
static int Fail() { std::printf("00a34950_TEST FAIL\n"); return 1; }

int main()
{
    CVectorMap m;
    for (int i = 0; i < 5; ++i) {
        TEntry e; SetE(e, (unsigned int)(10 - i), i);
        m.Dirty = false;
        m.Insert(e);
        if (!m.Dirty) return Fail();
    }
    if (m.size() != 5 || g_allocs != 4 || g_frees != 3) return Fail();      // capacities 1, 2, 4, 8
    for (int i = 0; i < 5; ++i) if (m._M_start[i].first != (unsigned int)(10 - i) || m._M_start[i].second.BoneIndex != i || !TagOk(m._M_start[i])) return Fail();
    ::operator delete(m._M_start);
    std::printf("00a34950_TEST PASS\n");
    return 0;
}