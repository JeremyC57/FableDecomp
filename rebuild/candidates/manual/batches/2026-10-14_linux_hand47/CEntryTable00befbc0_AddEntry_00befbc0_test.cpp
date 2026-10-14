#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00BEFBC0: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_freed = 0, g_pushed = 0; static long g_seen[4]; static void* g_seenFirst = 0;
struct CRetailVector_00befbc0 {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CRetailVector_00befbc0(const CRetailVector_00befbc0& other) { m_pFirst = std::malloc(16); std::memcpy(m_pFirst, other.m_pFirst, 16); m_pLast = (char*)m_pFirst + 16; m_pEnd = m_pLast; }
    ~CRetailVector_00befbc0() { if (m_pFirst) { ++g_freed; std::free(m_pFirst); } }
};
struct SKey_00befbc0 { long m_a; long m_b; };
struct SValue_00befbc0 { long m_c; long m_d; CRetailVector_00befbc0 m_items; SValue_00befbc0(const SValue_00befbc0& other) : m_c(other.m_c), m_d(other.m_d), m_items(other.m_items) {} };
struct SEntry_00befbc0 { SKey_00befbc0 first; SValue_00befbc0 second; SEntry_00befbc0(const SKey_00befbc0& key, const SValue_00befbc0& value) : first(key), second(value) {} };
struct CEntryTable_00befbc0 {
    unsigned char _pad[0xd]; bool m_dirty;
    void Push(const SEntry_00befbc0& entry) { ++g_pushed; g_seen[0] = entry.first.m_a; g_seen[1] = entry.first.m_b; g_seen[2] = entry.second.m_c; g_seen[3] = entry.second.m_d; g_seenFirst = entry.second.m_items.m_pFirst; }
    void AddEntry(const SKey_00befbc0& key, const SValue_00befbc0& value) { { SEntry_00befbc0 entry(key, value); Push(entry); } m_dirty = true; }
};
static bool Run()
{
    long raw[4] = { 7, 8, 9, 10 };
    CRetailVector_00befbc0* source = (CRetailVector_00befbc0*)std::malloc(sizeof(CRetailVector_00befbc0));
    source->m_pFirst = raw; source->m_pLast = raw + 4; source->m_pEnd = raw + 4;
    SValue_00befbc0* value = (SValue_00befbc0*)std::malloc(sizeof(SValue_00befbc0));
    value->m_c = 3; value->m_d = 4; std::memcpy(&value->m_items, source, sizeof(*source));
    SKey_00befbc0 key; key.m_a = 1; key.m_b = 2;
    CEntryTable_00befbc0 table; table.m_dirty = false;
    table.AddEntry(key, *value);
    bool ok = g_pushed == 1 && g_seen[0] == 1 && g_seen[1] == 2 && g_seen[2] == 3 && g_seen[3] == 4 && g_seenFirst != raw && g_freed == 1 && table.m_dirty;
    std::free(value); std::free(source);
    return ok;
}
int main()
{
    if (!Run()) { std::printf("00befbc0_TEST FAIL\n"); return 1; }
    std::printf("00befbc0_TEST PASS\n");
    return 0;
}
