// CUnprovenCallback_00c2a5a0::Invoke(CResourceWorld* host) -- when the host's key field (+0x04) matches the held owner's (+4), run host->FreeAllocation(owner)
// Leaf: ?Invoke@CUnprovenCallback_00c2a5a0@@UAEXPAUCResourceWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c2a5a0, 27 bytes, thiscall, `ret 4` (or a tail jump).
// Identity (descriptive): slot 1 of the two-entry non-RTTI vtable 0x012B4188 (slot 0 0x00C2AC80); it tail-calls the landed CResourceWorld::FreeAllocation(void*) 0x00C36290 (the inverse of AllocateFor; it names 0x00C2A5B3 as its tail-jump caller). libpk: no PDB, no RTTI; the callback class and its slot are named descriptively, the
//   host and its operation keep the landed names.
// Callees: CResourceWorld::FreeAllocation(void*) 0x00C36290 (thiscall; reached by a tail jump with the owner written over the argument slot).
// Shape (family L126): the held owner is loaded first, the two key fields compared, then the call in tail position.
struct CUnprovenKeyed_00c2a5a0 {                 // the held owner (only its key field is read)
    void* _00;
    long m_key;                                 // +0x04
};
struct CResourceWorld {
    unsigned char _pad[0x04];
    long m_key;                                 // +0x04
    void FreeAllocation(void* owner);                    // 0x00C36290
};
class CUnprovenCallback_00c2a5a0 {                 // vtable 0x012B4188: { scalar deleting destructor, Invoke }
public:
    virtual ~CUnprovenCallback_00c2a5a0();
    virtual void Invoke(CResourceWorld* host);
    CUnprovenKeyed_00c2a5a0* m_owner;              // +0x04
};
void CUnprovenCallback_00c2a5a0::Invoke(CResourceWorld* host)
{
    CUnprovenKeyed_00c2a5a0* owner = m_owner;
    if (host->m_key == owner->m_key)
        host->FreeAllocation(owner);
}