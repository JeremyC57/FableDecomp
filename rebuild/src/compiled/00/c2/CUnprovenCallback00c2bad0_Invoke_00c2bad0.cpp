// CUnprovenCallback_00c2bad0::Invoke(CEntryHost_00c37b90* host) -- when the host's key field (+0x08) matches the held key's (+4), run host->DestroyEntry(key)
// Leaf: ?Invoke@CUnprovenCallback_00c2bad0@@UAEXPAUCEntryHost_00c37b90@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c2bad0, 27 bytes, thiscall, `ret 4` (or a tail jump).
// Identity (descriptive): slot 1 of the two-entry non-RTTI vtable 0x012B44C0 (slot 0 0x00C2BF70); it tail-calls the landed CEntryHost_00c37b90::DestroyEntry(void*) 0x00C37B90, CreateEntry's inverse (the landed sources give the one host two descriptive names). libpk: no PDB, no RTTI; the callback class and its slot are named descriptively, the
//   host and its operation keep the landed names.
// Callees: CEntryHost_00c37b90::DestroyEntry(void*) 0x00C37B90 (thiscall; reached by a tail jump with the key written over the argument slot).
// Shape (family L126): the held key is loaded first, the two key fields compared, then the call in tail position.
struct CUnprovenKeyed_00c2bad0 {                 // the held key (only its key field is read)
    void* _00;
    long m_key;                                 // +0x04
};
struct CEntryHost_00c37b90 {
    unsigned char _pad[0x08];
    long m_key;                                 // +0x08
    void DestroyEntry(void* key);                    // 0x00C37B90
};
class CUnprovenCallback_00c2bad0 {                 // vtable 0x012B44C0: { scalar deleting destructor, Invoke }
public:
    virtual ~CUnprovenCallback_00c2bad0();
    virtual void Invoke(CEntryHost_00c37b90* host);
    CUnprovenKeyed_00c2bad0* m_key;              // +0x04
};
void CUnprovenCallback_00c2bad0::Invoke(CEntryHost_00c37b90* host)
{
    CUnprovenKeyed_00c2bad0* key = m_key;
    if (host->m_key == key->m_key)
        host->DestroyEntry(key);
}