// CEntryHost_00c37b90::DestroyEntry(void* key) -- find the registered entry for `key` in the host's registry (+0x28, slot-17 lookup by key address) and delete it (its destructor 0x00C473F0 unregisters it)
// Leaf: ?DestroyEntry@CEntryHost_00c37b90@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37B90, 49 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The inverse of CreateEntry 0x00C37990 (same host, same
//   0x68-byte entry of the landed 0x00C47340 / 0x00C473F0 constructor/destructor). Reached through 0x00C2BAD0 and
//   0x00C37DE6. The manifest's "CPredicate_And_Destructor" is propagated.
// Callees: the registry's virtual slot 17 (+0x44, lookup by key address), the entry's non-virtual destructor
//   0x00C473F0, operator delete 0x00BFE9BC.
// Shape: the registry's inline Find takes the key BY VALUE and passes its address, which gives retail's self-copy of
//   the argument slot (`mov eax,[esp+4] ; mov [esp+4],eax`) before the lookup; `delete entry` keeps its null test.
void __cdecl operator delete(void* block) throw();             // 0x00BFE9BC

struct CRegisteredEntry_00c37b90 {              // 0x68 bytes, not polymorphic
    unsigned char _body[0x68];
    ~CRegisteredEntry_00c37b90();               // 0x00C473F0
};

struct CEntryRegistry_00c37b90 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CRegisteredEntry_00c37b90* entry);     // slot 8 (+0x20)
    virtual void Add(CRegisteredEntry_00c37b90* entry);        // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CRegisteredEntry_00c37b90* Lookup(void* const* key);   // slot 17 (+0x44)
    CRegisteredEntry_00c37b90* Find(void* key) { return Lookup(&key); }
};

struct CEntryHost_00c37b90 {
    unsigned char _pad_0x00[0x28];
    CEntryRegistry_00c37b90 m_registry;         // +0x28
    void DestroyEntry(void* key);
};

void CEntryHost_00c37b90::DestroyEntry(void* key)
{
    CRegisteredEntry_00c37b90* entry = m_registry.Find(key);
    delete entry;
}