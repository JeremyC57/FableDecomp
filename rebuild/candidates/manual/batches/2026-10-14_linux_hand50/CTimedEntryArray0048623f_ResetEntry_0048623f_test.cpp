#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static unsigned long g_index = 99; static const void* g_entry = 0;
struct SEntry { unsigned char _body[0x1c]; SEntry() { std::memset(_body, 7, sizeof _body); std::strcat(g_seq, "C"); } };
struct CArray { SEntry* first; SEntry* last; SEntry* end; void Set(unsigned long index, const SEntry& entry) { std::strcat(g_seq, "S"); g_index = index; g_entry = &entry; if (entry._body[3] != 7) g_index = 1000; } };
struct CTimedEntryArray { CArray m_entries; void ResetEntry(unsigned long index) { m_entries.Set(index, SEntry()); } };
int main()
{
    CTimedEntryArray a; a.m_entries.first = a.m_entries.last = a.m_entries.end = 0; a.ResetEntry(5);
    if (std::strcmp(g_seq, "CS") || g_index != 5 || g_entry == 0) { std::printf("0048623f_TEST FAIL\n"); return 1; }
    std::printf("0048623f_TEST PASS\n");
    return 0;
}
