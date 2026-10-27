// CItemGridTable_012b501c::Remove(unsigned long row, unsigned long column) -- slot 8: drop one entry of a row; a row left empty is removed from the row list and deleted
// Leaf: ?Remove@CItemGridTable_012b501c@@UAEXKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34FF0, 71 bytes, thiscall `ret 8`.
// Identity: slot 8 (Remove(row, column), CItemTable slot layout of the landed Validate 0x00C348F0) of vtable 0x012B501C;
//   the landed CPtrArray<CPlaybackEntry*>::Remove 0x00C35080 names this as its caller. Descriptive (libpk, no PDB/RTTI).
// Callees: the row's RemoveAt(index) 0x00C42B80, GetCount() 0x00C42AC0, the row-list Remove(value) 0x00C35080, the row
//   destructor 0x00C42AD0 (??1?$CPtrArray@PAUCTableEntry@@@@, as landed), scalar operator delete 0x00BFE9BC.
// Shape: `delete row` = null test + destructor + operator delete; the row is a derived class with an implicit
//   destructor, which inlines to the base destructor call.
void __cdecl operator delete(void* p);

struct CTableEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    ~CPtrArray();                           // 0x00C42AD0 for CTableEntry*
    unsigned long GetCount();               // 0x00C42AC0
    void RemoveAt(unsigned long index);     // 0x00C42B80
    void Remove(T value);                   // 0x00C35080 for CPlaybackEntry*
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
    virtual CTableEntry* GetByIndex(unsigned long index);                    // slot 7
    virtual void Remove(unsigned long row, unsigned long column);            // slot 8
    CPtrArray<CPlaybackEntry*> m_rows;      // +4
};

void CItemGridTable_012b501c::Remove(unsigned long row, unsigned long column)
{
    CPlaybackEntry* entry = m_rows.m_data[row];
    entry->RemoveAt(column);
    if (entry->GetCount() == 0) {
        m_rows.Remove(entry);
        delete entry;
    }
}