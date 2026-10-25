// CLimiter::UpdateEnvelope() -- follow the magnitude of the sample 44 entries back in the delay line: hold the largest magnitude for up to 44 samples, then move the gain envelope towards it (fast attack plus a 100.0 offset) or let it decay (x 0.9999476 per sample), leaving the release state once the envelope is back at full scale
// Leaf: ?UpdateEnvelope@CLimiter@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C592F0, length 208 bytes; `ret`, returns nothing.
// Callers: the limiter's process loop 0x00C593C0 (attack state 1 and release state 2). Constants: 2^32 (0x0122DCB4,
// the unsigned-to-float fix-up), 0.0586f (0x012B8F6C), 100.0f (0x01230A08), 0.9999476f (0x012B8F68), 32767.0f
// (0x0129C820).
// Label: the manifest's "FollowCamera_UpdateDamping" is propagated; the class is the limiter of 0x00C59570.
// Shape: the magnitude of a negative 16-bit-range sample is `-1 - s` (`or edx,-1 ; sub edx,eax`), converted as an
// UNSIGNED long (`fild` + 2^32 fix-up); `level > m_peak || m_holdCount == 44` keeps the hold increment on the
// fall-through with `fstp st(0)`; `m_peak >= m_envelope` gives `test ah,1 ; jne`, `m_envelope <= 32767.0f` gives
// `test ah,41h ; jp`, and `m_envelope *= k` re-uses the stored value (`fst`) for the compare.

// The GameCODA output limiter (vtable 0x012B8F64, no RTTI; 0x17C bytes; six of them at +0xC34 of the output device
// built by 0x00C3EFA0 through `__ehvec_ctor` with this constructor and the destructor 0x00C592C0). Its process loop
// 0x00C593C0 writes each 32-bit mixed sample into an 88-entry delay line, reads the sample 44 entries back, and runs a
// three-state machine (0 pass-through, 1 attack, 2 release) that scales the delayed sample by 32767 / envelope.
class CLimiter {
public:
    virtual ~CLimiter();                // 0x00C592C0 (body) / 0x00C595A0 (scalar deleting)
    long          m_delay[88];          // +0x004 delay line of mixed samples
    unsigned long m_writeIndex;         // +0x164
    float         m_peak;               // +0x168 held peak magnitude of the delayed samples
    float         m_envelope;           // +0x16c gain envelope (attack towards the peak, slow release)
    long          m_holdCount;          // +0x170 samples the peak has been held (re-armed at 44)
    long          m_state;              // +0x174 0 pass-through, 1 attack, 2 release
    long          m_releaseCount;       // +0x178
    CLimiter();
    void UpdateEnvelope();
};

void CLimiter::UpdateEnvelope()
{
    long idx = m_writeIndex - 44;
    if (idx < 0)
        idx += 88;
    long s = m_delay[idx];
    unsigned long mag = s < 0 ? -1 - s : s;
    float level = (float)mag;
    if (level > m_peak || m_holdCount == 44) {
        m_peak = level;
        m_holdCount = 0;
    } else
        ++m_holdCount;
    if (m_peak >= m_envelope)
        m_envelope = (m_peak - m_envelope) * 0.0586f + m_envelope + 100.0f;
    else {
        m_envelope *= 0.99994761f;
        if (m_envelope <= 32767.0f && m_state == 2)
            m_state = 0;
    }
}