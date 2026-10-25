// CRowTable::~CRowTable() -- empties the row table (vtable 0x012B501C) by removing its last row until the count is zero and deleting each removed row (each row is an entry array with its own destructor), frees the pointer array and falls back to the abstract table base
// Grade: RELOC_IDENTITY reject at landing -- bytes match but the build calls scalar operator delete (??3, 0x00BFE9BC) where retail calls the operator delete[] import 0x00BFEB1C; an explicit ::operator delete[](m_data) changed the body (130v177 / 151v203). Next: find the source form VC7.1 lowers to ??_V (class-type element array or a typed delete[])
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34E60, 203 bytes, __thiscall `ret`, EH-framed (handler 0x01214393). Callers:
//   the deleting destructor 0x00C350D0 (vtable 0x012B501C slot 11) and the inline-destroyed owners at 0x00C1F618,
//   0x00C20028, 0x00C20208. Descriptive names (no PDB covers this library): the catalog's
//   `Vector_RemoveAndDestroyDuplicates` is a placeholder. Twin of CEntryList::~CEntryList 0x00C34B80.
// Layout: the abstract table base (vtable 0x012B3444; Validate/Find/CopyFrom 0x00C348F0/0x00C34930/0x00C34870) and a
//   pointer array {data +4, capacity +8, count +0xC} -- the template of CPtrArray::Remove 0x00C35080 / 0x00C1EBF0,
//   whose destructor frees the data with operator delete[] @ 0x00BFEB1C. The derived class is one row of entries
//   (slot 4 `return 1`, slot 5 `return m_count`, slot 6/7 `m_data[i]`) in the twin; here each entry is a heap row
//   (columns = the row's count, 0x00C34DF0) destroyed by its out-of-line destructor @ 0x00C42AD0 and scalar
//   operator delete @ 0x00BFE9BC.
// Shape: `for (count = m_count; count; count = m_count) Remove(data[count - 1])` -- the re-read count is the loop
//   variable, so the index comes from the tested register (a plain `while (m_count)` re-reads it first and VC7.1 then
//   pads both inner loop heads); Remove (find-last + shift + `--count`) inlined; the member
//   array's inline destructor then the base's (its vtable store). The row delete can throw, so the body runs at EH
//   state 1 and the member array's destructor at state 0 (the twin, with no call in its body, stores only state 0).
class CTableRow {
public:
    unsigned char _body[0xc];
    ~CTableRow();                           // 0x00C42AD0
};
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

class CRowTable : public CItemTable {
public:
    CPtrArray<CTableRow*> m_entries;                                         // +0x04
    virtual unsigned long GetType();
    virtual long Insert(unsigned long row, CTableEntry* entry);
    virtual unsigned long GetColumnCount(unsigned long row);
    virtual unsigned long GetRowCount();
    virtual CTableEntry* GetAt(unsigned long row, unsigned long column);
    virtual CTableEntry* GetByIndex(unsigned long index);
    virtual void Remove(unsigned long row, unsigned long column);
    virtual ~CRowTable();
};

CRowTable::~CRowTable()
{
    for (unsigned long count = m_entries.m_count; count != 0; count = m_entries.m_count) {
        CTableRow* row = m_entries.m_data[count - 1];
        m_entries.Remove(row);
        delete row;
    }
}
