// Behaviour test for 0x009f6890: STLport vector::operator= over a 52-byte non-POD element (three branches + self-assignment).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct CElem {
    long w[13];
    CElem(long v) { for (int i = 0; i < 13; ++i) w[i] = v + i; ++g_live; }
    CElem(const CElem& o) { for (int i = 0; i < 13; ++i) w[i] = o.w[i]; ++g_live; }
    CElem& operator=(const CElem& o) { for (int i = 0; i < 13; ++i) w[i] = o.w[i]; return *this; }
    ~CElem() { --g_live; }
};
static CElem* UninitializedCopy(const CElem* first, const CElem* last, CElem* result)
{ for (; first != last; ++first, ++result) new (result) CElem(*first); return result; }
static CElem* CopyRange(const CElem* first, const CElem* last, CElem* result)
{ for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; } return result; }
static void DestroyRange(CElem* first, CElem* last) { for (; first != last; ++first) first->~CElem(); }
struct CVec {
    CElem* _M_start; CElem* _M_finish; CElem* _M_end_of_storage;
    CVec() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned size() const { return unsigned(_M_finish - _M_start); }
    unsigned capacity() const { return unsigned(_M_end_of_storage - _M_start); }
    CElem* _M_allocate_and_copy(unsigned n, const CElem* first, const CElem* last)
    { CElem* r = n ? (CElem*)std::malloc(n * sizeof(CElem)) : 0; UninitializedCopy(first, last, r); return r; }
    void _M_clear() { DestroyRange(_M_start, _M_finish); std::free(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; }
    void Reserve(unsigned n) { _M_clear(); _M_start = _M_finish = (CElem*)std::malloc(n * sizeof(CElem)); _M_end_of_storage = _M_start + n; }
    void Push(long v) { new (_M_finish) CElem(v); ++_M_finish; }
    CVec& operator=(const CVec& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                CElem* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear(); _M_start = tmp; _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                CElem* i = CopyRange(x._M_start, x._M_finish, _M_start);
                DestroyRange(i, _M_finish);
            } else {
                CopyRange(x._M_start, x._M_start + size(), _M_start);
                UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static bool Same(const CVec& a, const CVec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned i = 0; i < a.size(); ++i) for (int k = 0; k < 13; ++k) if (a._M_start[i].w[k] != b._M_start[i].w[k]) return false;
    return true;
}
int main()
{
    int ok = 1;
    {
        CVec src; src.Reserve(4); src.Push(10); src.Push(20); src.Push(30);
        CVec a; a.Reserve(1); a.Push(1); a = src; if (!Same(a, src) || a.capacity() != 3) ok = 0;
        CVec b; b.Reserve(5); b.Push(1); b.Push(2); b.Push(3); b.Push(4); b = src; if (!Same(b, src) || b.capacity() != 5) ok = 0;
        CVec c; c.Reserve(6); c.Push(7); c = src; if (!Same(c, src) || c.capacity() != 6) ok = 0;
        CVec& s = (src = src); if (&s != &src || src.size() != 3) ok = 0;
        CVec e; e = CVec(); if (e.size() != 0) ok = 0;
        if (g_live != 12) ok = 0;
        src._M_clear(); a._M_clear(); b._M_clear(); c._M_clear();
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("009f6890_TEST FAIL\n"); return 1; }
    std::printf("009f6890_TEST PASS\n");
    return 0;
}