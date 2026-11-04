// CSoundEventSet::IsBusy() -- pop the next free event from the set's free-event pool at +0x30 (its virtual slot 1); returns the event or 0 when the pool is exhausted
// Leaf: ?IsBusy@CSoundEventSet@@QAEJXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21DF0, 8 bytes: `add ecx,30h ; mov eax,[ecx] ; jmp [eax+4]` (a no-argument virtual call on a
//   member object: the this-adjust comes first; with an argument VC7.1 loads the vptr from [ecx+N] first, as 0x00C465B0 shows).
// Library: libpk audio (no PDB / RTTI): descriptive names, the landed spelling of this address (CSoundEventManager::OnKey 0x00C11F00
//   calls it three times, UpdateItems 0x00C11CB0's model declares it). The catalog label `CIsThingAlive::operator()` is propagated
//   across byte-identical bodies (0x00C21E50 / 0x00C21F20 are the same shape on other members); reloc_bindings still carries the
//   stale `?IsBusy@CPanelItemSet@@QAEJXZ` (NAME_FIXES.tsv).
// Callee: the pool at +0x30 is built by the CSoundEventSet ctor 0x00C222D0 (`lea ecx,[esi+30h] ; call 0x00C239B0`, vtable
//   0x012B3A94: slot 0 the deleting destructor 0x00C22840, slot 1 0x00C23BE0 = `if (!m_freeCount) return 0; return
//   m_items[index()], --m_freeCount`). The descriptive name `IsBusy` undersells it (it hands back a free event; OnKey returns
//   it), but it is the established spelling: see NOTES.md.
class CSoundEventFreeList {                     // vtable 0x012B3A94 (ctor 0x00C239B0)
public:
    virtual ~CSoundEventFreeList();             // slot 0 (0x00C22840)
    virtual long Pop();                         // slot 1 (0x00C23BE0): the next free element, 0 when none
};

struct CSoundEventSet {                         // +0x2C of CSoundEventManager (ctor 0x00C222D0)
    unsigned char       _pad00[0x30];
    CSoundEventFreeList m_freeEvents;           // +0x30
    long IsBusy();
};

long CSoundEventSet::IsBusy()
{
    return m_freeEvents.Pop();
}