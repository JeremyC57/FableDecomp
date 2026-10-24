// CScriptHost::DestroyOwner(CScriptOwnerThing* owner) -- delete every script the list at +0x50 finds for the owner (keyed lookup, virtual slot 17), then delete the owner itself (virtual destructor)
// Leaf: DestroyOwner
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EC10, 67 bytes (`ret 4`). Descriptive names (no PDB covers this library):
//   the manifest's "CTCMapwho_ClearMessageQueue" is propagated. Callers: the host's purge loop 0x00C0EC90 and its
//   destructor helper 0x00C0EC60 (both pass an element of the host's owner array at +0x2c/+0x34), 0x00C1654C.
// Callees: the typed list's Find (virtual slot 17, +0x44: forwards to the implementation's keyed search; the key is
//   passed by reference, so the argument slot's address is pushed), CScriptHost::DeleteScript @ 0x00C0EAD0, the owner's
//   scalar deleting destructor (virtual slot 0, `push 1`).
// Shape: `delete owner` on a polymorphic class: null test, then `call [vtbl]` with 1, after the frame pops.
struct CScript;
struct CScriptOwnerThing {                      // polymorphic, virtual destructor in slot 0
    virtual ~CScriptOwnerThing();
};

struct CScriptList {                            // typed list over an implementation object (vtable 0x012B2350)
    virtual long Compare0(void* a, void* b);
    virtual long Compare1(void* a, void* b);
    virtual void* Key2(void* item);
    virtual void* Key3(void* item);
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Remove(CScript* item);         // slot 8 (+0x20)
    virtual void Add(CScript* item);            // slot 9 (+0x24)
    virtual void Slot10();
    virtual CScript* GetFirst();                // slot 11 (+0x2c)
    virtual CScript* GetNext();                 // slot 12 (+0x30)
    virtual void Slot13();
    virtual void Slot14();
    virtual unsigned long GetCount();           // slot 15 (+0x3c)
    virtual void Slot16();
    virtual CScript* Find(CScriptOwnerThing* const& key);   // slot 17 (+0x44)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CScriptHost {
    void* m_vtable;                             // +0x00 (0x012B25A8)
    unsigned char _pad04[0x50 - 0x04];
    CScriptList m_scripts;                      // +0x50
    void DeleteScript(CScript* script);         // 0x00C0EAD0
    void DestroyOwner(CScriptOwnerThing* owner);
};

void CScriptHost::DestroyOwner(CScriptOwnerThing* owner)
{
    CScript* script = m_scripts.Find(owner);
    while (script) {
        DeleteScript(script);
        script = m_scripts.Find(owner);
    }
    delete owner;
}