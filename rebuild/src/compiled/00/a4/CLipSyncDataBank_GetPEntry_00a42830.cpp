// CLipSyncDataBank::GetPEntry(unsigned long index) -- the bank's 8-byte entry record for `index` (the CArray at +0x164) when the bank entry exists, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42830, length 39 bytes.
// Callees: CBankFile::EntryExists(unsigned long) @ 0x009CC190 (landed, on `this`). Layout as the landed CLipSyncDataBank::BeginLoadingEntry @ 0x00A42AC0 (the entries' first pointer
// at +0x164). Shape: the index kept in edi across the call, `lea eax,[eax+edi*8]`, the null result after `pop edi`, `ret 4`. The chunk label "CGuiInputProcess::AddToInterface" is propagated.
class CLipSyncDataBankEntry { public:                     // 8 bytes: the counted object pointer and a word
    void* m_pObject;
    long  m_state;
};

class CLipSyncDataBank { public:
    unsigned char          _bankFile[0x164];       // +0x000 .. +0x164: CBankFile and the bank state
    CLipSyncDataBankEntry* m_pEntries;             // +0x164 CArray<CLipSyncDataBankEntry>::m_pFirst
    bool EntryExists(unsigned long index) const;   // 0x009CC190 (CBankFile)
    CLipSyncDataBankEntry* GetPEntry(unsigned long index);
};

CLipSyncDataBankEntry* CLipSyncDataBank::GetPEntry(unsigned long index)
{
    if (EntryExists(index))
        return &m_pEntries[index];
    return 0;
}