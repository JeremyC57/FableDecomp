// CDataBankTextEntry::IsLoaded() const -- true when the text entry object exists and either has no pending asynchronous bank read or that read has finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7BE0, length 30 bytes.
// Callees: CBankFileAsyncData::IsFinished() @ 0x009D54E0 (bool, `this` = the async record at +0x38 of the text entry).
// Shape: PText's object pointer (+0) null -> false (`xor al,al`); its async-data pointer at +0x38 null -> true
// (`mov al,1`); otherwise the IsFinished result is tested and both true paths share the `mov al,1` tail.
// PDB: CDataBankTextEntry { CIVCountedPointer<NGameText::CTextEntry> PText +0, long NarratorIndex +4 }.
class CBankFileAsyncData { public:
    bool IsFinished() const;             // 0x009D54E0
};

struct CTextEntry {
    unsigned char        _pad[0x38];
    CBankFileAsyncData*  m_pAsyncData;   // +0x38 the pending bank read, null once consumed
};

struct CDataBankTextEntry {
    CTextEntry* PText;                   // +0x00 CIVCountedPointer<CTextEntry>::m_p
    long        NarratorIndex;           // +0x04
    bool IsLoaded() const;
};

bool CDataBankTextEntry::IsLoaded() const
{
    if (PText != 0) {
        if (PText->m_pAsyncData == 0 || PText->m_pAsyncData->IsFinished())
            return true;
    }
    return false;
}