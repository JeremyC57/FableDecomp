// CDataBank::GetTextInfo(unsigned long index) -- the text-bank entry record for a bank index, or null when the entry's data type is not the text type (0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7D00, length 37 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) @ 0x009CC210 (on `this`: CDataBank derives from CBankFile).
// Shape: the index is kept in edi across the call (used again to index the entry table); the type test is
// `test eax,eax ; jne` and the success path is the `if` body (fall-through), the null return sits after it. The entry table is the first word
// (m_pFirst) of the CArray<CDataBankEntry*> Entries member, at +0x1e0 in retail (PDB +0x254: every CArray/map in
// the shipped CBankFile/CDataBank is smaller than in Ego_r.pdb, so the PDB header cannot be included).
struct CDataBankEntry;

template <class T> struct CArray {      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CDataBank {                      // CBankFile base (0x90 bytes in retail) + the text-bank state
    void*                   __vftable;               // +0x000
    unsigned char           _bankFile[0x1e0 - 4];    // +0x004 .. +0x1e0: CBankFile state and the data-bank buffers
    CArray<CDataBankEntry*> Entries;                 // +0x1e0 (PDB name)
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210 (CBankFile)
    CDataBankEntry* GetTextInfo(unsigned long index) const;
};

CDataBankEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
        return Entries[index];
    return 0;
}
