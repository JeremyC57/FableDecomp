// CDspGain::Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode, unsigned int count, unsigned int remaining) -- cross-fade a gain change over 512 samples: run the new settings' ramps into an aligned scratch block and the old settings' ramps into the output block, fade the old master out and the new master in (each by master/512 per sample from the remaining fraction), mix the scratch block in, and adopt the new settings once the fade ends in this block
// Leaf: ?Crossfade@CDspGain@@QAEXPAUSGainParams@@PBU2@PBMPAMHII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74D30, 297 bytes, thiscall `ret 1Ch`, `and esp,-16` frame (the 128-float scratch
//   block is 16-byte aligned for the SIMD ops table).
// Identity (descriptive: the libpk reverb module has no PDB or RTTI): the CDspGain of the landed Apply 0x00C74CE0 (same
//   ApplyRamps 0x00C74510 call shape: params+4 / params+0x10 / the gain's ramp state, source, block, mode, count; the
//   shared ops table CDspObject::s_pOps 0x0143CCDC) with its second ramp state +0xC (Reset 0x00C744E0 sets all four floats
//   to 1.0f). Called from the reverb gains next to Apply (0x00C69DB5.., 0x00C75915, the diffuser stages 0x00C89638..).
//   The manifest label "Animation_InterpolateHelper" is a guess.
// Callees: CDspGain::ApplyRamps 0x00C74510 (twice), CDspOps slot 2 ScaleRamp (+8, twice) and slot 5 Add (+0x14).
// Constants: 4294967296.0f 0x0122DCB4 (the unsigned-to-float fix), 1/512 0x012A3AF4, -1/512 0x012BF070, 1.0f 0x0122DED8.
// Shape: `remaining` is unsigned (fild + the 2^32 fix, `jb`); the fraction is kept in a frame local across the calls; each
//   ScaleRamp call evaluates its step before its gain (right-to-left arguments); the ramp-state addresses computed for the
//   ApplyRamps calls are reused for the final two-float copy, which stores the first float before loading the second:
//   only a user element-wise operator= on a two-float SRampState gives both (a float m_gain[4] model recomputes the
//   addresses; a plain struct copy loads both floats first). The landed Apply 0x00C74CE0 models the same object as
//   float m_gain[4] (header debt). Grades: 4.
struct CDspOps {                           // x87 vtable 0x012BF02C (the landed CDspGain::Apply model)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

struct SGainParams {                       // 0x1C bytes
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct SRampState {                       // one settings' per-channel ramp state (two floats)
    float m_values[2];
    SRampState& operator=(const SRampState& other)   // element-wise (retail stores the first before loading the second)
    {
        m_values[0] = other.m_values[0];
        m_values[1] = other.m_values[1];
        return *this;
    }
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    SRampState m_old;                      // +0x04 ramp state of the current settings (Apply's)
    SRampState m_new;                      // +0x0C ramp state of the settings faded in
    void Reset();                          // 0x00C744E0
    void ApplyRamps(const float* current, const float* target, float* state, const float* source, float* block,
        int mode, unsigned int count);     // 0x00C74510
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
        unsigned int count, unsigned int remaining);
};

void CDspGain::Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
    unsigned int count, unsigned int remaining)
{
    __declspec(align(16)) float scratch[128];
    if (remaining == 512) {
        m_new.m_values[0] = 1.0f;
        m_new.m_values[1] = 1.0f;
    }
    ApplyRamps(newParams->m_current, newParams->m_target, m_new.m_values, source, scratch, mode, count);
    ApplyRamps(oldParams->m_current, oldParams->m_target, m_old.m_values, source, block, mode, count);
    float fraction = (float)remaining * 0.001953125f;
    s_pOps->ScaleRamp(block, fraction * oldParams->m_master, oldParams->m_master * -0.001953125f, count);
    s_pOps->ScaleRamp(scratch, (1.0f - fraction) * newParams->m_master, newParams->m_master * 0.001953125f, count);
    s_pOps->Add(block, scratch, count);
    if (count >= remaining) {
        *oldParams = *newParams;
        m_old = m_new;
    }
}