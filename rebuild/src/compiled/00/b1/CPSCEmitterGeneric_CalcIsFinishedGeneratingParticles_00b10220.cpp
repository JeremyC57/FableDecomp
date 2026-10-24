// CPSCEmitterGeneric::CalcIsFinishedGeneratingParticles() const -- a lifetime-limited emitter is finished once its emitted count reaches rate x elapsed: the 15-bit packed lifetime (0..300 s, rounded to 2 decimals) times the elapsed global time compared with the particles generated so far
// Leaf: CalcIsFinishedGeneratingParticles
// Retail-asm: 0x00B10286 `fld dword ptr [esp+4] ; fistp dword ptr [esp+8]` -- the engine's inline-assembly ROUND helper (the scaled value is stored to the helper's by-value copy and reloaded for a bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload); the asm block also makes VC spill the elapsed time to [esp+10h] across it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B10220, length 148 bytes (padded to 160).
// Callees: the global particle clock getter @ 0x00AEA9F0 (returns the long @ 0x01399504; manifest "GFGetPreMainMemoryUsed").
// Globals: 4294967296.0f @ 0x0122DCB4 (unsigned fix-up), 1/32767 @ 0x0129C81C, 300.0f @ 0x01238808, 10.0f @ 0x01237F0C, 1.0f @ 0x0122DED8.
// Shape: `unpacked` and the x300 scale are separate statements (VC7.1 folds adjacent constant multiplies); the power-of-ten loop is a do-while; the comparison is written particles > lifetime*elapsed (test ah,41h ; jne).
// Shape: the packed field is decoded by an inline quantiser: unsigned-to-float, scale, then rounded to two decimals with
// an inline power-of-ten loop and an inline float-to-int helper -- retail's helper is a bare dword `fistp` after a
// float store/reload (inline asm in the original): lane gap.
struct CPSCEmitterGeneric {
    unsigned char _pad00[0x28];
    long          m_particlesGenerated;     // +0x28
    unsigned char _pad2c[4];
    unsigned long m_flags;                  // +0x30 (bit 23: lifetime-limited)
    unsigned char _pad34[0xc];
    unsigned long m_packedLifetime;         // +0x40 (low 15 bits)
    bool CalcIsFinishedGeneratingParticles() const;
};

long GetParticleClock_00aea9f0();           // 0x00AEA9F0

// the engine's inline-assembly float-to-long helper, expanded in place (the by-value parameter is homed in a dead slot)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}

bool CPSCEmitterGeneric::CalcIsFinishedGeneratingParticles() const
{
    float elapsed = (float)GetParticleClock_00aea9f0();
    if (m_flags & 0x800000)
    {
        float unpacked = (float)(m_packedLifetime & 0x7fff) * (1.0f / 32767.0f);   // separate statements: VC7.1 would fold the two constants
        float lifetime = RoundToDecimals(unpacked * 300.0f, 2);
        if ((float)m_particlesGenerated > lifetime * elapsed)
            return true;
    }
    return false;
}