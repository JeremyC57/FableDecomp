#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#pragma pack(push, 2)
class CEngineGraphic { public:                        // PDB: 14 bytes, 2-byte packed
    unsigned long BankIndex;                   // +0x00
    float         AnimStep;                    // +0x04
    float         RenderSizeX;                 // +0x08
    bool          AdditiveAlpha;               // +0x0c
    unsigned char Type;                        // +0x0d
};
#pragma pack(pop)

class CDegradableInfo { public:                       // PDB: 28 bytes, a POD block-copied as 7 dwords
    float          HealthPercentage;           // +0x00
    CEngineGraphic EngineGraphic;              // +0x04
    long           SmashParticleEmitter;       // +0x14
    bool           BlocksNavigation;           // +0x18
};

struct CDegradableInfoVector {
    typedef CDegradableInfo* pointer;
    typedef const CDegradableInfo* const_pointer;
    pointer _M_start, _M_finish, _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer f, const_pointer l)
    { pointer b = n ? (CDegradableInfo*)std::malloc(n * sizeof(CDegradableInfo)) : 0; for (pointer d = b; f != l; ++f, ++d) new (d) CDegradableInfo(*f); return b; }
    void _M_clear() { std::free(_M_start); }
    CDegradableInfoVector& operator=(const CDegradableInfoVector& __x)
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
                for (; s != __x._M_finish; ++s, ++d) new (d) CDegradableInfo(*s);
            }
            _M_finish = _M_start + __xlen;
        }
        return *this;
    }
};
static void make(CDegradableInfoVector& v, unsigned int n, unsigned int cap, int seed)
{
    v._M_start = cap ? (CDegradableInfo*)std::malloc(cap * sizeof(CDegradableInfo)) : 0; v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i) std::memset(&v._M_start[i], seed + (int)i, sizeof(CDegradableInfo));
}
static bool same(const CDegradableInfoVector& a, const CDegradableInfoVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (std::memcmp(&a._M_start[i], &b._M_start[i], sizeof(CDegradableInfo))) return false;
    return true;
}
int main()
{
    CDegradableInfoVector a, b, c, d;
    make(a, 2, 2, 1); make(b, 5, 5, 20);
    a = b;                                   // grow: reallocate
    if (!same(a, b) || a.capacity() != 5) { std::printf("00454467_TEST FAIL\n"); return 1; }
    make(c, 1, 1, 40); a = c;                // shrink: copy over
    if (!same(a, c) || a.capacity() != 5) { std::printf("00454467_TEST FAIL\n"); return 1; }
    make(d, 4, 4, 60); a = d;                // within capacity: copy + construct tail
    if (!same(a, d) || a.capacity() != 5) { std::printf("00454467_TEST FAIL\n"); return 1; }
    a = a;
    if (!same(a, d)) { std::printf("00454467_TEST FAIL\n"); return 1; }
    std::printf("00454467_TEST PASS\n");
    return 0;
}