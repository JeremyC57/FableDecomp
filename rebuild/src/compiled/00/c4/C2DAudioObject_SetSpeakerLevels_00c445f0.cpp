// C2DAudioObject::SetSpeakerLevels(const SSpeakerLevelTable_00c445f0* levels) -- merge a per-speaker level list (millibels) into the object's level table (+0x10C): for each requested speaker, entries before its match are silenced (-10000 mB) and the matching entry takes the new level, raising dirty bit 0x400 of +0x14 on a change; pending bits 0xA02 also force 0x400 first
// Leaf: ?SetSpeakerLevels@C2DAudioObject@@QAEXPBUSSpeakerLevelTable_00c445f0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C445F0, 142 bytes, thiscall `ret 4`.
// Identity (descriptive method and record names; libpk has no PDB or RTTI): it walks the +0x10C speaker table the
//   landed C2DAudioObject::InitSpeakerMaps 0x00C448D0 builds (header {count, entries}, 8-byte {speaker bit, level}
//   entries); -10000 (0xFFFFD8F0) is the libpk millibel floor (landed AmplitudeToMillibels 0x00C40F50). Only caller the
//   one-line libpk API wrapper at 0x00C35F60. Catalog gapscan name is a placeholder.
// Shape: the table pointer is re-read at every use (the entry stores may alias it); the match test breaks before the
//   silencing store; the 0xA02 test stores the masked word or 0x400, exactly as retail (`and eax,0A02h ; je ; or eax,400h ;
//   mov [ecx+14h],eax`).
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

void C2DAudioObject::SetSpeakerLevels(const SSpeakerLevelTable_00c445f0* levels)
{
    unsigned long pending = m_dirtyFlags & 0xa02;
    if (pending)
        m_dirtyFlags = pending | 0x400;
    for (long i = 0; i < levels->m_count; ++i) {
        long j = 0;
        for (; j < m_pLevels->m_count; ++j) {
            if (m_pLevels->m_pEntries[j].m_speaker == levels->m_pEntries[i].m_speaker)
                break;
            m_pLevels->m_pEntries[j].m_level = -10000;
        }
        long* level = &m_pLevels->m_pEntries[j].m_level;
        if (*level != levels->m_pEntries[i].m_level) {
            *level = levels->m_pEntries[i].m_level;
            m_dirtyFlags |= 0x400;
        }
    }
}