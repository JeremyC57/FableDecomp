// CMappedValueOwner_00c2a340::LookupMappedValue(const SKeyPair& key) -- resolves a key pair through the attached resolver's slot-1 virtual, then looks the handle up in the embedded map object (slot 17) -- 0 when no resolver is attached or the owner is disabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A340, length 75 bytes. No direct callees
// (two virtual calls: slot 1 of the resolver at +0xd0, slot 17 (+0x44) of the embedded object at +0x68).
// The manifest label "CAIBrain::GetCurrentStateGroupType" is propagated (CAIBrain is 0xb0 bytes;
// this object reaches +0xd0) and is rejected; names are descriptive. The argument is a reference
// to a two-dword pair pushed right to left (second, first); the resolver's long result is a
// temporary whose address goes to the map lookup -- VC7.1 homes it in the dead by-reference
// argument slot ([esp+0Ch]), exactly as retail. The disabled flag at +0xcc is a byte.
struct SKeyPair {
    long first;                             // +0x00
    long second;                            // +0x04
};

struct CKeyResolver {
    virtual void v00();
    virtual long Resolve(long first, long second);   // slot 1 (+0x04)
};

struct CHandleMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual long Lookup(const long& handle);         // slot 17 (+0x44)
};

struct CMappedValueOwner_00c2a340 {
    unsigned char _pad0[0x68];
    CHandleMap    m_map;                    // +0x68 (embedded, own vtable)
    unsigned char _pad1[0x60];              // +0x6c..+0xcc
    bool          m_bDisabled;              // +0xcc
    unsigned char _pad2[3];
    CKeyResolver* m_pResolver;              // +0xd0
    long LookupMappedValue(const SKeyPair& key);
};

long CMappedValueOwner_00c2a340::LookupMappedValue(const SKeyPair& key)
{
    if (m_pResolver != 0 && !m_bDisabled)
        return m_map.Lookup(m_pResolver->Resolve(key.first, key.second));
    return 0;
}
