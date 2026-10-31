// Behaviour test for 0x00C313C0 CSampleTableReader::OnRecord: the reconstructed body verbatim over a fake sample map
// (slot-17 Find over a small table, CreateEntry appends) and a minimal CString::operator=.
#include <cstdio>
#include <cstring>
struct CString {
    char* m_pchData;
    CString& operator=(const char* text);   // 0x00C0C200
};

struct CEntry_00c31560 {                    // 0x30 bytes: one sample of the bank
    void* m_key;                            // +0x00 sample id (stored by the constructor 0x00C3BAA0)
    unsigned long m_waveId;                 // +0x04
    CString m_group;                        // +0x08
    unsigned short m_field0C;               // +0x0C
    unsigned short m_field0E;               // +0x0E
    unsigned long m_priority;               // +0x10
    unsigned long m_field14;                // +0x14
    unsigned short m_field18;               // +0x18
    unsigned short m_field1A;               // +0x1A
    unsigned short m_field1C;               // +0x1C
    unsigned short m_field1E;               // +0x1E
    unsigned short m_flags;                 // +0x20
    unsigned short m_field22;               // +0x22
    unsigned long m_field24;                // +0x24
    unsigned long m_field28;                // +0x28
    unsigned long m_field2C;                // +0x2C
};

struct CAutoMapOwner_00c31560 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CEntry_00c31560* Find(void* const* key);    // slot 17
    CEntry_00c31560* CreateEntry(void* const* key);     // 0x00C31560
    CEntry_00c31560* Get(void* const* key)              // inline find-or-create
    {
        if (CEntry_00c31560* entry = Find(key))
            return entry;
        return CreateEntry(key);
    }
};

struct CSampleBank {
    unsigned char _pad00[0x10];
    CAutoMapOwner_00c31560 m_samples;       // +0x10
};

struct SSampleTableRecord {                 // 652 bytes (AUDIO.md B.2.4)
    char m_sourcePath[0x104];               // +0x000
    void* m_id;                             // +0x104
    unsigned long m_waveId;                 // +0x108
    unsigned long m_riffSize;               // +0x10C
    unsigned long m_riffOffset;             // +0x110
    unsigned long m_unk0;                   // +0x114
    unsigned short m_unk1Low;               // +0x118
    unsigned short m_unk1High;              // +0x11A
    unsigned char _pad11C[0x24];
    char m_group[0x100];                    // +0x140
    unsigned long m_priority;               // +0x240
    unsigned long m_paramFlags;             // +0x244
    unsigned long m_param248;               // +0x248
    unsigned long _pad24C;                  // +0x24C
    unsigned long m_param250;               // +0x250
    unsigned long m_param254;               // +0x254
    unsigned long m_param258;               // +0x258
    unsigned long m_param25C;               // +0x25C
    unsigned long m_param260;               // +0x260
    unsigned long m_param264;               // +0x264
    unsigned long m_param268;               // +0x268
    unsigned long m_param26C;               // +0x26C
    unsigned long _pad270;                  // +0x270
    unsigned long m_param274;               // +0x274
    unsigned long m_param278;               // +0x278
    unsigned long m_param27C;               // +0x27C
};

struct CSampleTableReader {
    virtual void OnRecord(void* context, const SSampleTableRecord* record);   // slot 0
    CSampleBank* m_pBank;                   // +0x04
};

void CSampleTableReader::OnRecord(void* context, const SSampleTableRecord* record)
{
    void* key = record->m_id;
    CEntry_00c31560* entry = m_pBank->m_samples.Get(&key);
    entry->m_flags = 0;
    entry->m_waveId = record->m_waveId;
    entry->m_group = record->m_group;
    entry->m_field0C = record->m_unk1High;
    entry->m_field0E = record->m_unk1Low;
    if (record->m_param278 != 0)
        entry->m_flags |= 4;
    if (!(record->m_paramFlags & 0x400) || record->m_param274 != 2)
        entry->m_flags |= 8;
    entry->m_field14 = 0;
    if (record->m_paramFlags & 0x40)
        entry->m_field14 = record->m_param248;
    entry->m_field28 = 0;
    if (record->m_paramFlags & 0x100) {
        entry->m_field28 = record->m_param26C;
        entry->m_flags |= 0x20;
    }
    entry->m_field24 = 0;
    if (record->m_paramFlags & 0x80) {
        entry->m_field24 = record->m_param268;
        entry->m_flags |= 0x10;
    }
    entry->m_field1E = 100;
    if (record->m_paramFlags & 1)
        entry->m_field1E = (unsigned short)record->m_param260;
    entry->m_field22 = (unsigned short)record->m_param264;
    entry->m_priority = 1;
    if (record->m_priority != 0)
        entry->m_priority = record->m_priority;
    entry->m_field2C = 1;
    if (record->m_param27C != (unsigned long)-1)
        entry->m_field2C = record->m_param27C;
    entry->m_field1C = 0x7f;
    if (record->m_paramFlags & 0x20)
        entry->m_field1C = (unsigned short)record->m_param25C;
    entry->m_field1A = 0;
    if (record->m_paramFlags & 8)
        entry->m_field1A = (unsigned short)record->m_param254;
    entry->m_field18 = 0;
    if (record->m_paramFlags & 4)
        entry->m_field18 = (unsigned short)record->m_param250;
    if (record->m_paramFlags & 0x10) {
        if (record->m_param258 & 1)
            entry->m_flags |= 1;
        if (record->m_param258 & 2)
            entry->m_flags |= 2;
    }
}

