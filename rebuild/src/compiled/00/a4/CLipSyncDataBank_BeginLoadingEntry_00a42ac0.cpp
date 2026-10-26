// CLipSyncDataBank::BeginLoadingEntry(unsigned long index) -- when the bank entry exists and its record (8-byte entries at +0x164) has no object yet, start the record's asynchronous load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42AC0, length 42 bytes.
// Callees: CBankFile::EntryExists(unsigned long) @ 0x009CC190 (on `this`), CLipSyncDataBankEntry::BeginLoading(unsigned long, CLipSyncDataBank*) @ 0x00A42980
// (on the record: `lea ecx,[eax+edi*8]`, arguments pushed right to left: the bank, then the index). Shape: the record address is formed once for the
// null test and the call (a reference local); the index stays in edi. The chunk label "CEditWorldMap::IsMapChangeable" is propagated (the callees are
// the lip-sync bank's); the entries live at +0x164 of the retail layout (first word of a CArray).
class CLipSyncDataBank;

class CLipSyncDataBankEntry { public:                     // 8 bytes: the counted object pointer and a word
    void* m_pObject;
    long  m_state;
    void BeginLoading(unsigned long index, CLipSyncDataBank* bank);   // 0x00A42980
};

class CLipSyncDataBank { public:
    void*                  __vftable;              // +0x000
    unsigned char          _bankFile[0x164 - 4];   // +0x004 .. +0x164: CBankFile and the bank state
    CLipSyncDataBankEntry* m_pEntries;             // +0x164 CArray<CLipSyncDataBankEntry>::m_pFirst
    bool EntryExists(unsigned long index) const;   // 0x009CC190 (CBankFile)
    void BeginLoadingEntry(unsigned long index);
};

void CLipSyncDataBank::BeginLoadingEntry(unsigned long index)
{
    if (EntryExists(index)) {
        CLipSyncDataBankEntry& entry = m_pEntries[index];
        if (entry.m_pObject == 0)
            entry.BeginLoading(index, this);
    }
}