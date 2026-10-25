// CSoundEmitter_00c37c40::CreateEntriesForSource() -- EH-framed: build a stack visitor (vtable 0x012B528C, constructed out of line @ 0x00C37BD0 with this emitter) whose Visit forwards each item to this emitter's CreateEntry, and hand it to the registry at +8 of the source object at +8 (virtual slot 10, for-each); the visitor's inline destructor is the EH state-0 unwind action
// Grade: DIFFER(78v75) -- retail keeps `this` in edx across the out-of-line visitor constructor call (LTCG knows 0x00C37BD0 leaves edx alone); VC7.1 saves it in esi (push/pop esi). Lane gap: LTCG register allocation (2 grades: declaration-only ctor, same-TU noinline ctor).
// Leaf: ?CreateEntriesForSource@CSoundEmitter_00c37c40@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37C40, 80 bytes (+ int3 padding), __thiscall, no arguments. Built under
//   /EHsc (unwind action 0x01214687 calls the visitor destructor 0x00C37F10, which stores the base vtable 0x012B465C).
// Library: libpk audio (no PDB/RTTI); descriptive names. The emitter is the class constructed at 0x00C37C90 (vtable
//   0x012B5370), which calls this as its last step (0x00C37D7E). The visitor's slot 0 (0x00C379F0:
//   `mov ecx,[ecx+4] ; jmp 0x00C37990`) is the host's CreateEntry; the base visitor (vtable 0x012B465C) has a pure
//   Visit and a virtual destructor. The manifest's "CSBAgents_BuildAndDispatch_CreatureOpinion" is propagated.
struct CSoundEmitter_00c37c40;

struct CItemVisitor_00c37c40 {                          // vtable 0x012B465C
    virtual void Visit(void* item) = 0;                 // slot 0
    virtual ~CItemVisitor_00c37c40() { }                // slot 1
};

struct CCreateEntryVisitor_00c37c40 : CItemVisitor_00c37c40 {   // vtable 0x012B528C
    CSoundEmitter_00c37c40* m_pHost;                    // +4
    CCreateEntryVisitor_00c37c40(CSoundEmitter_00c37c40* host);  // 0x00C37BD0
    virtual void Visit(void* item);                     // 0x00C379F0
};

struct CRegistry_00c37c40 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void ForEach(CItemVisitor_00c37c40* visitor);   // slot 10
};

struct CSource_00c37c40 {
    unsigned char _pad[8];
    CRegistry_00c37c40 m_registry;                      // +8
};

struct CSoundEmitter_00c37c40 {
    void* _vtbl;                                        // +0 (vtable 0x012B5370)
    void* m_pOwner;                                     // +4
    CSource_00c37c40* m_pSource;                        // +8
    void CreateEntriesForSource();
};

void CSoundEmitter_00c37c40::CreateEntriesForSource()
{
    CCreateEntryVisitor_00c37c40 visitor(this);
    m_pSource->m_registry.ForEach(&visitor);
}
