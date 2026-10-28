// CSoundEventManager::GetPhysicalPlayer(unsigned long priority) -- hand back a free physical player; when none is free, make the playing event found below the given priority yield its player and hand back the freed one (null when no event can be pre-empted)
// Leaf: ?GetPhysicalPlayer@CSoundEventManager@@QAEPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10C90, 60 bytes, thiscall, `ret 4`. Descriptive names (the libpk audio library
//   has no PDB or RTTI); the class set is the landed one: CSoundEventManager (vtable 0x012B2748) with its event set at
//   +0x2C (landed 0x00C10CD0 AssignPendingItems, 0x00C21EB0 FindEntry), CSoundEvent (landed 0x00C216D0).
//   Label correction: the manifest row "CInputProcessRebootGame::ProcessInput" is propagated.
// Caller: the event's physical-play path 0x00C21780 (`push [event+14h] ; call ; test eax,eax ; je` then
//   PlayPhysical 0x00C20A20 with the result as the player).
// Callees: CSoundEventSet::GetFreeSlot 0x00C21E50, CSoundEventSet::FindEntry 0x00C21EB0,
//   CSoundEvent::YieldPhysicalPlayer 0x00C216D0 (state 6 -> 3, frees the player).
// Shape: the early free-slot return and the null-entry return both leave their value in eax (`jne` to the shared exit,
//   and the second `ret 4` returns the null entry).
struct CSoundEventManager;
struct CSoundEvent {
    void YieldPhysicalPlayer(CSoundEventManager* manager);   // 0x00C216D0
};
struct CSoundEventEntry {
    unsigned char _pad00[4];
    CSoundEvent*   m_pEvent;                    // +0x04
};
struct CSoundEventSet {                         // +0x2C of the manager
    void* GetFreeSlot();                        // 0x00C21E50
    CSoundEventEntry* FindEntry(unsigned long key);   // 0x00C21EB0
};
struct CSoundEventManager {
    unsigned char _pad00[0x2c];
    CSoundEventSet m_eventSet;                  // +0x2C
    void* GetPhysicalPlayer(unsigned long priority);
};

void* CSoundEventManager::GetPhysicalPlayer(unsigned long priority)
{
    void* slot = m_eventSet.GetFreeSlot();
    if (slot)
        return slot;
    CSoundEventEntry* entry = m_eventSet.FindEntry(priority);
    if (!entry)
        return 0;
    entry->m_pEvent->YieldPhysicalPlayer(this);
    return m_eventSet.GetFreeSlot();
}