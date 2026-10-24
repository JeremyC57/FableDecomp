// CScriptHost::DeleteAllScripts() -- destroy every script registered in the host's list at +0x50: delete its first entry (whose destructor unlinks it) until the list is empty
// Leaf: DeleteAllScripts
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EAF0, 41 bytes (`ret`). Descriptive names (no PDB covers this library):
//   the manifest's "CTCMapwho_ClearAllMappedActorsFromUI" is propagated. Called from the host destructor (0x00C0ED15;
//   host vtable 0x012B25A8, the landed CScriptHost_00c0e600 of TryActivate 0x00C0E600).
// Callees: the typed list's GetFirst (virtual slot 11, +0x2c; list vtable 0x012B2350), CScriptHost::DeleteScript
//   @ 0x00C0EAD0 (`delete script`: destructor 0x00C1E260 unlinks the script from this list through slot 8).
// Shape: the list address is kept in esi, `this` in edi; the loop is rotated and the result never leaves eax.
struct CScript;
struct CScriptOwnerThing;

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
    void DeleteAllScripts();
};

void CScriptHost::DeleteAllScripts()
{
    CScript* script = m_scripts.GetFirst();
    while (script) {
        DeleteScript(script);
        script = m_scripts.GetFirst();
    }
}