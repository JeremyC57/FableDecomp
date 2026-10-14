// CRegistryOwner_00c21ee0::Lookup(long key) -- hand the key (a local homed in its own argument slot, by address) to the registry object at +0x108 through its virtual slot 17
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21EE0, length 32 bytes.
// The chunk label "CIsThingAlive::operator()" is propagated; named by its effect. Shape: `mov [esp+4],eax` (the key copied into its slot), `add ecx,108h`, `ret 4`.
struct CRegistry_00c21ee0 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual long Find(long* key);                  // slot 17 (+0x44)
};
struct CRegistryOwner_00c21ee0 {
    unsigned char        _pad[0x108];
    CRegistry_00c21ee0   m_registry;               // +0x108
    long Lookup(long key);
};

long CRegistryOwner_00c21ee0::Lookup(long key)
{
    long value = key;
    return m_registry.Find(&value);
}
