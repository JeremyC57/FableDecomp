// Behaviour test for the camera-tag vector's _M_insert_overflow (0x004D1360): growth by max(size, n), head/new/tail order.
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() {} };
struct CCameraTag { long CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView; };
struct SCameraTagEntry {
    float first;
    CCameraTag second;
    SCameraTagEntry() {}
    SCameraTagEntry(const SCameraTagEntry& o) : first(o.first), second(o.second) {}
};
static SCameraTagEntry* __uninitialized_copy(SCameraTagEntry* f, SCameraTagEntry* l, SCameraTagEntry* r, const __false_type&)
{
    for (; f != l; ++f, ++r) *r = *f;
    return r;
}
static SCameraTagEntry* __uninitialized_fill_n(SCameraTagEntry* f, unsigned long n, const SCameraTagEntry& x, const __false_type&)
{
    for (; n; --n, ++f) *f = x;
    return f;
}
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
static unsigned long g_lastAlloc = 0;
struct CCameraTagVector {
    SCameraTagEntry* _M_start; SCameraTagEntry* _M_finish; SCameraTagEntry* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static SCameraTagEntry* allocate(unsigned long n)
    {
        g_lastAlloc = n;
        return n != 0 ? (SCameraTagEntry*)std::malloc(n * sizeof(SCameraTagEntry)) : 0;
    }
    static void deallocate(SCameraTagEntry* p) { if (p) std::free(p); }
    void _M_clear() { deallocate(_M_start); }
    void _M_set(SCameraTagEntry* s, SCameraTagEntry* f, SCameraTagEntry* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(SCameraTagEntry* position, const SCameraTagEntry& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        const unsigned long old_size = size();
        const unsigned long len = old_size + (max)(old_size, fill_len);
        SCameraTagEntry* new_start = allocate(len);
        SCameraTagEntry* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) {
            new (new_finish) SCameraTagEntry(x);
            ++new_finish;
        }
        else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
};

int main()
{
    int ok = 1;
    CCameraTagVector v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    SCameraTagEntry e; e.first = 1.5f; e.second.CameraView = 7; e.second.CameraPosition = 3;
    v._M_insert_overflow(v._M_finish, e, __false_type(), 1, true);          // empty: grows to 1
    if (v.size() != 1 || g_lastAlloc != 1 || v._M_start[0].first != 1.5f || v._M_start[0].second.CameraView != 7) ok = 0;
    SCameraTagEntry f; f.first = 2.5f; f.second.CameraView = 9; f.second.CameraPosition = 4;
    v._M_insert_overflow(v._M_start, f, __false_type(), 2, false);          // insert 2 at the front: len = 1 + 2
    if (v.size() != 3 || g_lastAlloc != 3 || v._M_end_of_storage != v._M_start + 3) ok = 0;
    if (v._M_start[0].first != 2.5f || v._M_start[1].second.CameraPosition != 4 || v._M_start[2].first != 1.5f) ok = 0;
    v._M_insert_overflow(v._M_finish, e, __false_type(), 1, true);          // append: len = 3 + 3
    if (v.size() != 4 || g_lastAlloc != 6 || v._M_start[3].second.CameraView != 7) ok = 0;
    v._M_clear();
    if (!ok) { std::printf("004d1360_TEST FAIL\n"); return 1; }
    std::printf("004d1360_TEST PASS\n");
    return 0;
}