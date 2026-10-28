// CSoundEvent::HasPositiveGain(CSoundEventManager* manager) -- in any state, 1 when the event's parameter block yields a gain above zero for this manager, else 0
// Leaf: ?HasPositiveGain@CSoundEvent@@QAEJPAUCSoundEventManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20CB0, 46 bytes, thiscall, `ret 4`. Descriptive names (the libpk audio library
//   has no PDB or RTTI); the class is the landed CSoundEvent (0xB4 bytes, vtable 0x012B3674, state word +0xAC, the
//   sound-parameter block at +0x38; landed 0x00C20C10 IsAudible, 0x00C216D0 YieldPhysicalPlayer).
//   Label correction: the manifest row "CDraw2DText::IsAlphaThresholdExceeded" is propagated.
// Callees: CSoundEvent::CheckState 0x00C20720 (-1 = any state), CSoundParams::GetGain 0x00C34440 (bound by the landed
//   0x00C20A20), the constant 0.0f @ 0x0122DEDC.
// Shape: CheckState's body is defined above the caller under auto_inline(off) (the landed 0x00C216D0 device): VC7.1 then
//   knows it preserves ecx, so `this` stays in ecx across it (`add ecx,38h` with no `mov esi,ecx`), as in retail's LTCG
//   build. `fcomp ; test ah,41h ; jne` is `gain > 0.0f`; the result is a 32-bit int (`mov eax,1` / `xor eax,eax`).
struct CSoundEventManager;

struct CSoundParams {                           // +0x38 of the event
    float GetGain(CSoundEventManager* manager);  // 0x00C34440
};

class CSoundEvent {
public:
    unsigned char  _pad00[0x38];
    CSoundParams   m_params;                    // +0x38
    unsigned char  _pad39[0xac - 0x39];
    unsigned short m_state;                     // +0xAC
    unsigned char  _padae[0x6];
    void CheckState(long expected);             // 0x00C20720
    long HasPositiveGain(CSoundEventManager* manager);
};

#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

long CSoundEvent::HasPositiveGain(CSoundEventManager* manager)
{
    CheckState(-1);
    if (m_params.GetGain(manager) > 0.0f)
        return 1;
    return 0;
}