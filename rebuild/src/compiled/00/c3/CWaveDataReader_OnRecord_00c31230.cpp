// CWaveDataReader::OnRecord(void* context, const SWaveDataRecord* record) -- copies one "LHAudioWaveData" record into the wave map entry for its key (finding it or creating it)
// Leaf: OnRecord
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31230, 141 bytes, __fastcall `ret 8`; slot 3 of the vtable at 0x012B4D30,
//   which sits right after the "LHAudioWaveData" string. Descriptive name (no PDB covers this library): the catalog's
//   `CShaderResource_SetPixelShader` is propagated. The first argument is never read.
// Callees: the map's slot-17 Find (key by address), CAutoMapOwner::CreateEntry 0x00C312C0 (landed; the 0x20-byte entry
//   whose constructor 0x00C3B900 stores the key at +0 and the empty string at +4), CString::operator=(const char*)
//   0x00C0C200 (landed under the propagated name CShaderResource::GetPixelShader; it is AssignCopy(SafeStrlen(p), p)).
// Shape: the record pointer lives in edi, so the key local is homed in its dead argument slot; the find-or-create is an
//   inline map member with an early return (both results meet in eax before the move to esi; an `if (!e) e = Create`
//   body, inline or open-coded, tests esi instead and costs two bytes).
struct CString {
    char* m_pchData;
    CString& operator=(const char* text);   // 0x00C0C200
};

struct SWaveEntry {                         // 0x20 bytes
    void* m_key;                            // +0x00
    CString m_name;                         // +0x04
    unsigned long m_size;                   // +0x08
    unsigned long m_offset;                 // +0x0C
    unsigned short m_field10;               // +0x10
    unsigned short m_field12;               // +0x12
    unsigned long m_field14;                // +0x14
    unsigned long m_field18;                // +0x18
    unsigned long m_field1C;                // +0x1C
};

struct CWaveEntryMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual SWaveEntry* Find(void* const* key);     // slot 17
    SWaveEntry* CreateEntry(void* const* key);      // 0x00C312C0
    SWaveEntry* Get(void* const* key)               // inline find-or-create
    {
        if (SWaveEntry* entry = Find(key))
            return entry;
        return CreateEntry(key);
    }
};

struct SWaveDataRecord {
    char m_name[0x108];                     // +0x000
    void* m_key;                            // +0x108
    unsigned long m_size;                   // +0x10C
    unsigned long m_offset;                 // +0x110
    unsigned char _pad114[0x10];
    unsigned short m_field124;              // +0x124
    unsigned short m_field126;              // +0x126
    unsigned long m_field128;               // +0x128
    unsigned char _pad12C[0xC];
    unsigned long m_field138;               // +0x138
    unsigned long m_field13C;               // +0x13C
};

struct CWaveDataReader {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void OnRecord(void* context, const SWaveDataRecord* record);   // slot 3
    CWaveEntryMap* m_pMap;                  // +0x04
    unsigned long m_baseOffset;             // +0x08
};

void CWaveDataReader::OnRecord(void* context, const SWaveDataRecord* record)
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