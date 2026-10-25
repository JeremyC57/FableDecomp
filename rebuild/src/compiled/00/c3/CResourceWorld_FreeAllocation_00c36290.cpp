// CResourceWorld::FreeAllocation(void* owner) -- detaches the owner's resource allocation from the world's +0x14 registry (virtual slot 17, keyed by the owner) and deletes it
// Leaf: FreeAllocation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36290, 49 bytes, __fastcall `ret 4`. Descriptive name (no PDB covers this
//   library): the catalog's `Thiscall_ResourceMapEntryRemove` is propagated. Callers: 0x00C2A5B3 (tail jump) and
//   0x00C365CB. Counterpart of AllocateFor 0x00C36180 (`new CResourceAllocation(this, owner)`), whose constructor
//   (landed 0x00C44EB0) registers the allocation in the same +0x14 registry.
// Callees: the registry's Detach (slot 17, +0x44: takes the key's address, returns the allocation or 0), the
//   allocation's destructor (landed 0x00C44F80) and scalar operator delete @ 0x00BFE9BC.
// Shape: an inline helper takes the key BY VALUE and passes its address (`mov eax,[esp+4] ; mov [esp+4],eax` -- the
//   fresh copy lands in the dead argument slot); `delete p` = null test + destructor + operator delete.
struct CResourceAllocation {
    unsigned char _body[0x8c];
    ~CResourceAllocation();                                 // 0x00C44F80
};

struct CAllocationRegistry {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CResourceAllocation* Detach(void* const* key);  // slot 17 (+0x44)
};

struct CResourceWorld {
    unsigned char       _pad00[0x14];
    CAllocationRegistry m_registry;                         // +0x14
    CResourceAllocation* DetachAllocation(void* owner) { return m_registry.Detach(&owner); }
    void FreeAllocation(void* owner);
};

void CResourceWorld::FreeAllocation(void* owner)
{
    delete DetachAllocation(owner);
}