#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
class COpinionDeedReactionDef {
public:
    int value;
    COpinionDeedReactionDef() : value(0) { ++g_live; }
    COpinionDeedReactionDef(int v) : value(v) { ++g_live; }
    COpinionDeedReactionDef(const COpinionDeedReactionDef& o) : value(o.value) { ++g_live; }
    virtual ~COpinionDeedReactionDef() { --g_live; }
    COpinionDeedReactionDef& operator=(const COpinionDeedReactionDef& o) { value = o.value; return *this; }
};
struct COpinionDeedReactionDefVector {
    typedef COpinionDeedReactionDef* pointer;
    typedef const COpinionDeedReactionDef* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer f, const_pointer l)
    { pointer b = n ? (pointer)std::malloc(n * sizeof(COpinionDeedReactionDef)) : 0; for (pointer d = b; f != l; ++f, ++d) new (d) COpinionDeedReactionDef(*f); return b; }
    void _M_clear() { for (pointer p = _M_start; p != _M_finish; ++p) p->~COpinionDeedReactionDef(); std::free(_M_start); }
    COpinionDeedReactionDefVector& operator=(const COpinionDeedReactionDefVector& __x)
    {
        if (&__x != this) {
            const unsigned int __xlen = __x.size();
            if (__xlen > capacity()) {
                pointer __tmp = _M_allocate_and_copy(__xlen, __x._M_start, __x._M_finish);
                _M_clear(); _M_start = __tmp; _M_end_of_storage = _M_start + __xlen;
            } else if (size() >= __xlen) {
                pointer d = _M_start; for (const_pointer s = __x._M_start; s != __x._M_finish; ++s, ++d) *d = *s;
                for (; d != _M_finish; ++d) d->~COpinionDeedReactionDef();
            } else {
                pointer d = _M_start; const_pointer s = __x._M_start;
                for (; s != __x._M_start + size(); ++s, ++d) *d = *s;
                for (; s != __x._M_finish; ++s, ++d) new (d) COpinionDeedReactionDef(*s);
            }
            _M_finish = _M_start + __xlen;
        }
        return *this;
    }
};
static void make(COpinionDeedReactionDefVector& v, unsigned int n, unsigned int cap, int seed)
{
    v._M_start = (COpinionDeedReactionDef*)std::malloc((cap ? cap : 1) * sizeof(COpinionDeedReactionDef));
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i) new (&v._M_start[i]) COpinionDeedReactionDef(seed + (int)i);
}
static bool same(const COpinionDeedReactionDefVector& a, const COpinionDeedReactionDefVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (a._M_start[i].value != b._M_start[i].value) return false;
    return true;
}
int main()
{
    COpinionDeedReactionDefVector a, b, c, d;
    make(a, 2, 2, 1); make(b, 5, 5, 20); make(c, 1, 1, 40); make(d, 4, 4, 60);
    int before = g_live;                         // 12 live
    a = b;                                       // grow: 2 destroyed, 5 built
    if (!same(a, b) || a.capacity() != 5 || g_live != before + 3) { std::printf("00471cc5_TEST FAIL\n"); return 1; }
    a = c;                                       // shrink: 4 surplus destroyed
    if (!same(a, c) || a.capacity() != 5 || g_live != before - 1) { std::printf("00471cc5_TEST FAIL\n"); return 1; }
    a = d;                                       // within capacity: 3 tail constructed
    if (!same(a, d) || a.capacity() != 5 || g_live != before + 2) { std::printf("00471cc5_TEST FAIL\n"); return 1; }
    std::printf("00471cc5_TEST PASS\n");
    return 0;
}