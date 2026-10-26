#include <cstdio>
#include <new>

struct __false_type { };
static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
struct CSymbolInfo {
    CCharString GroupName;
    CCharString EntryName;
    long        Val;
    CSymbolInfo() : Val(0) { }
};
struct SPairCharStringSymbolInfo {
    CCharString first;
    CSymbolInfo second;
};
static const char* g_names[3] = { "alpha", "beta", "gamma" };
static const char* g_groups[3] = { "g0", "g1", "g2" };

typedef SPairCharStringSymbolInfo T_;

static T_* UninitCopy(const T_* first, const T_* last, T_* result, const __false_type&)
{
    T_* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) T_(*first);
    return cur;
}

int main()
{
    bool ok = true;
    T_* src = (T_*)new double[(3 * sizeof(T_)) / sizeof(double) + 1];
    for (int i = 0; i < 3; ++i) {
        T_* s = new (&src[i]) T_();
        s->first.m_pData = g_names[i]; s->second.GroupName.m_pData = g_groups[i]; s->second.EntryName.m_pData = g_names[2 - i];
        s->second.Val = 40 + i;
    }
    static double storage[64];
    T_* dst = (T_*)storage;
    __false_type tag;
    g_stringCopies = 0;
    T_* r = UninitCopy(src, src + 3, dst, tag);
    if (g_stringCopies != 9) ok = false;
    if (r != dst + 3) ok = false;
    for (int i = 0; i < 3; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->first.m_pData != g_names[i] || d->second.GroupName.m_pData != g_groups[i] || d->second.EntryName.m_pData != g_names[2 - i]) ok = false;
        if (d->second.Val != 40 + i) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("009e6270_TEST FAIL\n"); return 1; }
    std::printf("009e6270_TEST PASS\n");
    return 0;
}