#include <cstdio>
#include <cstring>
struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct SEAXFXSlotProperties {                     // EAX 4.0 EAXFXSLOTPROPERTIES, 0x1c bytes
    SGuid         guidLoadEffect;                 // +0x00
    long          lVolume;                        // +0x10 -10000..0 mB
    long          lLock;                          // +0x14
    unsigned long ulFlags;                        // +0x18 bit 0 EAXFXSLOTFLAGS_ENVIRONMENT
};
struct SEAXVector { float x, y, z; };
struct SEAXReverbProperties {                     // EAX 3.0/4.0 EAXREVERBPROPERTIES, 0x70 bytes
    unsigned long ulEnvironment;                  // +0x00
    float         flEnvironmentSize;              // +0x04
    float         flEnvironmentDiffusion;         // +0x08
    long          lRoom;                          // +0x0c
    long          lRoomHF;                        // +0x10
    long          lRoomLF;                        // +0x14
    float         flDecayTime;                    // +0x18
    float         flDecayHFRatio;                 // +0x1c
    float         flDecayLFRatio;                 // +0x20
    long          lReflections;                   // +0x24
    float         flReflectionsDelay;             // +0x28
    SEAXVector    vReflectionsPan;                // +0x2c
    long          lReverb;                        // +0x38
    float         flReverbDelay;                  // +0x3c
    SEAXVector    vReverbPan;                     // +0x40
    float         flEchoTime;                     // +0x4c
    float         flEchoDepth;                    // +0x50
    float         flModulationTime;               // +0x54
    float         flModulationDepth;              // +0x58
    float         flAirAbsorptionHF;              // +0x5c
    float         flHFReference;                  // +0x60
    float         flLFReference;                  // +0x64
    float         flRoomRolloffFactor;            // +0x68
    unsigned long ulFlags;                        // +0x6c EAXREVERBFLAGS_* (bits 0..7)
};
struct CEAXContext {
    unsigned char        _pad000[0x4];
    long                 m_currentSlot;           // +0x04
    unsigned char        _pad008[0x84 - 0x8];
    SEAXFXSlotProperties m_slots[4];              // +0x84
    SEAXReverbProperties m_slotReverb[4];         // +0xf4
    unsigned char        _pad2b4[0x340 - 0x2b4];
    SEAXReverbProperties m_committed;             // +0x340
    unsigned char        _pad3b0[0x3bc - 0x3b0];
    unsigned long        m_dirty;                 // +0x3bc
    unsigned char        _pad3c0[0x3d4 - 0x3c0];
    long                 m_lastError;             // +0x3d4
    long SetSlotEnabled(long slot, long enabled); // 0x00C630D0
    long SetSlotFlags(long slot, unsigned long flags);
};
long CEAXContext::SetSlotFlags(long slot, unsigned long flags)
{
    long result = 0;
    if (flags & ~1UL)
        return -3;
    if (flags != m_slots[slot].ulFlags)
        result = SetSlotEnabled(slot, (flags & 1) == 1);
    return result;
}
static int g_calls, g_slot, g_enabled;
long CEAXContext::SetSlotEnabled(long slot, long enabled)
{
    ++g_calls; g_slot = slot; g_enabled = enabled;
    if (enabled) m_slots[slot].ulFlags |= 1; else m_slots[slot].ulFlags &= ~1UL;
    return 7;
}
static int fail() { std::printf("00c67430_TEST FAIL\n"); return 1; }
int main()
{
    static CEAXContext c;
    std::memset(&c, 0, sizeof(c));
    if (c.SetSlotFlags(1, 2) != -3 || g_calls != 0 || c.m_lastError != 0) return fail();
    if (c.SetSlotFlags(1, 0) != 0 || g_calls != 0) return fail();               // unchanged
    if (c.SetSlotFlags(2, 1) != 7 || g_calls != 1 || g_slot != 2 || g_enabled != 1) return fail();
    if (c.m_slots[2].ulFlags != 1) return fail();
    if (c.SetSlotFlags(2, 1) != 0 || g_calls != 1) return fail();               // unchanged now
    if (c.SetSlotFlags(2, 0) != 7 || g_calls != 2 || g_enabled != 0 || c.m_slots[2].ulFlags != 0) return fail();
    std::printf("00c67430_TEST PASS\n");
    return 0;
}