#include <cstdio>
#include <cstring>
#include <string.h>
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
const SGuid g_EAXReverbEffectGuid = { 0x0C238157, 0x1234, 0x5678, { 1, 2, 3, 4, 5, 6, 7, 8 } };        // 0x012B94B8
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
    long SetSlotVolume(long slot, long volume);
};
long CEAXContext::SetSlotVolume(long slot, long volume)
{
    if (volume < -10000 || volume > 0) {
        m_lastError = -2;
        return -3;
    }
    if (volume != m_slots[slot].lVolume) {
        m_slots[slot].lVolume = volume;
        if (m_currentSlot == slot) {
            if (memcmp(&m_slots[slot].guidLoadEffect, &g_EAXReverbEffectGuid, sizeof(SGuid)) == 0) {
                m_committed.lRoom = volume + m_slotReverb[slot].lRoom;
                if (m_committed.lRoom < -10000)
                    m_committed.lRoom = -10000;
                m_dirty |= 8;
            }
        }
    }
    return 0;
}
static int fail() { std::printf("00c67720_TEST FAIL\n"); return 1; }
int main()
{
    static CEAXContext c;
    std::memset(&c, 0, sizeof(c));
    if (c.SetSlotVolume(0, 1) != -3 || c.m_lastError != -2) return fail();
    c.m_lastError = 0;
    if (c.SetSlotVolume(0, -10001) != -3 || c.m_lastError != -2) return fail();
    c.m_lastError = 0;
    // not the current slot: stored, nothing committed
    c.m_currentSlot = 1;
    if (c.SetSlotVolume(0, -500) != 0 || c.m_slots[0].lVolume != -500 || c.m_dirty != 0) return fail();
    // current slot but not a reverb effect: stored only
    if (c.SetSlotVolume(1, -300) != 0 || c.m_slots[1].lVolume != -300 || c.m_dirty != 0) return fail();
    // current reverb slot: room = slot room + volume
    c.m_slots[1].guidLoadEffect = g_EAXReverbEffectGuid;
    c.m_slotReverb[1].lRoom = -1000;
    if (c.SetSlotVolume(1, -200) != 0 || c.m_committed.lRoom != -1200 || c.m_dirty != 8) return fail();
    // floored at -10000
    c.m_dirty = 0;
    c.m_slotReverb[1].lRoom = -9000;
    if (c.SetSlotVolume(1, -5000) != 0 || c.m_committed.lRoom != -10000 || c.m_dirty != 8) return fail();
    // unchanged volume: no recommit
    c.m_dirty = 0; c.m_committed.lRoom = 3;
    if (c.SetSlotVolume(1, -5000) != 0 || c.m_committed.lRoom != 3 || c.m_dirty != 0) return fail();
    std::printf("00c67720_TEST PASS\n");
    return 0;
}