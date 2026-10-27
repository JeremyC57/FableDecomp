// Behaviour test for 0x00C44680 C2DAudioObject::SetSpeakerGains
#include <cstdio>
struct E { unsigned int m_speaker; float m_gain; };
struct Tab { int m_count; E* m_pEntries; };
struct Obj {
    unsigned int m_dirtyFlags; Tab* m_pGains;
    void SetSpeakerGains(const Tab* gains)
    {
        unsigned int pending = m_dirtyFlags & 0x602;
        if (pending) m_dirtyFlags = pending | 0x800;
        for (int i = 0; i < gains->m_count; ++i) {
            int j = 0;
            for (; j < m_pGains->m_count; ++j) {
                if (m_pGains->m_pEntries[j].m_speaker == gains->m_pEntries[i].m_speaker) break;
                m_pGains->m_pEntries[j].m_gain = 0.0f;
            }
            float* gain = &m_pGains->m_pEntries[j].m_gain;
            if (*gain != gains->m_pEntries[i].m_gain) { *gain = gains->m_pEntries[i].m_gain; m_dirtyFlags |= 0x800; }
        }
    }
};
static int Fail() { std::printf("00c44680_TEST FAIL\n"); return 1; }
int main()
{
    E own[3] = { { 1, 1.0f }, { 2, 1.0f }, { 4, 1.0f } }; Tab ownT = { 3, own };
    E in[2] = { { 2, 0.5f }, { 1, 0.25f } }; Tab inT = { 2, in };
    Obj o; o.m_dirtyFlags = 0; o.m_pGains = &ownT;
    o.SetSpeakerGains(&inT);
    if (own[0].m_gain != 0.25f || own[1].m_gain != 0.5f || own[2].m_gain != 1.0f || o.m_dirtyFlags != 0x800) return Fail();
    o.m_dirtyFlags = 0; o.SetSpeakerGains(&inT);
    if (own[0].m_gain != 0.25f || o.m_dirtyFlags != 0x800) return Fail(); // speaker 1 is zeroed while seeking 2, then restored
    o.m_dirtyFlags = 0x10602; o.SetSpeakerGains(&inT);
    if (o.m_dirtyFlags != 0xe02) return Fail();
    std::printf("00c44680_TEST PASS\n");
    return 0;
}