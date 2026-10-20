#include <cstdio>
#include <cstring>
struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
static const SGuid g_ReverbEffectGuid = { 0x0cf95c8f, 0xa3cc, 0x4849, { 0xb0, 0xb6, 0x83, 0x2e, 0xcc, 0x18, 0x22, 0xdf } };
struct SEffectSlot { SGuid EffectId; long RoomOffset; long _reserved14; unsigned long Flags; };
struct SListenerProperties { long Room; float AirAbsorptionHF; float HFReference; long Other; };
struct CLight {
    long m_currentSlot;
    SEffectSlot m_slots[3];
    SListenerProperties m_slotProperties[3];
    float m_defaultAirAbsorptionHF, m_defaultHFReference;
    SListenerProperties m_committed;
    unsigned long m_dirty;
    long SetSlotEnabled(long slot, long enabled)
    {
        if (enabled != ((m_slots[slot].Flags & 1) == 1)) {
            if (enabled) {
                m_slots[slot].Flags |= 1;
                if (m_currentSlot == slot) {
                    if (std::memcmp(&m_slots[slot].EffectId, &g_ReverbEffectGuid, sizeof(SGuid)) == 0) {
                        m_committed = m_slotProperties[slot];
                        m_committed.Room += m_slots[slot].RoomOffset;
                        if (m_committed.Room < -10000) m_committed.Room = -10000;
                        m_dirty |= 0x5000000;
                    }
                }
                return 0;
            } else {
                m_slots[slot].Flags &= ~1;
                if (m_currentSlot == slot) {
                    m_committed.Room = -10000;
                    m_committed.HFReference = m_defaultHFReference;
                    m_committed.AirAbsorptionHF = m_defaultAirAbsorptionHF;
                    m_dirty |= 0x4180008;
                }
            }
        }
        return 0;
    }
};
int main()
{
    CLight l; std::memset(&l, 0, sizeof(l));
    l.m_currentSlot = 1; l.m_slots[1].EffectId = g_ReverbEffectGuid; l.m_slots[1].RoomOffset = -9500;
    l.m_slotProperties[1].Room = -1000; l.m_slotProperties[1].Other = 7;
    l.m_defaultAirAbsorptionHF = -5.0f; l.m_defaultHFReference = 5000.0f;
    l.SetSlotEnabled(1, 1);
    if (!(l.m_slots[1].Flags & 1) || l.m_committed.Room != -10000 || l.m_committed.Other != 7 || l.m_dirty != 0x5000000) { std::printf("00c630d0_TEST FAIL\n"); return 1; }
    l.m_dirty = 0; l.SetSlotEnabled(1, 1);              // no change: nothing happens
    if (l.m_dirty != 0) { std::printf("00c630d0_TEST FAIL\n"); return 1; }
    l.SetSlotEnabled(1, 0);
    if ((l.m_slots[1].Flags & 1) || l.m_committed.HFReference != 5000.0f || l.m_committed.AirAbsorptionHF != -5.0f || l.m_dirty != 0x4180008) { std::printf("00c630d0_TEST FAIL\n"); return 1; }
    l.m_dirty = 0; l.SetSlotEnabled(2, 1);              // not the current slot: flag only
    if (!(l.m_slots[2].Flags & 1) || l.m_dirty != 0) { std::printf("00c630d0_TEST FAIL\n"); return 1; }
    std::printf("00c630d0_TEST PASS\n");
    return 0;
}