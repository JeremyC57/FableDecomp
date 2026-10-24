// Behaviour test for 0x00A32020 _STL::__uninitialized_fill_n over the 20-byte helper-point entry: n copies of x built in
// raw storage, the end pointer returned; n == 0 builds nothing and returns first.
#include <cstdio>
#include <new>
static int g_copies = 0;
struct SRecordInfo {
    unsigned int m_a, m_b, m_c, m_d;
    SRecordInfo() : m_a(0), m_b(0), m_c(0), m_d(0) { }
    SRecordInfo(const SRecordInfo& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c), m_d(o.m_d) { ++g_copies; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, SRecordInfo> TEntry;
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
static TEntry* __uninitialized_fill_n(TEntry* first, unsigned int n, const TEntry& x, const __false_type&)
{
    TEntry* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
int main()
{
    TEntry x; x.first = 42; x.second.m_a = 1; x.second.m_b = 2; x.second.m_c = 3; x.second.m_d = 4;
    static unsigned char raw[5 * sizeof(TEntry)];
    TEntry* dst = (TEntry*)raw;
    TEntry* end = __uninitialized_fill_n(dst, 4, x, __false_type());
    if (end != dst + 4 || g_copies != 4) { std::printf("00a32020_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (dst[i].first != 42 || dst[i].second.m_a != 1 || dst[i].second.m_d != 4) { std::printf("00a32020_TEST FAIL\n"); return 1; }
    if (__uninitialized_fill_n(dst, 0, x, __false_type()) != dst || g_copies != 4) { std::printf("00a32020_TEST FAIL\n"); return 1; }
    std::printf("00a32020_TEST PASS\n");
    return 0;
}