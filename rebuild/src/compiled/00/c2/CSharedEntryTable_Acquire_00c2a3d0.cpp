// CSharedEntryTable::Acquire(void* key) -- find the key's shared entry in the table's keyed list (+0x38); when there is none, create one (a 20-byte entry whose constructor registers itself with this list); either way add one reference (+0x10) and return the entry
// Leaf: Acquire
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A3D0, length 128 bytes (113 + int3 padding), __fastcall, `ret 4`.
// Caller: 0x00C44D0D (a loop over a key array that hands each acquired entry to a collector's virtual slot 9).
// Callees: the keyed list's virtual Find (slot 17, +0x44), MSVCR71 scalar operator new (thunk 0x00BFEA1A), the entry's
//   constructor CSharedEntry(table, key) @ 0x00C37770 (owner +0x00, index -1 +0x04, key +0x08, two zero words +0x0C/+0x10,
//   then `table->m_entries.Add(this)` through the list's virtual slot 9).
// Name: no PDB covers this library; the names are descriptive (the catalog's `CWStringParser_FindOrCreate` is propagated).
// Shape: built under /EHsc -- the `new` expression's EH state 0 (the raw block freed if the constructor throws) with the
//   block homed in the `push ecx` frame word; the key is converted to the list's key type in a temporary that lives in the
//   dead argument slot (`mov [esp+20h],edi`), the original kept in edi for the constructor; the null-`new` path yields 0
//   and the reference increment is shared by both paths.
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CSharedEntryTable;

class CSharedEntry {                        // 20 bytes
public:
    CSharedEntry(CSharedEntryTable* table, void* key);   // 0x00C37770
    CSharedEntryTable* m_pTable;            // +0x00
    long               m_index;             // +0x04
    void*              m_key;               // +0x08
    long               m_0c;                // +0x0c
    long               m_refCount;          // +0x10
};

struct CSharedEntryList {                   // libpk keyed list (polymorphic)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CSharedEntry* Find(const unsigned long& key);    // slot 17 (+0x44)
};

class CSharedEntryTable {
public:
    CSharedEntry* Acquire(void* key);
private:
    unsigned char    _pad_0x00[0x38];
    CSharedEntryList m_entries;             // +0x38
};

CSharedEntry* CSharedEntryTable::Acquire(void* key)
{
    CSharedEntry* entry = m_entries.Find((unsigned long)key);
    if (!entry)
        entry = new CSharedEntry(this, key);
    ++entry->m_refCount;
    return entry;
}