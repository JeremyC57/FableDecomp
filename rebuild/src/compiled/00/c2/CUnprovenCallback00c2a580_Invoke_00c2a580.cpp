// CUnprovenCallback_00c2a580::Invoke(CResourceWorld* host) -- when the host's key field (+0x04) matches the held owner's (+4), run host->AllocateFor(owner)
// Leaf: ?Invoke@CUnprovenCallback_00c2a580@@UAEXPAUCResourceWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c2a580, 27 bytes, thiscall, `ret 4` (or a tail jump).
// Identity (descriptive): slot 1 of the two-entry non-RTTI vtable 0x012B4180 (slot 0 its scalar deleting destructor 0x00C2AC30); it tail-calls the landed CResourceWorld::AllocateFor(void*) 0x00C36180 (which names 0x00C2A593 as its tail-jump caller) on its argument with the held owner. libpk: no PDB, no RTTI; the callback class and its slot are named descriptively, the
//   host and its operation keep the landed names.
// Callees: CResourceWorld::AllocateFor(void*) 0x00C36180 (thiscall; reached by a tail jump with the owner written over the argument slot).
// Shape (family L126): the held owner is loaded first, the two key fields compared, then the call in tail position.
struct CUnprovenKeyed_00c2a580 {                 // the held owner (only its key field is read)
    void* _00;
    long m_key;                                 // +0x04
};
struct CResourceWorld {
    unsigned char _pad[0x04];
    long m_key;                                 // +0x04
    void AllocateFor(void* owner);                    // 0x00C36180
};
class CUnprovenCallback_00c2a580 {                 // vtable 0x012B4180: { scalar deleting destructor, Invoke }
public:
    virtual ~CUnprovenCallback_00c2a580();
    virtual void Invoke(CResourceWorld* host);
    CUnprovenKeyed_00c2a580* m_owner;              // +0x04
};
void CUnprovenCallback_00c2a580::Invoke(CResourceWorld* host)
{
    CUnprovenKeyed_00c2a580* owner = m_owner;
    if (host->m_key == owner->m_key)
        host->AllocateFor(owner);
}