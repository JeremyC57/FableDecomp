// CAudioParamBlock::Update() -- advance every ramped gain parameter whose ramp has not finished by one tick (1000 / the owner's tick rate) and, when any moved, refresh the owner's display object and its optional sprite
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0AF20, 108 bytes (`ret`; the sprite update is a tail `jmp`). Descriptive
//   names (no PDB covers this library): the manifest's "NUISystem_CComponent_UpdateAnimation" is propagated. Callers:
//   the end of the landed block constructor 0x00C0B200 ("Finish") and the landed Shutdown 0x00C091F0 (m_pFirst).
// Callees: CRampedParam::Update(float) @ 0x00C0B7B0, the owner's display Refresh @ 0x00C11180 and sprite Update
//   @ 0x00C15B90 (the owner model of the landed SetSlot 0x00C0B380: display at +0x38, sprite at +0x60).
// Constants: 1000.0 @ 0x01252370.
// Shape: the sixteen-slot loop is strength-reduced (esi walks +0x1c of each slot: `fld [esi]` = duration,
//   `fld [esi-4]` = time: `time != duration` loads the right operand first; ebx counts down); the "any moved" flag lives in cl, so the float argument's slot is made by
//   `push ecx` and the flag rematerialised (`mov cl,1`) after the call.
struct CRampedParam {                           // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void Update(float dt);                      // 0x00C0B7B0
};

struct CDisplay { void Refresh(); };            // 0x00C11180
struct CSprite { void Update(); };              // 0x00C15B90

struct CAudioOwner {
    unsigned char _pad0[0x38];
    CDisplay* m_pDisplay;                       // +0x38
    unsigned char _pad3c[0x60 - 0x3c];
    CSprite* m_pSprite;                         // +0x60
    unsigned char _pad64[0x70 - 0x64];
    float m_tickRate;                           // +0x70
};

struct CAudioParamBlock {
    void* m_vtable;                             // +0x000
    CRampedParam m_gains[16];                   // +0x004
    float m_pitchFactors[8];                    // +0x344
    unsigned char _pad364[0x3c4 - 0x364];       // +0x364 the sixteen + eight parameter names (CString)
    CAudioOwner* m_pOwner;                      // +0x3c4
    void Update();
};

void CAudioParamBlock::Update()
{
    bool changed = false;
    for (int i = 0; i < 16; ++i) {
        if (m_gains[i].m_time != m_gains[i].m_duration) {
            m_gains[i].Update(1000.0f / m_pOwner->m_tickRate);
            changed = true;
        }
    }
    if (changed) {
        m_pOwner->m_pDisplay->Refresh();
        if (m_pOwner->m_pSprite)
            m_pOwner->m_pSprite->Update();
    }
}