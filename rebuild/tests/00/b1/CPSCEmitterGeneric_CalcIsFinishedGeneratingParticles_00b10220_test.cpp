#include <cstdio>
struct CPSCEmitterGeneric { unsigned char _pad00[0x28]; long m_particlesGenerated; unsigned char _pad2c[4]; unsigned long m_flags; unsigned char _pad34[0xc]; unsigned long m_packedLifetime; bool CalcIsFinishedGeneratingParticles() const; };
static long g_clock = 0;
long GetParticleClock_00aea9f0() { return g_clock; }
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f; long i = decimals;
    do scale *= 10.0f; while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}
bool CPSCEmitterGeneric::CalcIsFinishedGeneratingParticles() const
{
    float elapsed = (float)GetParticleClock_00aea9f0();
    if (m_flags & 0x800000)
    {
        float unpacked = (float)(m_packedLifetime & 0x7fff) * (1.0f / 32767.0f);
        float lifetime = RoundToDecimals(unpacked * 300.0f, 2);
        if ((float)m_particlesGenerated > lifetime * elapsed)
            return true;
    }
    return false;
}
int main()
{
    CPSCEmitterGeneric e = {}; e.m_flags = 0; e.m_particlesGenerated = 1000; e.m_packedLifetime = 0x7fff; g_clock = 1;
    bool ok = !e.CalcIsFinishedGeneratingParticles();                 // not lifetime-limited
    e.m_flags = 0x800000; ok = ok && e.CalcIsFinishedGeneratingParticles();   // 1000 > 300*1
    g_clock = 4; ok = ok && !e.CalcIsFinishedGeneratingParticles();            // 1000 > 1200 is false
    e.m_packedLifetime = 0x7fff / 2; g_clock = 7; ok = ok && !e.CalcIsFinishedGeneratingParticles();  // lifetime 149.99 (2 decimals) * 7 = 1049.93 > 1000 -> false
    ok = ok && !(e.m_particlesGenerated > 149.99f * 7);
    std::printf(ok ? "00b10220_TEST PASS\n" : "00b10220_TEST FAIL\n"); return ok ? 0 : 1;
}