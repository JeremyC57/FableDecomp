// Behaviour test for 00c3b900: SWaveEntry constructor -- key stored, name empty (shared empty data), all fields zero
#include <cstdio>
static char g_nil[4] = { 0 };
static char* _afxPchNil = g_nil;
class CString { public: char* m_pchData; CString() : m_pchData(_afxPchNil) {} };
struct SWaveEntry {
    void* m_key; CString m_name; unsigned long m_size; unsigned long m_offset; unsigned short m_field10; unsigned short m_field12;
    unsigned long m_field14; unsigned long m_field18; unsigned long m_field1C;
    SWaveEntry(void* key) : m_key(key), m_name(), m_size(0), m_offset(0), m_field10(0), m_field12(0), m_field14(0), m_field18(0), m_field1C(0) {}
};
int main()
{
    int k; SWaveEntry* e = new SWaveEntry(&k);
    bool ok = e->m_key == &k && e->m_name.m_pchData == g_nil && e->m_size == 0 && e->m_offset == 0 && e->m_field10 == 0 &&
              e->m_field12 == 0 && e->m_field14 == 0 && e->m_field18 == 0 && e->m_field1C == 0 && sizeof(SWaveEntry) == 0x20;
    delete e;
    if (!ok) { std::printf("00c3b900_TEST FAIL\n"); return 1; }
    std::printf("00c3b900_TEST PASS\n");
    return 0;
}
