// CSoundEvent::Stop(CSoundEventManager* manager) -- stop the event: after the state check an event that is waiting or playing (states 2..6) leaves its state through ReturnToIdle; then state 1, out of its group (CSteeringGroup::RemoveAgent), out of the manager's listening list (+0x1F4, only when flagged) and its +0x1C4 / +0x104 lists (list slot 8 = Remove), the sample reference reset, and back into the manager's event set (+0x2C)
// Leaf: ?Stop@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CSteeringAgentc21620_OnCreate_00c21620.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21620, 167 bytes (+ int3 padding), `ret 4`. Renamed (sweep1) from
//   CSteeringAgent_c21620::OnCreate (the catalog's "CTCInventory::OnCreate" is propagated). Identity: the CSoundEvent state
//   machine (state word +0xAC; agent434 NOTES: event-list slot 8 = Remove, slot 9 = Add); agent433 calls this the event's
//   Stop (CSoundEventManager::Shutdown 0x00C123B0 stops every active event with it; UpdateItems 0x00C11CB0 stops the
//   inaudible ones). Eleven callers in the manager code (e.g. 0x00C112AC).
// Callees: CSoundEvent::CheckState @ 0x00C20720 (a jump-table switch on the state word), IsWaitingOrPlayingPhysical
//   @ 0x00C207C0 (states 2..4, 6) and the state-5 ("Playing using virtual player") query @ 0x00C207A0, ReturnToIdle
//   @ 0x00C21560, the group's RemoveAgent(manager, event) @ 0x00C29EE0 (landed as CSteeringGroup; on +0x30), the manager
//   lists' virtual slot 8 (+0x20, Remove) at +0x1F4 (only when the event's +0xB0 listening flag is set), +0x1C4 and
//   +0x104, the +0x18 sample reference's Reset() @ 0x00C25CB0, the manager event set's Add(event) @ 0x00C21F30 (+0x2C).
// Shape: `this` stays in ecx across the state helpers because their bodies (here stand-ins, compiled above with
// auto_inline off) are known not to clobber it; the queries return int (`test eax,eax`) -- only `mov esi,ecx` is kept for the later calls; the three member
// resets sit between the last call's push and the call.
struct CSoundEventManager;
struct CSoundEvent;

struct CSoundEventList {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CSoundEvent* event); // slot 8 (+0x20)
};
struct CSoundEventSet {
    void Add(CSoundEvent* event);          // 0x00C21F30
};
struct CSoundEventManager {
    unsigned char             _pad_0x000[0x2C];
    CSoundEventSet m_eventSet;                // +0x2C
    unsigned char             _pad_0x030[0xD4];
    CSoundEventList  m_list104;             // +0x104
    unsigned char             _pad_0x108[0xBC];
    CSoundEventList  m_list1C4;             // +0x1C4
    unsigned char             _pad_0x1c8[0x2C];
    CSoundEventList  m_list1F4;             // +0x1F4
};
struct CSteeringGroup {
    void RemoveAgent(CSoundEventManager* manager, CSoundEvent* event);   // 0x00C29EE0
};
struct CSampleRef {
    void Reset();                                    // 0x00C25CB0
};

struct CSoundEvent {
    void*                     __vftable;             // +0x00
    unsigned char             _pad_0x04[0x08];
    long                      Index;                 // +0x0C
    unsigned char             _pad_0x10[0x08];
    CSampleRef     m_sample;                 // +0x18
    unsigned char             _pad_0x19[0x0F];
    long                      Pending;               // +0x28
    unsigned char             _pad_0x2c[0x04];
    CSteeringGroup* m_pGroup;             // +0x30
    unsigned char             _pad_0x34[0x78];
    unsigned short            m_state;             // +0xAC
    unsigned char             _pad_0xae[0x02];
    bool                      m_listening;            // +0xB0
    long                      Event;                 // +0xB4 (stand-in only)

    void CheckState(long event);           // 0x00C20720
    long IsWaitingOrPlayingPhysical();                        // 0x00C207C0 (int result)
    long IsPlayingVirtual_00c207a0();                      // 0x00C207A0 (int result)
    void ReturnToIdle(CSoundEventManager* manager);   // 0x00C21560
    void Stop(CSoundEventManager* manager);
};

// Stand-in bodies of the three state helpers (same TU in retail, compiled before Stop): they keep `this` in ecx,
// which is what lets the caller skip reloading it. Only Stop is graded from this file.
#pragma auto_inline(off)
void CSoundEvent::CheckState(long event)
{
    switch (m_state) {
    case 0: Event = event; break;
    case 1: Event = 1; break;
    case 2: Event = 2; break;
    case 3: Event = 3; break;
    case 4: Event = 4; break;
    case 5: Event = 5; break;
    case 6: Event = 6; break;
    }
}
long CSoundEvent::IsWaitingOrPlayingPhysical()
{
    CheckState(-1);
    long state = m_state;
    if (state >= 2 && (state <= 4 || state == 6))
        return 1;
    return 0;
}
long CSoundEvent::IsPlayingVirtual_00c207a0()
{
    CheckState(-1);
    return m_state == 5;
}
#pragma auto_inline(on)

void CSoundEvent::Stop(CSoundEventManager* manager)
{
    CheckState(-1);
    if (IsWaitingOrPlayingPhysical() || IsPlayingVirtual_00c207a0())
        ReturnToIdle(manager);
    CheckState(1);
    if (m_pGroup)
        m_pGroup->RemoveAgent(manager, this);
    if (m_listening)
        manager->m_list1F4.Remove(this);
    manager->m_list1C4.Remove(this);
    manager->m_list104.Remove(this);
    m_sample.Reset();
    Pending = 0;
    m_state = 0;
    Index = -1;
    manager->m_eventSet.Add(this);
}