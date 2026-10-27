// CItemGridTable_012b501c::GetAt(unsigned long row, unsigned long column) -- slot 6 of the two-level item table: the row's entry list (rows at +4), then its column-th entry
// Leaf: ?GetAt@CItemGridTable_012b501c@@UAEPAUCTableEntry@@KK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34E20, 23 bytes, thiscall `ret 8`.
// Identity: slot 6 of vtable 0x012B501C (one of the four tables derived from the abstract CItemTable 0x012B3444 whose
//   slot layout the landed Validate 0x00C348F0 / Find 0x00C34930 record: slot 6 GetAt(row, column)). Descriptive class
//   name (libpk: no PDB/RTTI). Its rows are the pointer array at +4 (CPtrArray<CPlaybackEntry*>, landed Remove
//   0x00C35080); every row is itself a CPtrArray<CTableEntry*> (landed destructor 0x00C42AD0).
// Callee: the row's out-of-line element read 0x00C42AB0 (`return m_data[i]`).
// Shape: the row read into a named local first (`entry = m_rows.m_data[row]`) loads the data pointer into eax before the
//   arguments, as retail; the direct `m_rows.m_data[row]->GetAt(column)` (and a named data-pointer local, or an inline
//   operator[]) loads the column first.
struct CTableEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    T GetAt(unsigned long index);           // out of line (0x00C42AB0 for CTableEntry*)
};

class CPlaybackEntry : public CPtrArray<CTableEntry*> {   // one row of the grid
};

class CItemGridTable_012b501c {             // vtable 0x012B501C
public:
    virtual unsigned long GetType();                                         // slot 0
    virtual long v01();                                                      // slot 1
    virtual long v02();                                                      // slot 2
    virtual long Insert(unsigned long row, CTableEntry* entry);              // slot 3
    virtual unsigned long GetColumnCount(unsigned long row);                 // slot 4
    virtual unsigned long GetRowCount();                                     // slot 5
    virtual CTableEntry* GetAt(unsigned long row, unsigned long column);     // slot 6
    CPtrArray<CPlaybackEntry*> m_rows;      // +4
};

CTableEntry* CItemGridTable_012b501c::GetAt(unsigned long row, unsigned long column)
{
    CPlaybackEntry* entry = m_rows.m_data[row];
    return entry->GetAt(column);
}