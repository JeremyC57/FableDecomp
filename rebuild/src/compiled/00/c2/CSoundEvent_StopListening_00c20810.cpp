// CSoundEvent::StopListening(CSoundEventManager* manager) -- after the any-state check, clear the event's in-behaviour flag (+0xB0) and remove it from the manager's behaviour list (+0x1F4, slot 8 Remove, tail call)
// Leaf: ?StopListening@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20810, 34 bytes, tail `jmp dword ptr [edx+20h]` (the list's Remove, `ret 4` there).
// Identity: the libpk sound event (vtable 0x012B3674, no RTTI / PDB; descriptive family names). The landed caller ResetLevel 0x00C20DF0
//   calls it as ?StopListening@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z when the +0xB0 flag is set (the only spelling bound to
//   0x00C20810, reused). Its counterpart is the Initialise path 0x00C21C20 (`mov byte ptr [esi+0B0h],1` after the +0x1F4 list's
//   Add slot 9); the describer 0x00C21040 prints "Emitter ID = <+0x24>" when the flag is set and "Not in behaviour" when clear;
//   Stop 0x00C21620 removes a flagged event from the same list. The catalog's "CActionSummonerLightningAttackTarget::OnActivate" is
//   propagated.
// Callees: CSoundEvent::CheckState 0x00C20720 (-1 = any state), the +0x1F4 list's virtual slot 8 (+0x20) Remove(event).
// Shape: the flag store sits between the list-address computation and the vtable load; `this` stays in ecx across CheckState
//   (same-TU stand-in, the landed WaitForData model) and is passed in the reused argument slot for the tail call.
struct CSoundEventManager;
struct CSoundEvent;

struct CEventList {                         // embedded polymorphic event registry of the manager
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CSoundEvent* event);    // slot 8 (+0x20)
    virtual void Add(CSoundEvent* event);       // slot 9 (+0x24)
};

struct CSoundEventManager {
    unsigned char _pad000[0x1F4];
    CEventList m_behaviourEvents;           // +0x1F4 (events with the +0xB0 flag)
};

struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void* __vftable;                        // +0x00
    unsigned char _pad04[0xAC - 0x04];
    unsigned short m_state;                 // +0xAC
    unsigned char _padae[2];
    bool m_listening;                       // +0xB0
    unsigned char _padb1[3];
    void CheckState(long expected);         // 0x00C20720
    void StopListening(CSoundEventManager* manager);
};

// Same-TU body of CheckState 0x00C20720 (the landed WaitForData stand-in); only StopListening is graded from this file.
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

void CSoundEvent::StopListening(CSoundEventManager* manager)
{
    CheckState(-1);
    m_listening = false;
    manager->m_behaviourEvents.Remove(this);
}