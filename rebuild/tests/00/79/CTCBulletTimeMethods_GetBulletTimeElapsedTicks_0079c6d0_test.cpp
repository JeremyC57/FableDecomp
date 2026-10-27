#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
long g_TicksPerSecond = 15;
class CTCBulletTime { public:
    void* __vftable; long f4; unsigned char _pad_0x08[4]; long BulletTimeRemainingTicks; long BulletTimeAbilityLevel;
    unsigned char _pad[0x14]; float m_fDuration;
    long GetBulletTimeTicksFromAbilityLevel() const { return CeilToLong(g_TicksPerSecond * m_fDuration); }
    float GetBulletTimeRemainingPercentage() const { long remaining = BulletTimeRemainingTicks; return (float)remaining / CeilToLong(g_TicksPerSecond * m_fDuration); }
    long GetBulletTimeElapsedTicks() const { return CeilToLong(g_TicksPerSecond * m_fDuration) - BulletTimeRemainingTicks; }
};
int main()
{
    CTCBulletTime t; t.m_fDuration = 2.0f; t.BulletTimeRemainingTicks = 12;
    bool ok = t.GetBulletTimeElapsedTicks() == 18;
    t.m_fDuration = 2.1f; t.BulletTimeRemainingTicks = 0; ok = ok && t.GetBulletTimeElapsedTicks() == 32;
    std::printf(ok ? "0079c6d0_TEST PASS\n" : "0079c6d0_TEST FAIL\n");
    return ok ? 0 : 1;
}