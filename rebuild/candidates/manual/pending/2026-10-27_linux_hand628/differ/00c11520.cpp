// CPanel::DispatchNavigation(unsigned long key, const CNavDirection& direction) -- under the library lock, find the handler the panel's item set holds for the key and, if there is one, send it a navigation event carrying its own can-handle answer and a copy of the direction
// Grade: DIFFER(163v163) -- scheduling only: in the direction copy retail stores the first word into the event (`mov [esp+10h],ecx`) before `push eax` (the event address for Handle), VC7.1 here after it; every other byte matches. Tried: struct assignment, memberwise x/y/z, z-first through locals (3 grades). Next idea: a same-TU noinline CNavEvent ctor body
// Flags: /EHsc
// Leaf: DispatchNavigation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11520, 163 bytes (`ret 8`). Built under /EHsc. Descriptive names (no PDB
//   covers this library): the manifest's "Inventory_ProcessNavigationItem" is propagated. Referenced from the panel
//   vtable slot at 0x012B27B4; the item set at +0x2c is the one of OnKey 0x00C11F00.
// Callees: CScopedLock::CScopedLock @ 0x00C0FFC0 / ~CScopedLock @ 0x00C0FFE0 over the global lock @ 0x0143BB14,
//   CPanelItemSet::FindHandler @ 0x00C21E20, CNavEvent::CNavEvent @ 0x00C20390 (out of line: zeroes the three words and
//   the flag), the handler's CanHandle (virtual slot 8, +0x20) and Handle (virtual slot 5, +0x14).
// Shape: the event is a 16-byte frame object behind the 8-byte lock; the direction is copied as three dwords (the
//   third loaded into eax and stored first).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;       // 0x0143BB14

struct CScopedLock {                            // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);     // 0x00C0FFC0
    ~CScopedLock();                             // 0x00C0FFE0
};

struct CNavDirection {                          // 12 bytes
    long m_x;
    long m_y;
    long m_z;
};

struct CNavEvent {                              // 16 bytes
    CNavDirection m_direction;                  // +0x00
    bool m_bCanHandle;                          // +0x0c
    CNavEvent();                                // 0x00C20390
};

struct CPanel;

struct CNavHandler {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Handle(CPanel* panel, CNavEvent* event);   // slot 5 (+0x14)
    virtual void Slot6(); virtual void Slot7();
    virtual bool CanHandle(CPanel* panel);      // slot 8 (+0x20)
};

struct CPanelItemSet {                          // +0x2c of the panel
    CNavHandler* FindHandler(unsigned long key);   // 0x00C21E20
};

struct CPanel {
    unsigned char _pad00[0x2c];
    CPanelItemSet m_itemSet;                    // +0x2c
    void DispatchNavigation(unsigned long key, const CNavDirection& direction);
};

void CPanel::DispatchNavigation(unsigned long key, const CNavDirection& direction)
{
    CScopedLock lock(&g_lock_0143bb14);
    CNavHandler* handler = m_itemSet.FindHandler(key);
    if (handler) {
        CNavEvent event;
        event.m_bCanHandle = handler->CanHandle(this);
        event.m_direction = direction;
        handler->Handle(this, &event);
    }
}
