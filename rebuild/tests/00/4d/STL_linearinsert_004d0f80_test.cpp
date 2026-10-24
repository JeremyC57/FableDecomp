#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CCharString {
    char m_text[12];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
struct SPair {
    float first; CCharString second;
    SPair(float f, const char* s) : first(f), second(s) { }
    SPair(const SPair& p) : first(p.first), second(p.second) { }
};
struct CLess { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };
static SPair* CopyBackward(SPair* first, SPair* last, SPair* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void UnguardedLinearInsert(SPair* last, SPair val, CLess comp)
{
    SPair* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(SPair* first, SPair* last, SPair val, CLess comp)
{
    if (comp(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
int main()
{
    bool ok = true;
    {
        SPair a[4] = { SPair(1.0f, "b"), SPair(3.0f, "d"), SPair(0.5f, "a"), SPair(2.0f, "c") };
        LinearInsert(a, a + 2, SPair(a[2]), CLess());
        if (std::strcmp(a[0].second.m_text, "a") || std::strcmp(a[1].second.m_text, "b") || std::strcmp(a[2].second.m_text, "d")) ok = false;
        LinearInsert(a, a + 3, SPair(a[3]), CLess());
        if (std::strcmp(a[2].second.m_text, "c") || std::strcmp(a[3].second.m_text, "d") || a[3].first != 3.0f || a[0].first != 0.5f) ok = false;
        if (g_live != 4) ok = false;                         // every by-value copy destroyed
    }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004d0f80_TEST FAIL\n"); return 1; }
    std::printf("004d0f80_TEST PASS\n");
    return 0;
}