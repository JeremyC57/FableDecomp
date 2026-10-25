// Behaviour test for 00c43530: CAudioObject::CanUseHardwareVoice -- false on a hardware voice, else whether the 2D/3D hardware pool covers the caps
#include <cstdio>

struct SVoiceCaps { unsigned int m_id, m_available, m_objectCaps, m_formatCaps, m_words[2]; };
static SVoiceCaps g_voiceCaps[4];
struct CVoice {
    bool m_hardware;
    virtual bool IsHardware() const { return m_hardware; }
    virtual ~CVoice() { }
};
struct CAudioObject {
    bool m_3d;
    unsigned int m_caps;
    CVoice* m_pVoice;
    virtual bool Is3D() const { return m_3d; }
    virtual ~CAudioObject() { }
    bool CanUseHardwareVoice()
    {
        if (m_pVoice && m_pVoice->IsHardware())
            return false;
        unsigned int supported = Is3D() ? g_voiceCaps[1].m_objectCaps : g_voiceCaps[0].m_objectCaps;
        if (m_caps & ~supported)
            return false;
        return true;
    }
};

int main()
{
    g_voiceCaps[0].m_objectCaps = 0x3;
    g_voiceCaps[1].m_objectCaps = 0xF;
    CVoice hwVoice; hwVoice.m_hardware = true;
    CVoice swVoice; swVoice.m_hardware = false;
    CAudioObject o; o.m_3d = false; o.m_caps = 0x2; o.m_pVoice = 0;
    int ok = 1;
    ok &= o.CanUseHardwareVoice() == true;
    o.m_pVoice = &hwVoice; ok &= o.CanUseHardwareVoice() == false;
    o.m_pVoice = &swVoice; ok &= o.CanUseHardwareVoice() == true;
    o.m_caps = 0x8; ok &= o.CanUseHardwareVoice() == false;      // 2D pool lacks bit 3
    o.m_3d = true; ok &= o.CanUseHardwareVoice() == true;        // 3D pool has it
    if (!ok) { std::printf("00c43530_TEST FAIL\n"); return 1; }
    std::printf("00c43530_TEST PASS\n");
    return 0;
}