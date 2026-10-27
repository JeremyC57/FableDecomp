// CItemGridTable_012b501c::Insert(unsigned long row, CTableEntry* entry) -- slot 3 of the two-level item table: append entry to an existing row, or (row past the end) build a new row holding entry and append the row; returns 0
// Leaf: ?Insert@CItemGridTable_012b501c@@UAEJKPAUCTableEntry@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34F30, 180 bytes, thiscall `ret 8`, EH-framed (state 0 frees the new row if its
//   constructor throws).
// Identity (descriptive; libpk has no PDB or RTTI): slot 3 of vtable 0x012B501C (its only reference, 0x012B5028), the
//   CItemTable slot layout the landed Validate 0x00C348F0 records (slot 3 Insert(row, entry)) and the landed siblings
//   GetAt 0x00C34E20 / Remove 0x00C34FF0 of CItemGridTable_012b501c use. Catalog gapscan name is a placeholder.
// Callees: scalar operator new 0x00BFEA1A; the row constructor 0x00C42A60 (12-byte CPtrArray<CTableEntry*> row: three
//   zero words) and the row's out-of-line Add 0x00C42B50 (the landed wrapper over the array's inline Add); the row
//   array's Grow 0x00C34D40 (CPtrArray<CPlaybackEntry*>, as the landed Add 0x00C35050 / Remove 0x00C35080).
//   0x00C42A60 / 0x00C42B50 are spelled on the row class CPlaybackEntry the landed Remove 0x00C34FF0 uses (their other
//   bindings are T::Init / CProfileMember / CUnprovenPtrList_00c42b50: see NOTES "Callee names").
// Shape: the row array's Add is inlined (`capacity - count == 0` grows by one, store at the count, bump it); the new
//   row is null-checked by `new` (xor edi,edi) before the out-of-line Add. The row count is read into a named local
//   first (retail `mov eax,[esi+0Ch] ; mov ecx,[row] ; cmp ecx,eax`); comparing the member directly gives `cmp eax,[esi+0Ch]`.
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A

struct CTableEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C34D40 for CPlaybackEntry*
    void Add(T value)
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

class CPlaybackEntry : public CPtrArray<CTableEntry*> {   // one row of the grid (12 bytes)
public:
    CPlaybackEntry();                       // 0x00C42A60
    void Add(CTableEntry* entry);           // 0x00C42B50
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
    virtual CTableEntry* GetByIndex(unsigned long index);                    // slot 7
    virtual void Remove(unsigned long row, unsigned long column);            // slot 8
    CPtrArray<CPlaybackEntry*> m_rows;      // +4
};

long CItemGridTable_012b501c::Insert(unsigned long row, CTableEntry* entry)
{
    unsigned long count = m_rows.m_count;
    if (row < count) {
        m_rows.m_data[row]->Add(entry);
        return 0;
    }
    CPlaybackEntry* newRow = new CPlaybackEntry;
    newRow->Add(entry);
    m_rows.Add(newRow);
    return 0;
}