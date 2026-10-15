// Behaviour test for 0x009C95E0 CDataBank::GetTextBySymbol: a symbol whose entry exists, is plain text and loads returns the
// entry's text and drops the reference taken by the lookup; anything else returns the empty string.
#include <cstdio>
#include <cstring>
static int g_destroyed = 0;
struct CGroupEntry {
    long m_refCount; const char* m_text;
    void Destroy() { ++g_destroyed; }
};
struct CEntryReference {
    CGroupEntry* m_pEntry;
    CEntryReference(CGroupEntry* e) : m_pEntry(e) {}
    ~CEntryReference() { if (--m_pEntry->m_refCount == 0) m_pEntry->Destroy(); }
};
struct CDataBank {
    unsigned int m_index; unsigned int m_type; CGroupEntry* m_entry;
    unsigned int LookupValue(const char* symbol) const { return std::strcmp(symbol, "TEXT_HELLO") == 0 ? m_index : 0; }
    unsigned int GetEntryDataType(unsigned int) const { return m_type; }
    CGroupEntry* GetPGroupEntry(unsigned int, bool) const { if (m_entry) ++m_entry->m_refCount; return m_entry; }
    const char* GetTextBySymbol(const char* symbol) const
    {
        unsigned int index = LookupValue(symbol);
        if (index > 0 && GetEntryDataType(index) == 0) {
            CGroupEntry* entry = GetPGroupEntry(index, true);
            if (entry) {
                CEntryReference reference(entry);
                return entry->m_text;
            }
        }
        return "";
    }
};
int main()
{
    CGroupEntry e; e.m_refCount = 0; e.m_text = "Hello";
    CDataBank bank; bank.m_index = 5; bank.m_type = 0; bank.m_entry = &e;
    if (std::strcmp(bank.GetTextBySymbol("TEXT_HELLO"), "Hello") || e.m_refCount != 0 || g_destroyed != 1) { std::printf("009c95e0_TEST FAIL\n"); return 1; }
    e.m_refCount = 1;
    if (std::strcmp(bank.GetTextBySymbol("TEXT_HELLO"), "Hello") || e.m_refCount != 1 || g_destroyed != 1) { std::printf("009c95e0_TEST FAIL\n"); return 1; }
    if (std::strcmp(bank.GetTextBySymbol("TEXT_OTHER"), "")) { std::printf("009c95e0_TEST FAIL\n"); return 1; }
    bank.m_type = 2;
    if (std::strcmp(bank.GetTextBySymbol("TEXT_HELLO"), "")) { std::printf("009c95e0_TEST FAIL\n"); return 1; }
    bank.m_type = 0; bank.m_entry = 0;
    if (std::strcmp(bank.GetTextBySymbol("TEXT_HELLO"), "")) { std::printf("009c95e0_TEST FAIL\n"); return 1; }
    std::printf("009c95e0_TEST PASS\n");
    return 0;
}