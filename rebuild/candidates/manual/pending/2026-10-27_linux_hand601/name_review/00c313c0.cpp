// CSampleTableReader::OnRecord(void* context, const SSampleTableRecord* record) -- copies one "LHAudioBankSampleTable" record (the 652-byte sample directory entry) into the bank's sample map entry for its id (finding it or creating it), unpacking the playback-parameter flags into the entry
// Leaf: ?OnRecord@CSampleTableReader@@UAEXPAXPBUSSampleTableRecord@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C313C0, 412 bytes, thiscall `ret 8`; the only slot of the one-slot vtable 0x012B4D34
//   (stored by the reader's inline constructor 0x00C311C0, which also stores the bank at +4, and by the bank loader at
//   0x00C31142, which registers it second, between the "LHAudioWaveData" reader 0x00C31230 [vtable 0x012B4D3C] and the
//   array-record reader 0x00C31660 [vtable 0x012B4D40]). Descriptive names (libpk has no PDB or RTTI). The record layout
//   is docs/formats/AUDIO.md B.2.4 (id +260, waveId +264, the u32 at +280 split into halves, group +320, playback
//   parameters +576..+651): every field read here falls on that layout. The first argument is never read.
// Callees: the sample map's slot-17 Find (key by address), CAutoMapOwner_00c31560::CreateEntry 0x00C31560 (landed; the
//   0x30-byte entry), CString::operator=(const char*) 0x00C0C200.
// Shape: twin of the landed CWaveDataReader::OnRecord -- the key local is homed in the dead record argument slot and the
//   find-or-create is an inline map member with an early return. In the two flag-plus-value blocks the value is
//   assigned before the flag bit (`m_field28 = v; m_flags |= 0x20`): flag first puts the `or` ahead of the load.
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
