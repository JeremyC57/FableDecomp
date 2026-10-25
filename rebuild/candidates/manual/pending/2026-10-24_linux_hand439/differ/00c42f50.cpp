// CAudioObject::SetGain(float gain) -- set the audio object's linear gain (+0x18) and, when it changed, its DirectSound-style volume in hundredths of a decibel (+0x1C: 2000 * log10(gain), floored at -10000, -10000 for silence) and dirty bit 2 of +0x14
// Grade: DIFFER(ours v retail) -- retail_asm candidate: retail rounds through a bare `fstp [esp+8] ; fld [esp+8] ; fistp dword ptr [esp]` (an inline-asm float-to-long helper whose by-value float parameter is homed in the dead argument slot); C++ `(long)x` calls __ftol2
// Leaf: ?SetGain@CAudioObject@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42F50, 96 bytes, __thiscall `ret 4`, one `push ecx` frame word for the result.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller: the `__stdcall` API wrapper 0x00C35AF0
//   (landed; reached from the landed CCodaPlayback SetSourceDecibels 0x00C49420). Same object as the slot-6/7
//   setters 0x00C43780 / 0x00C437A0 (dirty flags +0x14). The manifest's "Audio_SetDecibels" describes the effect.
// Constants: 0.0f @ 0x0122DEDC, 2000.0f @ 0x012A2AD4; -10000 = DSBVOLUME_MIN.
// Retail-asm evidence: 0x00C42F8B `fstp dword ptr [esp+8] ; fld dword ptr [esp+8] ; fistp dword ptr [esp] ; mov eax,[esp]`
//   -- the rounding helper of the engine's particle/vertex code family (GOTCHAS wave 25 chunk 145/150); VC7.1 emits
//   `call __ftol2` for the cast and `/QIfist` a qword `fistp` with no float reload. Lands only as grade retail_asm.
// Shape otherwise: `fld gain ; fld [ecx+18h] ; fucompp ; test ah,44h ; jnp` = `m_gain != gain`; the float copied as a
//   dword; `0.0f` loaded from the pool before the reload of the argument; the clamp `cmp eax,-10000 ; jge`.
extern "C" double __cdecl log10(double);
#pragma intrinsic(log10)

class CAudioObject {
public:
    unsigned char m_pad00[0x14];
    unsigned long m_dirtyFlags;                 // +0x14
    float         m_gain;                       // +0x18 linear gain
    long          m_volume;                     // +0x1c hundredths of a decibel
    void SetGain(float gain);
};

static inline long FloatToLong(float value)
{
    return (long)value;                         // retail: inline-asm `fld value ; fistp result`
}

void CAudioObject::SetGain(float gain)
{
    if (m_gain != gain) {
        m_gain = gain;
        long volume;
        if (gain == 0.0f)
            volume = -10000;
        else {
            volume = FloatToLong((float)log10(gain) * 2000.0f);
            if (volume < -10000)
                volume = -10000;
        }
        m_volume = volume;
        m_dirtyFlags |= 2;
    }
}
