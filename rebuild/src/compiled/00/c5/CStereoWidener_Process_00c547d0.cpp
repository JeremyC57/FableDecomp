// CStereoWidener::Process(long* left, long* right) -- widen a stereo pair of 128-sample mix accumulators: each channel gets a low-passed, inverted copy of the other channel's output from ten samples earlier (one-pole filter y += (-335 y - 258 x) / 4096 over a ten-sample history per channel)
// Leaf: ?Process@CStereoWidener@@QAEXPAJ0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C547D0, 680 bytes, thiscall `ret 8` (the 128-sample loop unrolled by four by VC7.1:
//   32 trips, the trip count homed in the dead `left` slot).
// Identity (descriptive; libpk software mixer, no PDB / RTTI): the 0x8C-byte filter objects the output device keeps at +0x5C
//   (front pair) and +0xE8 (rear pair), cleared and run by the landed 0x00C3E800 on the mixer's channel buffers (which binds the
//   address as ?Process@CLevelMeter_00c3e800@@QAEXJJ@Z: the arguments are the accumulators, and the body writes them, so it is a
//   filter, not a meter: NAME_FIXES). The catalog's "Audio_MixStereoSamples" is a propagated description.
// Layout (retail code): two 16-word histories (+0x00 left output, +0x40 right output; ten words used), the two filter states
//   +0x80 / +0x84, the history index +0x88 (wraps at 10).
// Shape: every member is re-read after each store through left / right (they may alias the object).
class CStereoWidener {                          // 0x8C bytes
public:
    long          m_history[2][16];             // +0x00 left, +0x40 right (ten samples used)
    long          m_filter[2];                  // +0x80 / +0x84
    unsigned long m_index;                      // +0x88
    void Process(long* left, long* right);
};

void CStereoWidener::Process(long* left, long* right)
{
    for (int i = 0; i < 128; ++i) {
        m_filter[0] += (m_filter[0] * -335 - m_history[0][m_index] * 258) >> 12;
        m_filter[1] += (m_filter[1] * -335 - m_history[1][m_index] * 258) >> 12;
        left[i] += m_filter[1];
        m_history[0][m_index] = left[i];
        right[i] += m_filter[0];
        m_history[1][m_index] = right[i];
        if (++m_index == 10)
            m_index = 0;
    }
}