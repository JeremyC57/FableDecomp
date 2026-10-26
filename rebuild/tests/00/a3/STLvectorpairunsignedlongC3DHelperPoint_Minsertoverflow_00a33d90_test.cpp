// Behaviour test for 0x00A33D90 vector<pair<unsigned long, C3DHelperPoint> >::_M_insert_overflow: growth to max(2*size, size+n), the head, n copies of x and (unless appending) the tail, the old block freed.
#include <cstdio>
#include <new>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class C3DHelperPoint { public: C3DVector Position; int BoneIndex; };
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
static int Fail() { std::printf("00a33d90_TEST FAIL\n"); return 1; }

int main()
{
    TVector v;
    TEntry x; SetE(x, 7, 70);
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);          // empty -> capacity 1
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].first != 7 || g_frees != 0) return Fail();
    TEntry y; SetE(y, 9, 90);
    v._M_insert_overflow(v._M_finish, y, __false_type(), 1, true);          // 1 -> 2
    if (v.size() != 2 || v._M_end_of_storage - v._M_start != 2 || v._M_start[1].first != 9 || g_frees != 1) return Fail();
    TEntry z; SetE(z, 3, 30);
    v._M_insert_overflow(v._M_start + 1, z, __false_type(), 3, false);      // insert 3 copies in the middle: 2 + max(2,3) = 5
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5) return Fail();
    unsigned int want[5] = { 7, 3, 3, 3, 9 };
    for (int i = 0; i < 5; ++i) if (v._M_start[i].first != want[i] || !TagOk(v._M_start[i])) return Fail();
    if (g_allocs != 3 || g_frees != 2) return Fail();
    ::operator delete(v._M_start);
    std::printf("00a33d90_TEST PASS\n");
    return 0;
}