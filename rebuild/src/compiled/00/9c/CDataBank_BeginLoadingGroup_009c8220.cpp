// CDataBank::BeginLoadingGroup(unsigned long index) -- start the asynchronous load of a group entry: only when the bank entry's data type is the group type (1) and the entry record exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8220, length 42 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210 (on `this`), CDataBankGroupEntry::BeginLoading(unsigned long, CDataBank*) @ 0x009C80F0
// (ecx = the entry record, arguments pushed right to left: the bank, then the index).
// Shape: the index stays in edi across the type call and indexes the Entries table (first word of the CArray at
// +0x1e0 in retail, PDB +0x254); `cmp eax,1 ; jne` is the type test, the null test of the record follows, and
// the two pushes `push esi ; push edi` are the right-to-left argument order. The manifest label "BeginLoadingText"
// is propagated from the text sibling @ 0x009C8630 (type 0, callee 0x009C8340); this one loads a group entry.
struct CDataBank;

struct CDataBankGroupEntry {
    void BeginLoading(unsigned long index, CDataBank* bank);   // 0x009C80F0
};

template <class T> class CArray { public:      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CBankFile
{
public:
    unsigned long GetEntryDataType(unsigned long) const;   // 0x009CC210
};
class CBankFileAsync : public CBankFile
{
public:
};

struct CDataBank : public CBankFileAsync {                      // CBankFile base (0x90 bytes in retail) + the text-bank state
    void*                        __vftable;               // +0x000
    unsigned char                _bankFile[0x1e0 - 4];    // +0x004 .. +0x1e0
    CArray<CDataBankGroupEntry*> Entries;                 // +0x1e0 (PDB name; CDataBankEntry* in the PDB)
    void BeginLoadingGroup(unsigned long index);
};

void CDataBank::BeginLoadingGroup(unsigned long index)
{
    if (GetEntryDataType(index) == 1) {
        CDataBankGroupEntry* entry = Entries[index];
        if (entry)
            entry->BeginLoading(index, this);
    }
}