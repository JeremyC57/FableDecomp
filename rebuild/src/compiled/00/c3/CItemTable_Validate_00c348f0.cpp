// CItemTable::Validate() -- walks every row and every column slot of the table (row count = virtual slot 5, column count of a row = slot 4) with an empty body: the per-entry check is compiled out of the release build
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C348F0, 61 bytes, __fastcall `ret`. Descriptive name (no PDB covers this
//   library): the catalog's `VectorLike_IterateNested` is a placeholder. It is slot 9 of the abstract table base
//   (vtable 0x012B3444: slots 0, 3-8 `_purecall`, 1/2 `return -1`, 9 this, 10 Find 0x00C34930, 11 the deleting
//   destructor 0x00C1EDE0) and is inherited unchanged by the four derived tables (vtables 0x012B4FBC, 0x012B4FEC,
//   0x012B501C, 0x012B504C).
// Shape: two rotated `for` loops over unsigned counters (row ebx, column edi) whose conditions re-call the virtual
//   counts every pass.
struct CTableEntry;

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

void CItemTable::Validate()
{
    for (unsigned long row = 0; row < GetRowCount(); ++row)
        for (unsigned long column = 0; column < GetColumnCount(row); ++column) {
        }
}