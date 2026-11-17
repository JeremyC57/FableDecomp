// Behaviour test for the bank sample record reader's OnRecord (0x00C313C0): the entry for the record's key is found or
// created, the name / two shorts copied, every optional field the present-mask marks is taken (absent ones get their
// defaults: priority 100, volume 0x7f, count 1, loop -1 -> 1) and the derived flag bits set.
#include <cstdio>
#include <cstring>
#include <string>

struct Entry { void* m_pKey; long m_4; std::string m_name; short m_c, m_e; long m_10, m_14; short m_18, m_1a, m_1c, m_1e;
               unsigned short m_flags; short m_22; long m_24, m_28, m_2c; };
struct Map {
    Entry e[4]; int n; int creates;
    Entry* Find(void* const* key) { for (int i = 0; i < n; ++i) if (e[i].m_pKey == *key) return &e[i]; return 0; }
    Entry* CreateEntry(void* const* key) { ++creates; e[n] = Entry(); e[n].m_pKey = *key; return &e[n++]; }
    Entry* Get(void* const* key) { if (Entry* x = Find(key)) return x; return CreateEntry(key); }
};
struct Record {
    void* m_key; long m_108; short m_118, m_11a; char m_name[32]; long m_240; unsigned long m_present; long m_248;
    short m_250, m_254; unsigned char m_258; short m_25c, m_260, m_264; long m_268, m_26c, m_274, m_278, m_27c;
};
static void OnRecord(Map* map, const Record* record)
{
    void* key = record->m_key;
    Entry* entry = map->Get(&key);
    entry->m_flags = 0;
    entry->m_4 = record->m_108;
    entry->m_name = record->m_name;
    entry->m_c = record->m_11a;
    entry->m_e = record->m_118;
    if (record->m_278 != 0) entry->m_flags |= 4;
    if (!(record->m_present & 0x400) || record->m_274 != 2) entry->m_flags |= 8;
    entry->m_14 = 0;
    if (record->m_present & 0x40) entry->m_14 = record->m_248;
    entry->m_28 = 0;
    if (record->m_present & 0x100) { entry->m_28 = record->m_26c; entry->m_flags |= 0x20; }
    entry->m_24 = 0;
    if (record->m_present & 0x80) { entry->m_24 = record->m_268; entry->m_flags |= 0x10; }
    entry->m_1e = 100;
    if (record->m_present & 1) entry->m_1e = record->m_260;
    entry->m_22 = record->m_264;
    entry->m_10 = 1;
    if (record->m_240 != 0) entry->m_10 = record->m_240;
    entry->m_2c = 1;
    if (record->m_27c != -1) entry->m_2c = record->m_27c;
    entry->m_1c = 0x7f;
    if (record->m_present & 0x20) entry->m_1c = record->m_25c;
    entry->m_1a = 0;
    if (record->m_present & 8) entry->m_1a = record->m_254;
    entry->m_18 = 0;
    if (record->m_present & 4) entry->m_18 = record->m_250;
    if (record->m_present & 0x10) {
        if (record->m_258 & 1) entry->m_flags |= 1;
        if (record->m_258 & 2) entry->m_flags |= 2;
    }
}

int main()
{
    Map m; m.n = 0; m.creates = 0;
    Record r; std::memset(&r, 0, sizeof r);
    r.m_key = (void*)0x10; r.m_108 = 7; r.m_118 = 1; r.m_11a = 2; std::strcpy(r.m_name, "drum"); r.m_27c = -1; r.m_264 = 9;
    OnRecord(&m, &r);
    Entry* e = &m.e[0];
    bool ok = m.creates == 1 && e->m_4 == 7 && e->m_name == "drum" && e->m_c == 2 && e->m_e == 1 && e->m_flags == 8
           && e->m_1e == 100 && e->m_1c == 0x7f && e->m_10 == 1 && e->m_2c == 1 && e->m_22 == 9 && e->m_14 == 0 && e->m_24 == 0;
    r.m_present = 0x400 | 0x40 | 0x100 | 0x80 | 1 | 0x20 | 8 | 4 | 0x10; r.m_274 = 2; r.m_278 = 1; r.m_258 = 3;
    r.m_248 = 11; r.m_26c = 12; r.m_268 = 13; r.m_260 = 14; r.m_25c = 15; r.m_254 = 16; r.m_250 = 17; r.m_240 = 18; r.m_27c = 19;
    OnRecord(&m, &r);
    ok = ok && m.creates == 1 && e->m_flags == (4 | 0x20 | 0x10 | 1 | 2) && e->m_14 == 11 && e->m_28 == 12 && e->m_24 == 13
            && e->m_1e == 14 && e->m_1c == 15 && e->m_1a == 16 && e->m_18 == 17 && e->m_10 == 18 && e->m_2c == 19;
    if (!ok) { std::printf("00c313c0_TEST FAIL\n"); return 1; }
    std::printf("00c313c0_TEST PASS\n");
    return 0;
}