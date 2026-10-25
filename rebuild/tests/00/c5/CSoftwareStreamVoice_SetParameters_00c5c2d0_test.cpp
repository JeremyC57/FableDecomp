// Behaviour test for CSoftwareStreamVoice::SetParameters (0x00C5C2D0): the base mask plus 0x1000 for buffer flag
// +0x12 bit 0, 0x80 / 0x100 when the description or (non-stream buffers only) the sample header asks for that filter,
// and both filters reset.
#include <cstdio>
struct SVoiceDesc { unsigned int m_flags, m_filter0, m_filter1; };
struct SSampleInfo { unsigned int m_filter0, m_filter1; };
struct CCodaBuffer { bool m_stream; unsigned char m_bufferFlags; SSampleInfo m_info; bool IsStream() { return m_stream; } };
static int g_resets = 0, g_base = 0;
struct CVoiceFilter { void Reset() { ++g_resets; } };
struct CSoftwareStreamVoice {
    CVoiceFilter m_filters[2];
    unsigned int BaseSetParameters(CCodaBuffer*, const SVoiceDesc* desc) { ++g_base; return desc->m_flags; }
    unsigned int SetParameters(CCodaBuffer* buffer, const SVoiceDesc* desc)
    {
        CCodaBuffer* sample = buffer->IsStream() ? 0 : buffer;
        unsigned int caps = BaseSetParameters(buffer, desc);
        if (buffer->m_bufferFlags & 1) caps |= 0x1000;
        if (desc->m_filter0 || (sample && sample->m_info.m_filter0)) caps |= 0x80;   // retail reads through the null base only when the description is 0
        if (desc->m_filter1 || (sample && sample->m_info.m_filter1)) caps |= 0x100;
        m_filters[0].Reset(); m_filters[1].Reset();
        return caps;
    }
};
int main()
{
    CSoftwareStreamVoice v;
    CCodaBuffer b = { false, 1, { 1, 0 } };
    SVoiceDesc d = { 4, 0, 0 };
    int ok = v.SetParameters(&b, &d) == (4u | 0x1000 | 0x80) && g_resets == 2 && g_base == 1;
    b.m_bufferFlags = 2; b.m_info.m_filter0 = 0; b.m_info.m_filter1 = 7;
    ok = ok && v.SetParameters(&b, &d) == (4u | 0x100);
    b.m_stream = true; d.m_filter0 = 1; d.m_filter1 = 1;
    ok = ok && v.SetParameters(&b, &d) == (4u | 0x80 | 0x100) && g_resets == 6;
    if (!ok) { std::printf("00c5c2d0_TEST FAIL\n"); return 1; }
    std::printf("00c5c2d0_TEST PASS\n");
    return 0;
}