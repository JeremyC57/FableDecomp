// Behaviour test for 0x0086ABF0 UninitCopy_PairLongRecord12_0086abf0: placement copy of pair<long, 12-byte POD>.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRecord12 { long m_0, m_4, m_8; };
struct SPair {
    long      first;
    SRecord12 second;
    SPair() : first(0) { second.m_0 = 0; second.m_4 = 0; second.m_8 = 0; }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};

static SPair* UninitCopy(const SPair* first, const SPair* last, SPair* result, const __false_type&)
{
    SPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SPair(*first);
    return cur;
}

int main()
{
    SPair src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 50 + i; src[i].second.m_0 = i; src[i].second.m_4 = 2 * i; src[i].second.m_8 = 3 * i; }
    SPair dst[3];
    __false_type tag;
    SPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != 50 + k || dst[k].second.m_0 != k || dst[k].second.m_4 != 2 * k || dst[k].second.m_8 != 3 * k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("0086abf0_TEST FAIL\n"); return 1; }
    std::printf("0086abf0_TEST PASS\n");
    return 0;
}