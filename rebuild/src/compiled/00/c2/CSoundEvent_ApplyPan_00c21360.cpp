// CSoundEvent::ApplyPan(CSoundEventManager* manager, float pan) -- when the parameter block's mode (GetMode) is zero, store pan as the event's 2D position
// Leaf: ?ApplyPan@CSoundEvent@@QAEXPAUCSoundEventManager@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21360, 22 bytes, thiscall, `ret 8`.
// Identity (descriptive): slot 7 of the CSoundEvent vtable 0x012B3674 (only reference 0x012B3690); its slot-6 twin
//   0x00C21310 runs MarkIfMatches 0x00C20E80 when the mode is NON-zero, slot 8 is the landed GetRecordFlag 0x00C21330.
//   libpk has no PDB or RTTI, so the method is named by what it does; the manager argument is unused.
// Callees: CSoundEvent::GetMode @ 0x00C20EE0, CSoundEvent::Set2DPosition(float) @ 0x00C20EB0 (landed).
// Shape: `this` stays in ecx across GetMode, whose body (and CheckState's) is compiled above in the same TU; the
//   float argument is forwarded as a dword (`mov eax,[esp+8] ; push eax`).
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
__declspec(noinline) long CSoundEvent::GetMode()
{
    CheckState(-1);
    return m_mode;
}
#pragma auto_inline(on)

void CSoundEvent::ApplyPan(CSoundEventManager* manager, float pan)
{
    if (!GetMode())
        Set2DPosition(pan);
}