static char g_strings[8][256];
static int g_nstrings = 0;
CString& CString::operator=(const char* text)
{
    m_pchData = g_strings[g_nstrings++];
    std::strcpy(m_pchData, text);
    return *this;
}
static CEntry_00c31560 g_entries[4];
static int g_nentries = 0, g_creates = 0;
void CAutoMapOwner_00c31560::v00() {} void CAutoMapOwner_00c31560::v01() {} void CAutoMapOwner_00c31560::v02() {}
void CAutoMapOwner_00c31560::v03() {} void CAutoMapOwner_00c31560::v04() {} void CAutoMapOwner_00c31560::v05() {}
void CAutoMapOwner_00c31560::v06() {} void CAutoMapOwner_00c31560::v07() {} void CAutoMapOwner_00c31560::v08() {}
void CAutoMapOwner_00c31560::v09() {} void CAutoMapOwner_00c31560::v10() {} void CAutoMapOwner_00c31560::v11() {}
void CAutoMapOwner_00c31560::v12() {} void CAutoMapOwner_00c31560::v13() {} void CAutoMapOwner_00c31560::v14() {}
void CAutoMapOwner_00c31560::v15() {} void CAutoMapOwner_00c31560::v16() {}
CEntry_00c31560* CAutoMapOwner_00c31560::Find(void* const* key)
{
    for (int i = 0; i < g_nentries; i++)
        if (g_entries[i].m_key == *key) return &g_entries[i];
    return 0;
}
CEntry_00c31560* CAutoMapOwner_00c31560::CreateEntry(void* const* key)
{
    g_creates++;
    CEntry_00c31560* e = &g_entries[g_nentries++];
    std::memset(e, 0xAB, sizeof(*e));
    e->m_key = *key;
    return e;
}

static int fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); fail++; } } while (0)

int main()
{
    CSampleBank bank;
    CSampleTableReader reader;
    reader.m_pBank = &bank;
    static SSampleTableRecord r;
    std::memset(&r, 0, sizeof(r));
    r.m_id = (void*)7; r.m_waveId = 33; r.m_unk1Low = 0x1111; r.m_unk1High = 0x2222;
    std::strcpy(r.m_group, "Balverine");
    r.m_priority = 300; r.m_paramFlags = 0x400 | 0x40 | 0x100 | 0x80 | 1 | 0x20 | 8 | 4 | 0x10;
    r.m_param248 = 0x248; r.m_param26C = 0x26C; r.m_param268 = 0x268; r.m_param260 = 0x10060; r.m_param264 = 0x64;
    r.m_param27C = 4000; r.m_param25C = 0x5C; r.m_param254 = 0x54; r.m_param250 = 0x50; r.m_param258 = 3;
    r.m_param274 = 2; r.m_param278 = 1;
    reader.OnRecord(0, &r);
    CHECK(g_creates == 1 && g_nentries == 1);
    CEntry_00c31560* e = &g_entries[0];
    CHECK(e->m_key == (void*)7 && e->m_waveId == 33 && std::strcmp(e->m_group.m_pchData, "Balverine") == 0);
    CHECK(e->m_field0C == 0x2222 && e->m_field0E == 0x1111);
    CHECK(e->m_flags == (4 | 0x20 | 0x10 | 1 | 2));        // 0x400 set and 274 == 2: no 8
    CHECK(e->m_field14 == 0x248 && e->m_field28 == 0x26C && e->m_field24 == 0x268);
    CHECK(e->m_field1E == 0x60 && e->m_field22 == 0x64 && e->m_priority == 300 && e->m_field2C == 4000);
    CHECK(e->m_field1C == 0x5C && e->m_field1A == 0x54 && e->m_field18 == 0x50);
    // same id again with no optional flags: entry is reused and every field takes its default
    std::memset(&r, 0, sizeof(r));
    r.m_id = (void*)7; r.m_waveId = 34; r.m_param27C = (unsigned long)-1; r.m_param264 = 9;
    std::strcpy(r.m_group, "Arena");
    reader.OnRecord(0, &r);
    CHECK(g_creates == 1 && g_nentries == 1);
    CHECK(e->m_waveId == 34 && std::strcmp(e->m_group.m_pchData, "Arena") == 0);
    CHECK(e->m_flags == 8);                                // 0x400 clear: 8
    CHECK(e->m_field14 == 0 && e->m_field28 == 0 && e->m_field24 == 0);
    CHECK(e->m_field1E == 100 && e->m_field22 == 9 && e->m_priority == 1 && e->m_field2C == 1);
    CHECK(e->m_field1C == 0x7f && e->m_field1A == 0 && e->m_field18 == 0);
    // a new id creates a second entry; 0x400 set but 274 != 2 still sets 8; 0x10 with 258 == 0 adds nothing
    r.m_id = (void*)8; r.m_paramFlags = 0x400 | 0x10; r.m_param274 = 3;
    reader.OnRecord(0, &r);
    CHECK(g_creates == 2 && g_nentries == 2 && g_entries[1].m_key == (void*)8 && g_entries[1].m_flags == 8);
    if (fail) { std::printf("00c313c0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c313c0_TEST PASS\n");
    return 0;
}
