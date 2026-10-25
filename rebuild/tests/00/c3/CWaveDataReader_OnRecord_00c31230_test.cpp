// Behaviour test for 00c31230: CWaveDataReader::OnRecord -- find-or-create the entry for the record key, then copy the fields
#include <cstdio>
#include <cstring>
struct CString {
    char m_text[64];
    CString& operator=(const char* text) { std::strcpy(m_text, text); return *this; }
};
struct SWaveEntry {
    void* m_key; CString m_name; unsigned int m_size; unsigned int m_offset;
    unsigned short m_field10, m_field12; unsigned int m_field14, m_field18, m_field1C;
};
struct CWaveEntryMap {
    SWaveEntry m_entries[4]; int m_count; int m_creates;
    CWaveEntryMap() : m_count(0), m_creates(0) { }
    SWaveEntry* Find(void* const* key) { for (int i = 0; i < m_count; ++i) if (m_entries[i].m_key == *key) return &m_entries[i]; return 0; }
    SWaveEntry* CreateEntry(void* const* key) { SWaveEntry* e = &m_entries[m_count++]; std::memset(e, 0, sizeof(*e)); e->m_key = *key; ++m_creates; return e; }
    SWaveEntry* Get(void* const* key) { if (SWaveEntry* entry = Find(key)) return entry; return CreateEntry(key); }
};
struct SWaveDataRecord {
    char m_name[0x108]; void* m_key; unsigned int m_size; unsigned int m_offset;
    unsigned short m_field124, m_field126; unsigned int m_field128, m_field138, m_field13C;
};
struct CWaveDataReader {
    CWaveEntryMap* m_pMap; unsigned int m_baseOffset;
    void OnRecord(void*, const SWaveDataRecord* record)
    {
        void* key = record->m_key;
        SWaveEntry* entry = m_pMap->Get(&key);
        entry->m_name = record->m_name;
        entry->m_size = record->m_size;
        entry->m_offset = record->m_offset + m_baseOffset;
        entry->m_field10 = record->m_field126;
        entry->m_field12 = record->m_field124;
        entry->m_field14 = record->m_field128;
        entry->m_field18 = record->m_field138;
        entry->m_field1C = record->m_field13C;
    }
};
int main()
{
    static int k1, k2;
    CWaveEntryMap map; CWaveDataReader r; r.m_pMap = &map; r.m_baseOffset = 100;
    SWaveDataRecord rec; std::memset(&rec, 0, sizeof(rec));
    std::strcpy(rec.m_name, "wave_a"); rec.m_key = &k1; rec.m_size = 7; rec.m_offset = 20;
    rec.m_field124 = 3; rec.m_field126 = 4; rec.m_field128 = 5; rec.m_field138 = 6; rec.m_field13C = 8;
    r.OnRecord(0, &rec);
    int ok = map.m_count == 1 && map.m_creates == 1;
    SWaveEntry* e = &map.m_entries[0];
    ok = ok && e->m_key == &k1 && std::strcmp(e->m_name.m_text, "wave_a") == 0 && e->m_size == 7 && e->m_offset == 120
        && e->m_field10 == 4 && e->m_field12 == 3 && e->m_field14 == 5 && e->m_field18 == 6 && e->m_field1C == 8;
    std::strcpy(rec.m_name, "wave_b"); rec.m_size = 9;
    r.OnRecord(0, &rec);                     // same key: the entry is reused
    ok = ok && map.m_count == 1 && std::strcmp(e->m_name.m_text, "wave_b") == 0 && e->m_size == 9;
    rec.m_key = &k2;
    r.OnRecord(0, &rec);                     // new key: a second entry
    ok = ok && map.m_count == 2 && map.m_entries[1].m_key == &k2;
    std::printf(ok ? "00c31230_TEST PASS\n" : "00c31230_TEST FAIL\n");
    return ok ? 0 : 1;
}