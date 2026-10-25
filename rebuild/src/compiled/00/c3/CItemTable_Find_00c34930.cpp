// CItemTable::Find(long key, unsigned long* pRow, unsigned long* pColumn) -- scans the table row by row for the entry whose target (+4) carries the key at +4; on a hit the row/column out-words hold its position and the result is 1, otherwise 0 (the out-words end past the last slot)
// Leaf: Find
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34930, 124 bytes, __fastcall `ret 0xc`. Descriptive name (no PDB covers this
//   library): the catalog's `FindInGrid` is a placeholder. It is slot 10 of the abstract table base (vtable
//   0x012B3444, see Validate 0x00C348F0) and is inherited unchanged by the four derived tables.
// Shape: the caller's out-words ARE the loop counters (`mov [ebx],0 ; ... inc dword ptr [edi]`), re-read after every
//   virtual call; the key lives in ebp; the hit returns 1 through its own epilogue after the shared `xor eax,eax` one.
struct CTableTarget {
    unsigned char _pad00[4];
    long          m_key;                                                     // +0x04
};
struct CTableEntry {
    unsigned char _pad00[4];
    CTableTarget* m_pTarget;                                                 // +0x04
};

struct CItemTable {
    virtual unsigned long GetType() = 0;                                     // slot 0
    virtual long v01();                                                      // slot 1
    virtual long v02();                                                      // slot 2
    virtual long Insert(unsigned long row, CTableEntry* entry) = 0;          // slot 3
    virtual unsigned long GetColumnCount(unsigned long row) = 0;             // slot 4
    virtual unsigned long GetRowCount() = 0;                                 // slot 5
    virtual CTableEntry* GetAt(unsigned long row, unsigned long column) = 0; // slot 6
    virtual CTableEntry* GetByIndex(unsigned long index) = 0;                // slot 7
    virtual void Remove(unsigned long row, unsigned long column) = 0;        // slot 8
    virtual void Validate();                                                 // slot 9
    virtual long Find(long key, unsigned long* pRow, unsigned long* pColumn); // slot 10
    virtual ~CItemTable() { }                                                // slot 11
};

long CItemTable::Find(long key, unsigned long* pRow, unsigned long* pColumn)
{
    for (*pRow = 0; *pRow < GetRowCount(); ++*pRow)
        for (*pColumn = 0; *pColumn < GetColumnCount(*pRow); ++*pColumn)
            if (GetAt(*pRow, *pColumn)->m_pTarget->m_key == key)
                return 1;
    return 0;
}