// CSoundEvent::GetMode() -- settle the state (CheckState(-1)) and return the parameter block's mode word (+0x48)
// Leaf: ?GetMode@CSoundEvent@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20EE0, 11 bytes: push -1 ; call 0x00C20720 ; mov eax,[ecx+48h] ; ret
// Identity (descriptive): the landed CSoundEvent::GetRecordFlag 0x00C21330 declares it as `long GetMode() const;
//   // 0x00C20EE0` (the event's parameter-block mode); callers 0x00C211B5, 0x00C212F0, 0x00C21310, 0x00C21333,
//   0x00C21360, 0x00C21427, 0x00C2143F, all CSoundEvent members. The catalog's
//   "CCombatActionBase::GetAnimationTransitionInTime" and the stale binding ?GetValue48@CPlayer@@QBEJXZ are propagated
//   game names; libpk has no PDB or RTTI (CSoundEvent is the landed descriptive class, vtable 0x012B3674).
// Callees: CSoundEvent::CheckState(long) @ 0x00C20720 (bound ?CheckState@CSoundEvent@@QAEXJ@Z; -1 = any state).
// Shape: `this` stays in ecx across the call because CheckState is compiled above it in the same TU and leaves ecx
//   alone (a declaration-only callee makes VC save `this` in esi). Not const: it calls the non-const CheckState.
struct CSoundEventManager;

struct SEventLevel {                            // the 8-byte level record at +4
    float m_gain;                               // +0
    long  m_muted;                              // +4
};

struct CSoundEvent {                            // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void*         __vftable;                    // +0x00
    SEventLevel   m_level;                      // +0x04
    unsigned char _pad0c[0x48 - 0x0c];
    long          m_mode;                       // +0x48 (the parameter block's mode word, block +0x38)
    unsigned char _pad4c[0xac - 0x4c];
    unsigned short m_state;                     // +0xAC
    unsigned char _padae[0xb4 - 0xae];
    void CheckState(long expected);             // 0x00C20720
    long GetMode();                             // 0x00C20EE0
    void Set2DPosition(float pan);              // 0x00C20EB0
    void GetLevel(CSoundEventManager* manager, SEventLevel* level);   // 0x00C20BE0
    void ApplyPan(CSoundEventManager* manager, float pan);
    float GetGain(CSoundEventManager* manager, long* muted);
};

// Same-TU body of CheckState 0x00C20720 (retail compiles it above its callers): a switch on the state word whose
// cases are all empty (their asserts are compiled out). VC7.1 tracks the register use of callees compiled earlier in
// the TU, so the callers keep `this` in ecx across the call. (VC7.1 folds the empty switch to `ret 4` where retail keeps
// its jump table; only the function named on line 1 is graded from this file.)
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}

long CSoundEvent::GetMode()
{
    CheckState(-1);
    return m_mode;
}
#pragma auto_inline(on)