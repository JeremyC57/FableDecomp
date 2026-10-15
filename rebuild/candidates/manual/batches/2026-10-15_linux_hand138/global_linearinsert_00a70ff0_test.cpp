// Behaviour test for 0x00a70ff0 __linear_insert over string-keyed records
#include <cstdio>
#include <cstring>
struct CCharString {
    char m_text[16];
    CCharString(const char* t) { std::strcpy(m_text, t); }
    bool operator<(const CCharString& o) const { return std::strcmp(m_text, o.m_text) < 0; }
};
struct SRecordData { int m_words[13]; };
struct SNamedRecord { CCharString first; SRecordData second; SNamedRecord(const char* n, int v) : first(n) { second.m_words[0] = v; } };
struct SNameLess { bool operator()(const SNamedRecord& a, const SNamedRecord& b) const { return a.first < b.first; } };
static SNamedRecord* copy_backward(SNamedRecord* first, SNamedRecord* last, SNamedRecord* result)
{
    while (first != last) *--result = *--last;
    return result;
}
static void __unguarded_linear_insert(SNamedRecord* last, SNamedRecord value, SNameLess comp)
{
    SNamedRecord* next = last; --next;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}
static void __linear_insert(SNamedRecord* first, SNamedRecord* last, SNamedRecord value, SNameLess comp)
{
    if (comp(value, *first)) { copy_backward(first, last, last + 1); *first = value; }
    else __unguarded_linear_insert(last, value, comp);
}
int main()
{
    SNamedRecord r[5] = { SNamedRecord("BANDIT", 1), SNamedRecord("GUARD", 2), SNamedRecord("TROLL", 3), SNamedRecord("ARCHER", 4), SNamedRecord("HOBBE", 5) };
    SNameLess less;
    __linear_insert(r, r + 3, r[3], less);   // ARCHER goes first
    __linear_insert(r, r + 4, r[4], less);   // HOBBE sinks between GUARD and TROLL
    const char* want[5] = { "ARCHER", "BANDIT", "GUARD", "HOBBE", "TROLL" }; int vals[5] = { 4, 1, 2, 5, 3 };
    bool ok = true;
    for (int i = 0; i < 5; ++i) ok = ok && std::strcmp(r[i].first.m_text, want[i]) == 0 && r[i].second.m_words[0] == vals[i];
    if (!ok) { std::printf("00a70ff0_TEST FAIL\n"); return 1; }
    std::printf("00a70ff0_TEST PASS\n");
    return 0;
}
