// C2DAudioObject::SetSpeakerGains(const SSpeakerGainTable_00c44680* gains) -- merge a per-speaker gain list into the object's gain table (+0x108): for each requested speaker, entries before its match are zeroed and the matching entry takes the new gain, raising dirty bit 0x800 of +0x14 on a change; pending bits 0x602 also force 0x800 first
// Leaf: ?SetSpeakerGains@C2DAudioObject@@QAEXPBUSSpeakerGainTable_00c44680@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44680, 151 bytes, thiscall `ret 4`.
// Identity (descriptive method and record names; libpk has no PDB or RTTI): the float twin of SetSpeakerLevels
//   0x00C445F0 over the +0x108 gain table of the landed InitSpeakerMaps 0x00C448D0 ({speaker bit, gain} entries, built at
//   1.0). Only caller the one-line libpk API wrapper at 0x00C35F90. Catalog gapscan name is a placeholder.
// Shape: as 0x00C445F0, with the gain compared by `fld [new] ; fld [entry] ; fucompp ; test ah,44h ; jnp` and the
//   zero / copy stores as integer moves.
struct SSpeakerLevel_00c445f0 {                // one speaker's level in millibels
    unsigned long m_speaker;                   // +0x00 speaker bit
    long          m_level;                     // +0x04
};
struct SSpeakerLevelTable_00c445f0 {           // counted entry list (header of the +0x10C table)
    long                    m_count;           // +0x00
    SSpeakerLevel_00c445f0* m_pEntries;        // +0x04
};
struct SSpeakerGain_00c44680 {                 // one speaker's linear gain
    unsigned long m_speaker;                   // +0x00 speaker bit
    float         m_gain;                      // +0x04
};
struct SSpeakerGainTable_00c44680 {            // counted entry list (header of the +0x108 table)
    long                   m_count;            // +0x00
    SSpeakerGain_00c44680* m_pEntries;         // +0x04
};

class C2DAudioObject {                         // libpk 2D audio object (layout as the landed InitSpeakerMaps 0x00C448D0)
public:
    void*                        m_vptr;       // +0x00 (vtable 0x012B7B88)
    unsigned long                m_node[2];    // +0x04
    unsigned long                m_caps;       // +0x0c requested capability bits
    unsigned long                m_unknown10;  // +0x10
    unsigned long                m_dirtyFlags; // +0x14
    unsigned char                m_pad18[0x108 - 0x18];
    SSpeakerGainTable_00c44680*  m_pGains;     // +0x108
    SSpeakerLevelTable_00c445f0* m_pLevels;    // +0x10c
    void SetSpeakerLevels(const SSpeakerLevelTable_00c445f0* levels);
    void SetSpeakerGains(const SSpeakerGainTable_00c44680* gains);
};

void C2DAudioObject::SetSpeakerGains(const SSpeakerGainTable_00c44680* gains)
{
    unsigned long pending = m_dirtyFlags & 0x602;
    if (pending)
        m_dirtyFlags = pending | 0x800;
    for (long i = 0; i < gains->m_count; ++i) {
        long j = 0;
        for (; j < m_pGains->m_count; ++j) {
            if (m_pGains->m_pEntries[j].m_speaker == gains->m_pEntries[i].m_speaker)
                break;
            m_pGains->m_pEntries[j].m_gain = 0.0f;
        }
        float* gain = &m_pGains->m_pEntries[j].m_gain;
        if (*gain != gains->m_pEntries[i].m_gain) {
            *gain = gains->m_pEntries[i].m_gain;
            m_dirtyFlags |= 0x800;
        }
    }
}