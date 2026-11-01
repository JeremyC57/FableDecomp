// CCombFilter::Blend(SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output, const float* input, unsigned long count, unsigned long blendPosition) -- one reverb comb block cross-faded between the current and target comb settings: the first history tap of the shared line is mixed into the output, the comb value (own ring read scaled by the feedback plus both history taps) is written back to the ring and mixed into the input channel at the comb gain, each sample weighting the current settings by t and the target by 1-t with t stepping down by 1/512 per sample; the write position advances and the target settings are copied over the current ones when the blend ends inside this block
// Grade: DIFFER(623v641) -- retail evaluates both history sums before the two ring products (`fadd st(2)` onto the deeper sum) and homes currentValue in the dead input argument slot (`fstp`, two `fld` reloads); ours keeps it on the x87 stack and computes each product right after its sum; register roles differ with it (retail this=edi, line data=ecx, current=ebx)
// Leaf: ?Blend@CCombFilter@@QAEXPAUSCombSettings@@PBU2@PAUCRingBuffer@@PAMPBMKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76120, 641 bytes, thiscall `ret 1Ch`, five frame dwords; libpk reverb (no PDB/RTTI).
// Identity (proven): (1) the sole caller is the landed CReverbProcessor::ProcessCombs 0x00C69FB0, which calls it on each 0x14-byte comb
//   (vtable 0x012B9588) as Blend(&settings->Combs[i], &target->Combs[i], &m_line, output, inputs[i], count, m_blendPosition) while a blend
//   is active on that comb mode, and CCombFilter::Process 0x00C75FF0 (landed) otherwise; (2) the body is the scalar form of Process with
//   every term cross-faded (history tap 0 -> output, ring*feedback + taps -> ring and input at the gain) and ends by copying the 0x14-byte
//   target settings over the current ones (`blendPosition <= count`), so the first parameter is non-const: the caller binding
//   `PBUSCombSettings@@0` is corrected to `PAUSCombSettings@@PBU2@` (NAME_FIXES.tsv). The catalog "SignalProcessing_InterpolateAndAccumulate"
//   is a descriptive stand-in. The blend step 1/512 is the per-TU literal 0x012A3AF4 (0.001953125f); 1.0f 0x0122DED8; 2^32 0x0122DCB4.
// Callees: none (everything inline).
struct CDspOps;
struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
    unsigned int Forward(unsigned int pos, unsigned int n) const { return pos + n < m_size ? pos + n : pos + n - m_size; }
};
struct SCombSettings {                     // 0x14 bytes
    unsigned int m_historyDelay[2];        // +0x00
    unsigned int m_delay;                  // +0x08
    float m_feedback;                      // +0x0C
    float m_gain;                          // +0x10
};
class CCombFilter : public CDspObject {    // vtable 0x012B9588, 0x14 bytes
public:
    CRingBuffer m_ring;                    // +0x04
    long        m_mode;                    // +0x10
    void Blend(SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output, const float* input,
               unsigned long count, unsigned long blendPosition);
};

void CCombFilter::Blend(SCombSettings* current, const SCombSettings* target, CRingBuffer* line, float* output,
                        const float* input, unsigned long count, unsigned long blendPosition)
{
    float* ring = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    const float* history = line->m_data;
    unsigned int readCurrent = m_ring.Back(pos, current->m_delay);
    unsigned int readTarget = m_ring.Back(pos, target->m_delay);
    unsigned int start = line->Back(line->m_pos, count);
    float t = (float)blendPosition * (1.0f / 512.0f);
    float w = 1.0f - t;
    unsigned int n = count;
    if (blendPosition < count)
        n = blendPosition;
    for (unsigned int i = 0; i < n; ++i) {
        float a = history[line->Back(start, current->m_historyDelay[0])];
        float b = history[line->Back(start, target->m_historyDelay[0])];
        *output++ += a * t + b * w;
        float currentValue = ring[readCurrent] * current->m_feedback + (a + history[line->Back(start, current->m_historyDelay[1])]);
        float targetValue = ring[readTarget] * target->m_feedback + (b + history[line->Back(start, target->m_historyDelay[1])]);
        ring[pos] = currentValue * t + targetValue * w;
        *(float*)input++ += currentValue * current->m_gain * t + targetValue * target->m_gain * w;
        readCurrent = m_ring.Forward(readCurrent, 1);
        readTarget = m_ring.Forward(readTarget, 1);
        pos = m_ring.Forward(pos, 1);
        start = line->Forward(start, 1);
        if (t > (1.0f / 512.0f)) {
            w += (1.0f / 512.0f);
            t -= (1.0f / 512.0f);
        }
    }
    m_ring.m_pos = pos;
    if (blendPosition <= count)
        *current = *target;
}
