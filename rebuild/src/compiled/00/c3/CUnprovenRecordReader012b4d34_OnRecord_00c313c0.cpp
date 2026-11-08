// CUnprovenRecordReader_012b4d34::OnRecord(CLoadLog* log, const void* record) -- copies one bank sample record into the bank's sample map entry for its key (finding it or creating it): the name, the two shorts, and every optional field the record's flag word marks present (absent ones get their defaults), plus the derived entry flags
// Leaf: ?OnRecord@CUnprovenRecordReader_012b4d34@@UAEXPAVCLoadLog@@PBX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C313C0, 412 bytes, thiscall `ret 8`; the only slot of the one-slot vtable 0x012B4D34,
//   the second record reader the landed ReadBankData 0x00C30F60 hands to the table loader (spelling bound there). The
//   first argument is never read. Descriptive name (libpk has no PDB or RTTI).
// Callees: the map's slot-17 Find (key by address), CAutoMapOwner_00c31560::CreateEntry 0x00C31560 (landed; the 0x30-byte
//   entry of CAutoMapEntry_00c3baa0 0x00C3BAA0), CString::operator=(const char*) 0x00C0C200.
// Shape: the landed CWaveDataReader::OnRecord 0x00C31230 shape -- the key homed in the dead record argument slot, the
//   find-or-create an inline early-return member.
struct CString {
    char* m_pchData;
    CString& operator=(const char* text);           // 0x00C0C200
};

struct CAutoMapEntry_00c3baa0 {                     // 0x30 bytes
    void*          m_pKey;                          // +0x00
    long           m_4;                             // +0x04
    CString        m_name;                          // +0x08
    short          m_c;                             // +0x0c
    short          m_e;                             // +0x0e
    long           m_10;                            // +0x10
    long           m_14;                            // +0x14
    short          m_18;                            // +0x18
    short          m_1a;                            // +0x1a
    short          m_1c;                            // +0x1c
    short          m_1e;                            // +0x1e
    unsigned short m_flags;                         // +0x20
    short          m_22;                            // +0x22
    long           m_24;                            // +0x24
    long           m_28;                            // +0x28
    long           m_2c;                            // +0x2c
};

struct CAutoMapOwner_00c31560 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CAutoMapEntry_00c3baa0* Find(void* const* key);     // slot 17
    CAutoMapEntry_00c3baa0* CreateEntry(void* const* key);      // 0x00C31560
    CAutoMapEntry_00c3baa0* Get(void* const* key)               // inline find-or-create
    {
        if (CAutoMapEntry_00c3baa0* entry = Find(key))
            return entry;
        return CreateEntry(key);
    }
};

class CBankData {
public:
    unsigned char          m_waves[0x10];           // +0x00 (the wave map CWaveDataReader fills)
    CAutoMapOwner_00c31560 m_samples;               // +0x10
};

struct SBankSampleRecord_00c313c0 {                 // 0x280 bytes
    unsigned char  _pad000[0x104];
    void*          m_key;                           // +0x104
    long           m_108;                           // +0x108
    unsigned char  _pad10c[0xc];
    short          m_118;                           // +0x118
    short          m_11a;                           // +0x11a
    unsigned char  _pad11c[0x24];
    char           m_name[0x100];                   // +0x140
    long           m_240;                           // +0x240
    unsigned long  m_present;                       // +0x244 (which optional fields follow)
    long           m_248;                           // +0x248
    unsigned char  _pad24c[4];
    short          m_250;                           // +0x250
    unsigned char  _pad252[2];
    short          m_254;                           // +0x254
    unsigned char  _pad256[2];
    unsigned char  m_258;                           // +0x258
    unsigned char  _pad259[3];
    short          m_25c;                           // +0x25c
    unsigned char  _pad25e[2];
    short          m_260;                           // +0x260
    unsigned char  _pad262[2];
    short          m_264;                           // +0x264
    unsigned char  _pad266[2];
    long           m_268;                           // +0x268
    long           m_26c;                           // +0x26c
    unsigned char  _pad270[4];
    long           m_274;                           // +0x274
    long           m_278;                           // +0x278
    long           m_27c;                           // +0x27c
};

class CLoadLog;
struct Callback {
    virtual void OnRecord(CLoadLog* log, const void* record) = 0;
};
struct CUnprovenRecordReader_012b4d34 : Callback {  // vtable 0x012B4D34
    CBankData* m_pData;                             // +0x04
    virtual void OnRecord(CLoadLog* log, const void* record);
};

void CUnprovenRecordReader_012b4d34::OnRecord(CLoadLog* log, const void* data)
{
    const SBankSampleRecord_00c313c0* record = (const SBankSampleRecord_00c313c0*)data;
    void* key = record->m_key;
    CAutoMapEntry_00c3baa0* entry = m_pData->m_samples.Get(&key);
    entry->m_flags = 0;
    entry->m_4 = record->m_108;
    entry->m_name = record->m_name;
    entry->m_c = record->m_11a;
    entry->m_e = record->m_118;
    if (record->m_278 != 0)
        entry->m_flags |= 4;
    if (!(record->m_present & 0x400) || record->m_274 != 2)
        entry->m_flags |= 8;
    entry->m_14 = 0;
    if (record->m_present & 0x40)
        entry->m_14 = record->m_248;
    entry->m_28 = 0;
    if (record->m_present & 0x100) {
        entry->m_28 = record->m_26c;
        entry->m_flags |= 0x20;
    }
    entry->m_24 = 0;
    if (record->m_present & 0x80) {
        entry->m_24 = record->m_268;
        entry->m_flags |= 0x10;
    }
    entry->m_1e = 100;
    if (record->m_present & 1)
        entry->m_1e = record->m_260;
    entry->m_22 = record->m_264;
    entry->m_10 = 1;
    if (record->m_240 != 0)
        entry->m_10 = record->m_240;
    entry->m_2c = 1;
    if (record->m_27c != -1)
        entry->m_2c = record->m_27c;
    entry->m_1c = 0x7f;
    if (record->m_present & 0x20)
        entry->m_1c = record->m_25c;
    entry->m_1a = 0;
    if (record->m_present & 8)
        entry->m_1a = record->m_254;
    entry->m_18 = 0;
    if (record->m_present & 4)
        entry->m_18 = record->m_250;
    if (record->m_present & 0x10) {
        if (record->m_258 & 1)
            entry->m_flags |= 1;
        if (record->m_258 & 2)
            entry->m_flags |= 2;
    }
}