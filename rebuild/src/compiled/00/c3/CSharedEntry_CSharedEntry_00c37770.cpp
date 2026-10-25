// CSharedEntry::CSharedEntry(CSharedEntryTable* table, void* key) -- a libpk shared table entry: owner, index -1, key, two zero words, then the entry registers itself in the table's keyed list
// Leaf: ??0CSharedEntry@@QAE@PAVCSharedEntryTable@@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37770, 46 bytes (+2 int3 padding), thiscall, `ret 8`.
// Identity: the only caller is the landed CSharedEntryTable::Acquire 0x00C2A3D0 (`new CSharedEntry(this, key)`, 20 bytes,
//   then `++entry->m_refCount` at +0x10), which already names this constructor. libpk (0x00C08AE0..): no PDB/RTTI, so the
//   names are descriptive; the catalog label CWStringParser::Init is propagated (the real Init is 0x00A0C8E0, landed).
// Callees: the table's keyed list (+0x38) virtual Add, slot 9 (+0x24).
// Shape: init-list stores (key and the shared zero register first, then owner and the -1 index around the list address),
//   then the virtual call on the embedded list member with `this` pushed.
class CSharedEntry;

struct CSharedEntryList {                   // libpk keyed list (polymorphic)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual void Add(CSharedEntry* entry);  // slot 9 (+0x24)
};

class CSharedEntryTable {
public:
    unsigned char    _pad_0x00[0x38];
    CSharedEntryList m_entries;             // +0x38
};

class CSharedEntry {                        // 20 bytes
public:
    CSharedEntry(CSharedEntryTable* table, void* key);
    CSharedEntryTable* m_pTable;            // +0x00
    long               m_index;             // +0x04
    void*              m_key;               // +0x08
    long               m_0c;                // +0x0c (use unknown; name kept from the landed Acquire 0x00C2A3D0 model)
    long               m_refCount;          // +0x10
};

CSharedEntry::CSharedEntry(CSharedEntryTable* table, void* key)
    : m_pTable(table), m_index(-1), m_key(key), m_0c(0), m_refCount(0)
{
    table->m_entries.Add(this);
}