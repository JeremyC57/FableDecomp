// Behaviour test for 0x00C204A0 CPKVoiceParams::SetMaxDistance(float): the value goes through the record's +8 slot (0x00C1BBA0) and flag bit 3 is set (OR, not assign)
#include <stdio.h>
static float g_last = 0.0f; static int g_called = 0;
struct CPKParamSlot_00c1bb80 { void Set_00c1bba0(float value); };
void CPKParamSlot_00c1bb80::Set_00c1bba0(float value) { g_last = value; ++g_called; }
struct CPKVoiceParams {
    unsigned long         m_flags;
    long                  m_4;
    CPKParamSlot_00c1bb80 m_slot8;
    void SetMaxDistance(float value);
};
void CPKVoiceParams::SetMaxDistance(float value) { m_slot8.Set_00c1bba0(value); m_flags |= 8; }
int main()
{
    CPKVoiceParams p; p.m_flags = 1; p.m_4 = 0;
    p.SetMaxDistance(12.5f);
    if (g_called == 1 && g_last == 12.5f && p.m_flags == 9) { printf("00c204a0_TEST PASS\n"); return 0; }
    printf("00c204a0_TEST FAIL\n"); return 1;
}
