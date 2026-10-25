// Behaviour test for 00c43d30: CAudioObject::CanUseBuffer -- streamed needs hardware, 3D needs mono, stereo flag must match channels
#include <cstdio>

static int g_error = 0;
struct SBufferDesc { unsigned int m_mode, m_channels; };
struct CSoundBuffer {
    unsigned int m_state; SBufferDesc m_desc;
    bool IsStreamed() const { return (m_state >> 2) & 1; }
    const SBufferDesc* GetDesc() const { return &m_desc; }
};
struct SVoiceCaps { unsigned int m_id, m_available, m_objectCaps, m_formatCaps, m_words[2]; };
static SVoiceCaps g_voiceCaps[4];
struct CVoice { bool m_hardware; bool IsHardware() const { return m_hardware; } };

struct CAudioObject {
    bool m_3d; unsigned int m_caps, m_flags; void* m_pLoopPoints; void* m_pVolumeParams; CVoice* m_pVoice;
    bool Is3D() const { return m_3d; }
    bool IsStereo() const { return (m_flags >> 16) & 1; }
    bool CanUseHardwareVoice()
    {
        if (m_pVoice && m_pVoice->IsHardware())
            return false;
        unsigned int supported = Is3D() ? g_voiceCaps[1].m_objectCaps : g_voiceCaps[0].m_objectCaps;
        if (m_caps & ~supported)
            return false;
        return true;
    }
    bool CanUseBuffer(CSoundBuffer* buffer)
    {
        bool ok = true;
        if (buffer->IsStreamed() && !CanUseHardwareVoice())
            ok = false;
        else if (Is3D() && buffer->GetDesc()->m_channels > 1)
            ok = false;
        else if (IsStereo() || m_pLoopPoints || m_pVolumeParams) {
            if (buffer->GetDesc()->m_channels == 1 && IsStereo())
                ok = false;
            else if (buffer->GetDesc()->m_channels > 1 && !IsStereo())
                ok = false;
        }
        if (!ok) {
            g_error = -14;
            return false;
        }
        return true;
    }
};

int main()
{
    g_voiceCaps[0].m_objectCaps = 1; g_voiceCaps[1].m_objectCaps = 1;
    CSoundBuffer mono = { 0, { 0, 1 } }, stereo = { 0, { 0, 2 } }, streamed = { 4, { 0, 1 } };
    CVoice hw = { true };
    int loop = 0;
    CAudioObject o; o.m_3d = false; o.m_caps = 1; o.m_flags = 0; o.m_pLoopPoints = 0; o.m_pVolumeParams = 0; o.m_pVoice = 0;
    int ok = 1;
    ok &= o.CanUseBuffer(&mono) && o.CanUseBuffer(&stereo) && o.CanUseBuffer(&streamed) && g_error == 0;
    o.m_pVoice = &hw; ok &= !o.CanUseBuffer(&streamed) && g_error == -14; g_error = 0; o.m_pVoice = 0;
    o.m_caps = 2; ok &= !o.CanUseBuffer(&streamed); o.m_caps = 1; g_error = 0;
    o.m_3d = true; ok &= o.CanUseBuffer(&mono) && !o.CanUseBuffer(&stereo); o.m_3d = false; g_error = 0;
    o.m_pLoopPoints = &loop; ok &= o.CanUseBuffer(&mono) && !o.CanUseBuffer(&stereo); g_error = 0;
    o.m_flags = 0x10000; ok &= !o.CanUseBuffer(&mono) && o.CanUseBuffer(&stereo);
    if (!ok) { std::printf("00c43d30_TEST FAIL\n"); return 1; }
    std::printf("00c43d30_TEST PASS\n");
    return 0;
}