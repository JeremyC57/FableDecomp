// C2DAudioObject::InitSpeakerMaps() -- set up the 2D audio object's per-speaker tables: with requested cap 0x40000 the gain table (+0x118, current pointer +0x108) gets one {speaker bit, 1.0} entry per output speaker, with cap 0x20000 the level table (+0x120, pointer +0x10C) one {speaker bit, 0.0} entry; the speaker bits are the set bits of the global speaker mask in order; always true
// Leaf: ?InitSpeakerMaps@C2DAudioObject@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C448D0, 198 bytes (+ int3 padding), __fastcall `ret`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller the 2D object's Open 0x00C449C0 (its tail
//   `return`). The manifest's "CCreatureAbility_InitializeFlagScales" is propagated.
// Layout: globals 0x0139CA04 (output speaker count) and 0x0139CA08 (speaker bit mask); the entry arrays are direct
//   members at +0x128 / +0x168 (eight 8-byte entries each), the table headers' first word is the count.
// Shape: the two loops are written out (an inline helper taking the entry array and level compiles smaller, 104
//   bytes); both globals are re-read every pass (the entry stores may alias them) and the header count store after
//   each loop writes the last-read count through the header's register.
extern long g_speakerCount;                 // 0x0139CA04
extern unsigned long g_speakerMask;         // 0x0139CA08

struct SSpeakerLevel {
    unsigned long m_speaker;                // +0x00 speaker bit
    float         m_level;                  // +0x04
};

struct SSpeakerTable {
    long          m_count;                  // +0x00
    unsigned long m_reserved;               // +0x04
};

class C2DAudioObject {
public:
    void*          m_vptr;                  // +0x00 (vtable 0x012B7B88)
    unsigned long  m_node[2];               // +0x04
    unsigned long  m_caps;                  // +0x0c requested capability bits
    unsigned char  m_pad10[0x108 - 0x10];
    SSpeakerTable* m_pGains;                // +0x108
    SSpeakerTable* m_pLevels;               // +0x10c
    unsigned long  m_unknown110[2];         // +0x110
    SSpeakerTable  m_gains;                 // +0x118
    SSpeakerTable  m_levels;                // +0x120
    SSpeakerLevel  m_gainEntries[8];        // +0x128
    SSpeakerLevel  m_levelEntries[8];       // +0x168
    bool InitSpeakerMaps();
};

bool C2DAudioObject::InitSpeakerMaps()
{
    if (m_caps & 0x40000) {
        m_pGains = &m_gains;
        unsigned long bit = 1;
        for (long i = 0; i < g_speakerCount; ++i) {
            while (!(bit & g_speakerMask))
                bit <<= 1;
            m_gainEntries[i].m_speaker = bit;
            m_gainEntries[i].m_level = 1.0f;
            bit <<= 1;
        }
        m_gains.m_count = g_speakerCount;
    }
    if (m_caps & 0x20000) {
        m_pLevels = &m_levels;
        unsigned long bit = 1;
        for (long i = 0; i < g_speakerCount; ++i) {
            while (!(bit & g_speakerMask))
                bit <<= 1;
            m_levelEntries[i].m_speaker = bit;
            m_levelEntries[i].m_level = 0.0f;
            bit <<= 1;
        }
        m_levels.m_count = g_speakerCount;
    }
    return true;
}