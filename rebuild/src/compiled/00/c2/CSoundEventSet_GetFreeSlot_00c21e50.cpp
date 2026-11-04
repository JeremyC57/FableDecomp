// CSoundEventSet::GetFreeSlot() -- pop the next free physical player from the set's player pool at +0x1F8 (its virtual slot 1); 0 when none is free
// Leaf: ?GetFreeSlot@CSoundEventSet@@QAEPAXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21E50, 11 bytes: `add ecx,1F8h ; mov eax,[ecx] ; jmp [eax+4]` (no-argument virtual call on a
//   member object).
// Library: libpk audio (no PDB / RTTI): descriptive names, the landed spelling of this address (reloc_bindings and the landed callers
//   CSoundEventManager::GetPhysicalPlayer 0x00C10C90 and AssignPendingItems 0x00C10CD0, both `void* GetFreeSlot()`). The catalog
//   label `CIsThingAlive::operator()` is propagated across byte-identical bodies.
// Callee: the pool at +0x1F8 is built by the CSoundEventSet ctor 0x00C222D0 (`lea ecx,[esi+1F8h] ; call 0x00C23E00`, vtable
//   0x012B3A9C: slot 1 0x00C24020 = the free-list pop, 0 when empty).
class CPhysicalPlayerFreeList {                 // vtable 0x012B3A9C (ctor 0x00C23E00)
public:
    virtual ~CPhysicalPlayerFreeList();         // slot 0
    virtual void* Pop();                        // slot 1 (0x00C24020)
};

struct CSoundEventSet {                         // +0x2C of CSoundEventManager (ctor 0x00C222D0)
    unsigned char           _pad00[0x1f8];
    CPhysicalPlayerFreeList m_freePlayers;      // +0x1F8
    void* GetFreeSlot();
};

void* CSoundEventSet::GetFreeSlot()
{
    return m_freePlayers.Pop();
}