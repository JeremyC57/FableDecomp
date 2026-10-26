// Behaviour test for 0x00786E40 UninitCopy_NamedRecord12_00786e40: placement copy of {long, CCharString, long} records.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pStringData;
    CCharString() : m_pStringData("") { }
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
struct SNamedRecord12 { long m_id; CCharString m_name; long m_value; };

static SNamedRecord12* UninitCopy(const SNamedRecord12* first, const SNamedRecord12* last, SNamedRecord12* result, const __false_type&)
{
    SNamedRecord12* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNamedRecord12(*first);
    return cur;
}

int main()
{
    static const char* names[3] = { "one", "two", "three" };
    SNamedRecord12 src[3];
    for (int i = 0; i < 3; ++i) { src[i].m_id = i + 1; src[i].m_name.m_pStringData = names[i]; src[i].m_value = -10 * i; }
    SNamedRecord12 dst[3];
    __false_type tag;
    SNamedRecord12* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_stringCopies == 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_id != k + 1 || dst[k].m_name.m_pStringData != names[k] || dst[k].m_value != -10 * k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || g_stringCopies != 3) ok = false;
    if (!ok) { std::printf("00786e40_TEST FAIL\n"); return 1; }
    std::printf("00786e40_TEST PASS\n");
    return 0;
}