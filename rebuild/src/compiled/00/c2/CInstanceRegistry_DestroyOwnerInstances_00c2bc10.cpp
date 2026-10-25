// CInstanceRegistry::DestroyOwnerInstances(void* owner) -- destroy every instance the owner list (+0x04) yields for the owner (DestroyInstance each, until Find comes back empty)
// Leaf: DestroyOwnerInstances
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BC10, length 80 bytes (67 + int3 padding), `ret 4`.
// Caller: the libpk owner's DeleteData @ 0x00C14A50 (call 0x00C14A65, on the registry at +0x0C right after the resource
//   registry's ReleaseOwnerEntries @ 0x00C2A640, this chunk). Callees: the owner list's virtual Find (slot 17, +0x44),
//   DestroyInstance @ 0x00C2BAF0 (landed as "CTCMapwho::OnDie": unlink @ 0x00C37DA0, then `operator delete`).
// Identity: the instance registry of the landed CreateInstance @ 0x00C2BB10 ("CTCMapwho_00c2bb10": its +0x34 list is keyed by
//   {owner, qualifier}); this +0x04 list is keyed by the owner alone. No PDB covers libpk: the names are descriptive (the
//   catalog's `CTCMapwho_DeleteAllInstances` is propagated; CTCMapwho is a game class).
// Shape: the twin of 0x00C2A640 -- the owner converted to the list's key type in a temporary rebuilt in the dead argument
//   slot for every Find (`mov [esp+14h],ebx`), a rotated while with the list pointer in esi and `this` in edi.
struct CInstance;

struct CInstanceList {                      // libpk keyed list (polymorphic)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CInstance* Find(const unsigned long& key);       // slot 17 (+0x44)
};

struct CInstanceRegistry {
    void*         __vftable;                // +0x00
    CInstanceList m_byOwner;                // +0x04 instances by owner
    void DestroyInstance(CInstance* instance);               // 0x00C2BAF0
    void DestroyOwnerInstances(void* owner);
};

void CInstanceRegistry::DestroyOwnerInstances(void* owner)
{
    CInstance* instance;
    while ((instance = m_byOwner.Find((unsigned long)owner)) != 0)
        DestroyInstance(instance);
}