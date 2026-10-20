#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct CReverbEnvironmentEntry {               // PDB: 84 bytes, a POD block-copied as 21 dwords
    float Params[21];
};

struct CReverbEnvironmentEntryVector {
    typedef CReverbEnvironmentEntry* pointer;
    typedef const CReverbEnvironmentEntry* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer f, const_pointer l)
    { pointer b = n ? (CReverbEnvironmentEntry*)std::malloc(n * sizeof(CReverbEnvironmentEntry)) : 0; for (pointer d = b; f != l; ++f, ++d) new (d) CReverbEnvironmentEntry(*f); return b; }
    void _M_clear() { std::free(_M_start); }
    CReverbEnvironmentEntryVector& operator=(const CReverbEnvironmentEntryVector& __x)
    {
        if (&__x != this) {
            const unsigned int __xlen = __x.size();
            if (__xlen > capacity()) {
                pointer __tmp = _M_allocate_and_copy(__xlen, __x._M_start, __x._M_finish);
                _M_clear(); _M_start = __tmp; _M_end_of_storage = _M_start + __xlen;
            } else if (size() >= __xlen) {
                pointer d = _M_start; for (const_pointer s = __x._M_start; s != __x._M_finish; ++s, ++d) *d = *s;
            } else {
                pointer d = _M_start; const_pointer s = __x._M_start;
                for (; s != __x._M_start + size(); ++s, ++d) *d = *s;
                for (; s != __x._M_finish; ++s, ++d) new (d) CReverbEnvironmentEntry(*s);
            }
            _M_finish = _M_start + __xlen;
        }
        return *this;
    }
};
static void make(CReverbEnvironmentEntryVector& v, unsigned int n, unsigned int cap, int seed)
{
    v._M_start = cap ? (CReverbEnvironmentEntry*)std::malloc(cap * sizeof(CReverbEnvironmentEntry)) : 0; v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i) std::memset(&v._M_start[i], seed + (int)i, sizeof(CReverbEnvironmentEntry));
}
static bool same(const CReverbEnvironmentEntryVector& a, const CReverbEnvironmentEntryVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (std::memcmp(&a._M_start[i], &b._M_start[i], sizeof(CReverbEnvironmentEntry))) return false;
    return true;
}
int main()
{
    CReverbEnvironmentEntryVector a, b, c, d;
    make(a, 2, 2, 1); make(b, 5, 5, 20);
    a = b;                                   // grow: reallocate
    if (!same(a, b) || a.capacity() != 5) { std::printf("00461017_TEST FAIL\n"); return 1; }
    make(c, 1, 1, 40); a = c;                // shrink: copy over
    if (!same(a, c) || a.capacity() != 5) { std::printf("00461017_TEST FAIL\n"); return 1; }
    make(d, 4, 4, 60); a = d;                // within capacity: copy + construct tail
    if (!same(a, d) || a.capacity() != 5) { std::printf("00461017_TEST FAIL\n"); return 1; }
    a = a;
    if (!same(a, d)) { std::printf("00461017_TEST FAIL\n"); return 1; }
    std::printf("00461017_TEST PASS\n");
    return 0;
}
