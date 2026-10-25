// SPKVoiceSettings::SPKVoiceSettings() -- libpk (audio middleware) settings record constructor: zero the id, construct the embedded two-word reference, then enable flag 1, two zero words and a unit (1.0f) gain
// Leaf: ??0SPKVoiceSettings@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D1A0, 49 bytes (+ int3 padding), thiscall, plain `ret` (returns `this`).
// Identity: libpk (0x00C08AE0..0x00C40000: no PDB names, no RTTI), so the class and member names are descriptive. Callers:
//   0x00C161D6 (a stack local under an EH state, its +0x0 float filled right after) and 0x00C2D36F (a member at +0x320 of a
//   larger libpk object). The catalog label `pair<unsigned_long,C3DMeshVolume>` is propagated and wrong.
// Callees: the embedded reference's constructor @ 0x00C167B0 (zeroes two words: an out-of-line LTCG body; catalogued as the
//   propagated `CBaseIntelligentPointer::CBaseIntelligentPointer`).
// Layout: +0x00 id (0), +0x04 the 8-byte reference, +0x0c enabled (1), +0x10 / +0x14 zero, +0x18 gain 1.0f.
// Shape: member-init list in declaration order; the id store is scheduled between the `lea ecx` and the call.
struct SPKReference {                               // 8 bytes; constructor out of line
    void* m_pObject;                                // +0x0
    void* m_pOwner;                                 // +0x4
    SPKReference();                                 // 0x00C167B0
};

struct SPKVoiceSettings {
    unsigned long m_id;                             // +0x00
    SPKReference  m_reference;                      // +0x04
    long          m_enabled;                        // +0x0c
    long          m_priority;                       // +0x10
    long          m_delay;                          // +0x14
    float         m_gain;                           // +0x18
    SPKVoiceSettings();
};

SPKVoiceSettings::SPKVoiceSettings() : m_id(0), m_enabled(1), m_priority(0), m_delay(0), m_gain(1.0f)
{
}