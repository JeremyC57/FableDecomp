// CResourceRegistry::ReleaseByKey(unsigned long key) -- look the key up in the embedded registry map and release the entry it finds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A6D0, length 33 bytes (48 with padding). Catalog label
// "CTCGraphicAppearanceMultipleStaticMeshes::OnCreate" is propagated and wrong: this is third-party library code in the
// 0x00C0xxxx..0x00C3xxxx block (operator delete @ 0x00BFE9BC), reached only from 0x00C14A19 under a scoped lock.
// Callees: the embedded map's virtual Find(const unsigned long* key) (slot 17, +0x44) and ReleaseEntry(void* entry)
// @ 0x00C2A5C0 (manifest "CTCParticleAttacher::HandleMessage", propagated: it destroys + deletes the entry and its owner
// node once the owner's count reaches zero).
// Layout: an embedded polymorphic map at +0x68 (a second one at +0x98 is used by the sibling @ 0x00C2A640).
// Leaf: ReleaseByKey
// Shape: the key's address is the incoming stack slot itself (`lea edx,[esp+8]`): the argument is passed by pointer.
struct CRegistryMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual void* Find(const unsigned long* key);       // slot 17 (+0x44)
};

struct CResourceRegistry {
    unsigned char _pad_0x00[0x68];
    CRegistryMap  m_entries;                            // +0x68
    void ReleaseEntry(void* entry);                     // 0x00C2A5C0
    void ReleaseByKey(unsigned long key);
};

void CResourceRegistry::ReleaseByKey(unsigned long key)
{
    void* entry = m_entries.Find(&key);
    if (entry)
        ReleaseEntry(entry);
}
