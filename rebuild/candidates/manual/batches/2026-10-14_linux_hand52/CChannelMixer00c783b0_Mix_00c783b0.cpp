// CChannelMixer_00c783b0::Mix(SChannelSet_00c783b0* set) -- mix the set's channel block (+0x58) into the four-wide record at +0x10 and the two-wide record at +0x20 (0x00C77D90, `__cdecl`: destination, source, width, the limit at +0x28 -> +4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C783B0, length 64 bytes.
// Callee: 0x00C77D90 (labelled "Audio_MixChannels") twice, `add esp,20h` once. The chunk label "CKilledThing::ModifyHeroStats" is propagated; named by effect. Shape: the set's block address in edi across both calls, `add esi,20h` for the second record, `ret 4`.
struct SChannelSet_00c783b0 {
    unsigned char _pad[0x58];
    float         m_channels[1];                   // +0x58
};
struct SLimits_00c783b0 {
    unsigned char _pad[4];
    long          m_limit;                         // +0x04
};
void __cdecl MixChannels_00c77d90(float* source, float* destination, long width, long limit);   // 0x00C77D90
struct CChannelMixer_00c783b0 {
    unsigned char      _pad[0x10];
    float              m_quad[4];                  // +0x10
    float              m_pair[2];                  // +0x20
    SLimits_00c783b0*  m_pLimits;                  // +0x28
    void Mix(SChannelSet_00c783b0* set);
};

void CChannelMixer_00c783b0::Mix(SChannelSet_00c783b0* set)
{
    MixChannels_00c77d90(set->m_channels, m_quad, 4, m_pLimits->m_limit);
    MixChannels_00c77d90(set->m_channels, m_pair, 2, m_pLimits->m_limit);
}
