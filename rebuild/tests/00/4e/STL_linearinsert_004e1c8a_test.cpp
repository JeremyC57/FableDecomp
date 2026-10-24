#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CCharString {
    char m_text[12];
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
    bool operator<(const CCharString& o) const { return std::strcmp(m_text, o.m_text) < 0; }
};
struct CRegisteredTCInfo { long AllocFunc; unsigned long DriverType; CCharString Name; CRegisteredTCInfo(long a, const char* n) : AllocFunc(a), DriverType(0), Name(n) { } };
struct SEntry {
    CCharString first; CRegisteredTCInfo second;
    SEntry(const char* k, long a) : first(k), second(a, k) { }
    SEntry(const SEntry& p) : first(p.first), second(p.second) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static SEntry* CopyBackward(SEntry* first, SEntry* last, SEntry* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(SEntry* first, SEntry* last, SEntry val, CLess comp)
{
    if (comp(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
int main()
{
    bool ok = true;
    {
        SEntry a[4] = { SEntry("look", 1), SEntry("wound", 2), SEntry("brain", 3), SEntry("light", 4) };
        LinearInsert(a, a + 2, SEntry(a[2]), CLess());       // "brain" goes first
        if (std::strcmp(a[0].first.m_text, "brain") || a[0].second.AllocFunc != 3 || std::strcmp(a[2].first.m_text, "wound")) ok = false;
        LinearInsert(a, a + 3, SEntry(a[3]), CLess());       // "light" in the middle (unguarded)
        if (std::strcmp(a[1].first.m_text, "light") || std::strcmp(a[2].first.m_text, "look") || a[3].second.AllocFunc != 2) ok = false;
        if (g_live != 8) ok = false;
    }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004e1c8a_TEST FAIL\n"); return 1; }
    std::printf("004e1c8a_TEST PASS\n");
    return 0;
}