// CEntryList::~CEntryList() -- empties the single-row entry table (vtable 0x012B4FEC) by removing its last entry until the count is zero (the entries are not owned), frees the pointer array and falls back to the abstract table base
// Grade: RELOCATION_MATCH (wave-2 sec6: the RELOC_IDENTITY reject (scalar ??3 for delete[]) fixed by declaring the
//   MSVCR71 ::operator delete[] (0x00BFEB1C, ??_V@YAXPAX@Z): VC7.1 predeclares only scalar new/delete, so an undeclared
//   delete[] of a pointer array lowers to ??3)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34B80, 177 bytes, __fastcall `ret`, EH-framed (handler 0x01214368). Callers:
//   the deleting destructor 0x00C34CB0 (vtable 0x012B4FEC slot 11) and the inline-destroyed owners at 0x00C1F5C8,
//   0x00C1FFB8, 0x00C20188. Descriptive names (no PDB covers this library): the catalog's `Vector_RemoveDuplicates`
//   is a placeholder.
// Layout: the abstract table base (vtable 0x012B3444; Validate/Find/CopyFrom 0x00C348F0/0x00C34930/0x00C34870) and a
//   pointer array {data +4, capacity +8, count +0xC} -- the template of CPtrArray::Remove 0x00C35080 / 0x00C1EBF0,
//   whose destructor frees the data with operator delete[] @ 0x00BFEB1C. The derived class is one row of entries
//   (slot 4 `return 1`, slot 5 `return m_count`, slot 6/7 `m_data[i]`).
// Shape: `for (count = m_count; count; count = m_count) Remove(data[count - 1])` -- the re-read count is the loop
//   variable, so the index comes from the tested register (a plain `while (m_count)` re-reads it first and VC7.1 then
//   pads both inner loop heads); Remove (find-last + shift + `--count`) inlined; the member
//   array's inline destructor then the base's (its vtable store). No call can throw inside the body, so VC7.1 stores
//   only EH state 0 (the twin 0x00C34E60, which deletes each entry, stores state 1 then 0).
void __cdecl operator delete[](void* p);   // 0x00BFEB1C MSVCR71 ??_V@YAXPAX@Z (import thunk); VC7.1 predeclares only scalar new/delete
class CListEntry;
class CTableEntry;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    ~CPtrArray()
    {
        if (m_data) {
            delete[] m_data;
            m_data = 0;
        }
    }
    long Find(T value)
    {
        long index = -1;
        for (long i = m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value)
                index = i;
        return index;
    }
    void Remove(T value)
    {
        for (unsigned long i = Find(value); i < m_count - 1; ++i)
            m_data[i] = m_data[i + 1];
        --m_count;
    }
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

class CEntryList : public CItemTable {
public:
    CPtrArray<CListEntry*> m_entries;                                        // +0x04
    virtual unsigned long GetType();
    virtual long Insert(unsigned long row, CTableEntry* entry);
    virtual unsigned long GetColumnCount(unsigned long row);
    virtual unsigned long GetRowCount();
    virtual CTableEntry* GetAt(unsigned long row, unsigned long column);
    virtual CTableEntry* GetByIndex(unsigned long index);
    virtual void Remove(unsigned long row, unsigned long column);
    virtual ~CEntryList();
};

CEntryList::~CEntryList()
{
    for (unsigned long count = m_entries.m_count; count != 0; count = m_entries.m_count)
        m_entries.Remove(m_entries.m_data[count - 1]);
}