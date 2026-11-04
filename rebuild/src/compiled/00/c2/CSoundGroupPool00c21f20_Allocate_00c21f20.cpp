// CSoundGroupPool_00c21f20::Allocate() -- pop the next free sound group from the pool member at +0x318 (its virtual slot 1); 0 when none is free
// Leaf: ?Allocate@CSoundGroupPool_00c21f20@@QAEPAVCSoundGroup_00c29ec0@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21F20, 11 bytes: `add ecx,318h ; mov eax,[ecx] ; jmp [eax+4]` (no-argument virtual call on a
//   member object).
// Library: libpk audio (no PDB / RTTI): descriptive names, the landed spelling of this address (reloc_bindings; the landed caller
//   CSoundSystem_00c131c0::PlaySoundList 0x00C131C0 calls it on its +0x2C member to chain started sounds into a group). The catalog
//   label `CIsThingAlive::operator()` is propagated across byte-identical bodies. The +0x318 member is the group free list the
//   CSoundEventSet ctor 0x00C222D0 builds (`lea ecx,[esi+318h] ; call 0x00C24050`, vtable 0x012B3AA4), so the class is probably
//   CSoundEventSet; kept as landed until the caller's class is proven (NOTES.md).
class CSoundGroup_00c29ec0;

class CSoundGroupFreeList {                     // vtable 0x012B3AA4 (ctor 0x00C24050)
public:
    virtual ~CSoundGroupFreeList();             // slot 0
    virtual CSoundGroup_00c29ec0* Pop();        // slot 1
};

class CSoundGroupPool_00c21f20 { public:        // the +0x2C member of PlaySoundList's `this`
    unsigned char       _pad00[0x318];
    CSoundGroupFreeList m_freeGroups;           // +0x318
    CSoundGroup_00c29ec0* Allocate();
};

CSoundGroup_00c29ec0* CSoundGroupPool_00c21f20::Allocate()
{
    return m_freeGroups.Pop();
}