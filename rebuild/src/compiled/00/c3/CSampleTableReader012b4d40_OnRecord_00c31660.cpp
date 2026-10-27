// CSampleTableReader_012b4d40::OnRecord(void* context, const char* name, unsigned long count, const unsigned long* values) -- copies one array record into the bank map entry for its name (finding it or creating it): the entry's word array is resized to count and filled from values
// Leaf: ?OnRecord@CSampleTableReader_012b4d40@@UAEXPAXPBDKPBK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31660, 168 bytes, thiscall `ret 0x10`, EH-framed (the key string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): the only slot of the one-slot vtable 0x012B4D40 (referenced by its
//   inline constructor at 0x00C31168 and by 0x00C31218), the third record reader the bank loader 0x00C30E90 registers
//   (through the array-record registration 0x00C3BCB0) next to the landed CWaveDataReader::OnRecord 0x00C31230
//   (vtable 0x012B4D3C, "LHAudioWaveData"). Same find-or-create shape; the first argument is never read. The catalog's
//   gapscan name is a placeholder.
// Callees: the libpk string's CDebugStream(const char*) 0x00C0BFE0 / ~CDebugStream 0x00C0BA40 (the chunk's trace rows'
//   spelling of 0x00C0BA40, one name per address; the catalog says CString); the map's slot-17 Find and the
//   landed CAutoMapOwner_00c31710::CreateEntry 0x00C31710 (12-byte entry: key +0, word array +4); the entry array's
//   Resize 0x00C31810 and operator[] 0x00C2EBC0 (bound as CAutoDeleteArray_00c3b7a0).
// Shape: the key string is homed in the dead `name` argument slot and destroyed before the resize; the find-or-create
//   is the inline early-return member the landed 0x00C31230 uses (both results meet in eax).
struct CDebugStream {                             // the libpk string (catalog: CString)
    char* m_pBuffer;
    CDebugStream(const char* text);               // 0x00C0BFE0
    ~CDebugStream();                              // 0x00C0BA40
};

struct CAutoDeleteArray_00c3b7a0 {                // owned word array: block, count
    unsigned long* m_pData;
    unsigned long  m_count;
    void Resize(unsigned long count);             // 0x00C31810
    unsigned long& operator[](unsigned long index);   // 0x00C2EBC0 (range-checked)
};

struct CEntry_00c31710 {                          // 12 bytes
    void*                     m_key;              // +0x00
    CAutoDeleteArray_00c3b7a0 m_values;           // +0x04
};

struct CAutoMapOwner_00c31710 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CEntry_00c31710* Find(void* key);     // slot 17
    CEntry_00c31710* CreateEntry(void* key);      // 0x00C31710
    CEntry_00c31710* Get(void* key)               // inline find-or-create
    {
        if (CEntry_00c31710* entry = Find(key))
            return entry;
        return CreateEntry(key);
    }
};

struct CSampleBank_012b4d40 {                     // the bank the reader fills
    unsigned char          _pad[0x20];
    CAutoMapOwner_00c31710 m_samples;             // +0x20
};

struct CSampleTableReader_012b4d40 {
    CSampleBank_012b4d40* m_pBank;                // +0x04
    virtual void OnRecord(void* context, const char* name, unsigned long count, const unsigned long* values);   // slot 0
};

void CSampleTableReader_012b4d40::OnRecord(void* context, const char* name, unsigned long count, const unsigned long* values)
{
    CEntry_00c31710* entry;
    {
        CDebugStream key(name);
        entry = m_pBank->m_samples.Get(&key);
    }
    entry->m_values.Resize(count);
    for (unsigned long i = 0; i < count; ++i)
        entry->m_values[i] = values[i];
}