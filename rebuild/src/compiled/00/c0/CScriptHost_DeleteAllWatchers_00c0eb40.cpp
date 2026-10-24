// CScriptHost::DeleteAllWatchers() -- destroy every entry of the host's second list at +0x74: delete its first entry (whose destructor unlinks it) until the list is empty
// Leaf: DeleteAllWatchers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EB40, 41 bytes (`ret`). Descriptive names (no PDB covers this library):
//   the manifest's "CTCMapwho_ClearAllInventoryItemsFromUI" is propagated; the twin of DeleteAllScripts 0x00C0EAF0 over
//   the list at +0x74 (vtable 0x012B23F0). Called from the host destructor (0x00C0ED0E).
// Callees: the typed list's GetFirst (virtual slot 11, +0x2c), CScriptHost::DeleteWatcher @ 0x00C0EB20
//   (`delete watcher`, destructor 0x00C1E850).
// Shape: the list address is kept in esi, `this` in edi; the loop is rotated and the result never leaves eax.
struct CWatcher;

struct CWatcherList {                           // typed list over an implementation object (vtable 0x012B23F0)
    virtual long Compare0(void* a, void* b);
    virtual long Compare1(void* a, void* b);
    virtual void* Key2(void* item);
    virtual void* Key3(void* item);
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Remove(CWatcher* item);        // slot 8 (+0x20)
    virtual void Add(CWatcher* item);           // slot 9 (+0x24)
    virtual void Slot10();
    virtual CWatcher* GetFirst();               // slot 11 (+0x2c)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

struct CScriptHost {
    void* m_vtable;                             // +0x00 (0x012B25A8)
    unsigned char _pad04[0x74 - 0x04];
    CWatcherList m_watchers;                    // +0x74
    void DeleteWatcher(CWatcher* watcher);      // 0x00C0EB20
    void DeleteAllWatchers();
};

void CScriptHost::DeleteAllWatchers()
{
    CWatcher* watcher = m_watchers.GetFirst();
    while (watcher) {
        DeleteWatcher(watcher);
        watcher = m_watchers.GetFirst();
    }
}