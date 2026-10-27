// CSoundEvent::GetGain(CSoundEventManager* manager, long* muted) -- read the event's current level record (GetLevel) and return its gain, storing its muted word through muted
// Leaf: ?GetGain@CSoundEvent@@QAEMPAUCSoundEventManager@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21380, 36 bytes, thiscall, `ret 8`, float result in st0.
// Identity (descriptive): slot 9 of the CSoundEvent vtable 0x012B3674 (only reference 0x012B3698); the landed
//   GetLevel 0x00C20BE0 lists 0x00C2138C as one of its callers. libpk has no PDB or RTTI, so the method is named by
//   what it does.
// Callees: CSoundEvent::GetLevel(CSoundEventManager*, SEventLevel*) @ 0x00C20BE0 (landed binding).
// Shape: the 8-byte level record is a frame local (`sub esp,8 ; lea eax,[esp]`); the gain is loaded (`fld`) before the
//   muted word is stored.
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

float CSoundEvent::GetGain(CSoundEventManager* manager, long* muted)
{
    SEventLevel level;
    GetLevel(manager, &level);
    *muted = level.m_muted;
    return level.m_gain;
}