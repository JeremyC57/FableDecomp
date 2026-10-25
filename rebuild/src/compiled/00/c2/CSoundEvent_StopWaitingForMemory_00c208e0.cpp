// CSoundEvent::StopWaitingForMemory(CSoundEventManager* manager) -- state 4 -> 1: after the state check the event leaves the manager's waiting-for-memory list (+0x194, slot 8 = Remove), its embedded virtual player (+0x78) reads the level into the event's +4 record (slot 2), then the state word is set to 1
// Leaf: ?StopWaitingForMemory@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C208E0, 59 bytes, `ret 4`. Renamed (sweep1, 2026-10-25) from
//   _unattributed::EnterStateAndAttach (CStateMachineObject; the manifest's "CScrollableList::OnDeactivate" is
//   propagated). Identity (agent434 NOTES; FINDINGS chunk 434): libpk has no PDB/RTTI; this is the 0xB4-byte CSoundEvent
//   (vtable 0x012B3674, state word +0xAC) of the CSoundEventManager (vtable 0x012B2748), next to its siblings
//   WaitForMemory 0x00C20880 (joins the same +0x194 list) and StopWaitingForData 0x00C20920 (the +0x134 twin of this
//   body); state 4 is the describer 0x00C21040's "Waiting for memory". The old comment read CheckState(4) as "switch to
//   state 4" and list slot 8 as "register": slot 8 is Remove, slot 9 Add. Callers 0x00C215EB and 0x00C219F9.
// Callees: CSoundEvent::CheckState @ 0x00C20720 (`this` saved in esi: no same-TU body here), the manager's
//   waiting-for-memory list at +0x194 slot 8 (+0x20) with `this`, the embedded virtual player at +0x78 slot 2 (+8)
//   ReadLevel(manager, &m_level).
// Shape: `push 4 ; mov esi,ecx ; call` (the argument is loaded only after the first call), both embedded
//   objects are member OBJECTS (`mov eax,[edi+194h] ; lea ecx,[edi+194h]`), arguments pushed right to left,
//   the final `mov word ptr [esi+0ACh],1` scheduled after `pop edi`.
struct CSoundEventManager;
struct CSoundEvent;

struct SEventLevel {                             // the 8-byte level record at +4
    float m_gain;                                // +0
    long  m_muted;                               // +4
};

struct CEventList {                              // embedded polymorphic event list of the manager
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void Remove(CSoundEvent* event);     // slot 8 (+0x20)
};

struct CSoundEventManager {
    unsigned char _pad_0x00[0x194];
    CEventList m_waitingForMemory;               // +0x194 (state 4)
};

struct CSoundPlayer {                            // the event's embedded virtual player (+0x78)
    virtual void v0(); virtual void v1();
    virtual void ReadLevel(CSoundEventManager* manager, SEventLevel* level);   // slot 2 (+8)
};

struct CSoundEvent {                             // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void*          __vftable;                    // +0x00
    SEventLevel    m_level;                      // +0x04
    unsigned char  _pad_0x0c[0x78 - 0x0c];
    CSoundPlayer   m_virtualPlayer;              // +0x78
    unsigned char  _pad_0x7c[0xac - 0x7c];
    unsigned short m_state;                      // +0xAC
    void CheckState(long expected);              // 0x00C20720
    void StopWaitingForMemory(CSoundEventManager* manager);
};

void CSoundEvent::StopWaitingForMemory(CSoundEventManager* manager)
{
    CheckState(4);
    manager->m_waitingForMemory.Remove(this);
    m_virtualPlayer.ReadLevel(manager, &m_level);
    m_state = 1;
}
