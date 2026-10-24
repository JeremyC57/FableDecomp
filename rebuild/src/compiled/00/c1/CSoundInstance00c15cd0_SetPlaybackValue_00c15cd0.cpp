// CSoundInstance_00c15cd0::SetPlaybackValue(unsigned long value) -- under the module lock, forwards the unsigned value as a float to the player object at +8 (which applies it to both of its 0x180-byte voices)
// Leaf: SetPlaybackValue
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15CD0, 114 bytes (+ int3 padding), __fastcall `ret 4`, EH-framed.
// Identity: a libpk sound class (vtable slot at 0x012B2A7C; no RTTI). Callees: the module's scoped lock over the
//   critical section @ 0x0143BB14 (@ 0x00C0FFC0 / 0x00C0FFE0, state 0) and the player's float setter @ 0x00C2D210
//   (clears its +0x33c byte, then feeds the value to each idle voice).
// Shape: `(float)value` of an unsigned long is `fild` plus the 2^32 correction (`test eax,eax ; jge ; fadd
//   4294967296.0f` @ 0x0122DCB4), the state store scheduled into the test; the float goes through `push ecx ;
//   fstp [esp]`.
// Label correction: the catalog's `Combat_ProcessAnimationByValue` is propagated; descriptive names.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CScopedLock_00c15cd0 {              // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock_00c15cd0(void* section);   // 0x00C0FFC0
    ~CScopedLock_00c15cd0();               // 0x00C0FFE0
};

struct CSoundPlayer_00c15cd0 {
    void SetValue(float value);            // 0x00C2D210
};

struct CSoundInstance_00c15cd0 {
    void* m_vtable;                        // +0x00
    long m_4;
    CSoundPlayer_00c15cd0* m_pPlayer;      // +0x08
    void SetPlaybackValue(unsigned long value);
};

void CSoundInstance_00c15cd0::SetPlaybackValue(unsigned long value)
{
    CScopedLock_00c15cd0 lock(g_section_0143bb14);
    m_pPlayer->SetValue((float)value);
}