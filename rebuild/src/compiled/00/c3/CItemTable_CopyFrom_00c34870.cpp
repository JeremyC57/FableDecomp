// CItemTable::CopyFrom(CItemTable& source) -- inserts every entry of the source table into this one at the same row (this table's virtual slot 3, `Insert(row, entry)`) and returns this table
// Leaf: CopyFrom
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34870, 114 bytes, __fastcall `ret 4`. Descriptive name (no PDB covers this
//   library): the catalog's `CCreatureAction_ExecuteWeaponAnimations` is propagated. Caller: the clone path at
//   0x00C1F2FD (`clone->GetTable()->CopyFrom(*GetTable())`, the result dropped). A non-virtual member of the abstract
//   table base (vtable 0x012B3444; see Validate 0x00C348F0 / Find 0x00C34930).
// Shape: two rotated `for` loops over the source's virtual counts (row edi, column ebx); `this` is spilled to the
//   `push ecx` frame word and its vtable is fetched BEFORE the source's GetAt call (the nested
//   `Insert(row, source.GetAt(row, column))`) and parked in the dead argument slot; `return *this` comes back in ebp.
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
    CItemTable& CopyFrom(CItemTable& source);
};

CItemTable& CItemTable::CopyFrom(CItemTable& source)
{
    for (unsigned long row = 0; row < source.GetRowCount(); ++row)
        for (unsigned long column = 0; column < source.GetColumnCount(row); ++column)
            Insert(row, source.GetAt(row, column));
    return *this;
}