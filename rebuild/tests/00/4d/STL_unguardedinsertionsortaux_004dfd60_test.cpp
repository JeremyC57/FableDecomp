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
struct CRegisteredTCInfo { long AllocFunc; unsigned long DriverType; CCharString Name; CRegisteredTCInfo(long a, unsigned long d, const char* n) : AllocFunc(a), DriverType(d), Name(n) { } };
struct SEntry {
    CCharString first; CRegisteredTCInfo second;
    SEntry(const char* k, long a) : first(k), second(a, (unsigned long)a * 2, k) { }
    SEntry(const SEntry& p) : first(p.first), second(p.second) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void UnguardedInsertionSortAux(SEntry* first, SEntry* last, SEntry*, CLess comp)
{
    for (SEntry* i = first; i != last; ++i)
        UnguardedLinearInsert(i, SEntry(*i), comp);
}
int main()
{
    bool ok = true;
    {
        SEntry a[6] = { SEntry("", 0), SEntry("light", 1), SEntry("combat", 2), SEntry("wound", 3), SEntry("brain", 4), SEntry("look", 5) };
        UnguardedInsertionSortAux(a + 1, a + 6, 0, CLess());
        const char* want[6] = { "", "brain", "combat", "light", "look", "wound" };
        long alloc[6] = { 0, 4, 2, 1, 5, 3 };
        for (int i = 0; i < 6; ++i)
            if (std::strcmp(a[i].first.m_text, want[i]) || a[i].second.AllocFunc != alloc[i] || std::strcmp(a[i].second.Name.m_text, want[i])) ok = false;
        if (g_live != 12) ok = false;
    }
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004dfd60_TEST FAIL\n"); return 1; }
    std::printf("004dfd60_TEST PASS\n");
    return 0;
}