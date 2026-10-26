// CDataBank::BeginLoadingText(unsigned long index) -- start the asynchronous load of a text entry: only when the bank entry's data type is the text type (0) and the entry record exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8630, length 42 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210 (on `this`), CDataBankTextEntry::BeginLoading(unsigned long, CDataBank*) @ 0x009C8340
// (ecx = the entry record, arguments pushed right to left: the bank, then the index).
// Shape: the index stays in edi across the type call and indexes the Entries table (first word of the CArray at
// +0x1e0 in retail, PDB +0x254); `test eax,eax ; jne` is the type-0 test, then the null test of the record and
// the two pushes `push esi ; push edi`. The group sibling @ 0x009C8220 tests for type 1 and calls 0x009C80F0.
struct CDataBank;

struct CDataBankTextEntry {
    void BeginLoading(unsigned long index, CDataBank* bank);    // 0x009C8340
};

template <class T> class CArray { public:      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CDataBank {                      // CBankFile base (0x90 bytes in retail) + the text-bank state
    void*                       __vftable;               // +0x000
    unsigned char               _bankFile[0x1e0 - 4];    // +0x004 .. +0x1e0
    CArray<CDataBankTextEntry*> Entries;                 // +0x1e0 (PDB name; CDataBankEntry* in the PDB)
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210 (CBankFile)
    void BeginLoadingText(unsigned long index);
};

void CDataBank::BeginLoadingText(unsigned long index)
{
    if (GetEntryDataType(index) == 0) {
        CDataBankTextEntry* entry = Entries[index];
        if (entry)
            entry->BeginLoading(index, this);
    }
}