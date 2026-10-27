// CSoftwareStreamVoice::Setup(CVoiceDesc_00c556e0* desc) -- vtable slot 1 of the GameCODA software stream voice: run the software voice's setup and, when it succeeds, reset the per-channel state: six Q15 levels to 0x7FFF, six float gains to 1.0, the flags at +0x228 / +0x248 cleared, +0x244 zeroed and the current gain to 0x7FFF; returns the base result
// Leaf: ?Setup@CSoftwareStreamVoice@@UAEJPAUCVoiceDesc_00c556e0@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5C1B0, 114 bytes (+ int3 padding), thiscall `ret 4`; slot 1 of the
//   stream voice vtable 0x012B5D70 (landed CSoftwareStreamVoice: SetParameters 0x00C5C2D0, MixMono 0x00C5C800).
// Callee: the software voice's slot 1 @ 0x00C5A1D0 called non-virtually, under the spelling the landed caller 0x00C556E0
//   binds (`?Setup@CSoftwareVoice_00c5a1d0@@UAEJPAUCVoiceDesc_00c556e0@@@Z`; the catalog's "CTCStealth::OnCreate" landing
//   of 0x00C5A1D0 is a propagated label).
// Name: descriptive (GameCODA, no PDB/RTTI), following the landed caller's Setup; +0x248 / +0x24A are the landed
//   MixMono's m_gainRampPending / m_gain. The manifest label `_gapscan::sub_00c5c1b0` is a placeholder.
// Shape: two counted loops (shorts, then floats) that VC7.1 unrolls: the six 0x7FFF shorts are merged into three
//   0x7FFF7FFF dword stores and the 1.0f stores share one register; six explicit assignments give word stores.
struct CVoiceDesc_00c556e0;
struct CSoftwareVoice_00c5a1d0 {                        // the software voice base (constructor 0x00C5B1A0)
    virtual void v00();
    virtual long Setup(CVoiceDesc_00c556e0* desc);      // slot 1 (0x00C5A1D0)
    unsigned char _pad04[0x20c];
};
class CSoftwareStreamVoice : public CSoftwareVoice_00c5a1d0 {   // vtable 0x012B5D70, 0x250 bytes
public:
    virtual long Setup(CVoiceDesc_00c556e0* desc);
    float          m_channelGain[6];                    // +0x210
    bool           m_228;                               // +0x228
    unsigned char  _pad229;
    short          m_channelLevel[6];                   // +0x22a Q15
    unsigned char  _pad236[0xe];
    unsigned long  m_244;                               // +0x244
    bool           m_gainRampPending;                   // +0x248
    unsigned char  _pad249;
    unsigned short m_gain;                              // +0x24a current Q15 gain
    unsigned short m_targetGain;                        // +0x24c
};

long CSoftwareStreamVoice::Setup(CVoiceDesc_00c556e0* desc)
{
    long result = CSoftwareVoice_00c5a1d0::Setup(desc);
    if (result >= 0) {
        m_228 = false;
        for (int i = 0; i < 6; ++i)
            m_channelLevel[i] = 0x7fff;
        for (int j = 0; j < 6; ++j)
            m_channelGain[j] = 1.0f;
        m_244 = 0;
        m_gainRampPending = false;
        m_gain = 0x7fff;
    }
    return result;
}