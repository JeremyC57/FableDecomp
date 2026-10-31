// CEffectFilter::SetLowPass(long cutoff, float reference, unsigned long sampleRate) -- libpk mixer DSP: set the six filter coefficients (+4..+0x18) for a second-order Butterworth low-pass: a zero cutoff gives the pass-through set {1,0,0, 1,0,0}; otherwise the corner reference * 2^((cutoff+300)/1200) (pow(10, (cutoff+300) * log10(2)/1200)), clamped to 0.495 * the sample rate, gives k = tan(pi f / rate) and the bilinear terms {1, 2a, -(a*a + x), y, 2y, y}
// Leaf: ?SetLowPass@CEffectFilter@@QAEXJMK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74AB0, 256 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Callers: CEffectPart260::Configure 0x00C783F0 (on the filter at voice +0x00, landed binding ?SetShelf@CEffectFilter@@QAEXMMK@Z)
//   and the slot thunk 0x00C777C7. Callee: _CIpow 0x00BFEB40 (pow(10.0, ...)); tan is the inline x87 intrinsic (fptan).
// Identity (descriptive; the libpk DSP module has no PDB/RTTI): class CEffectFilter as the landed sibling
//   CEffectFilter::SetHighPass 0x00C74BB0 (the next routine, same six-float block, same pass-through set, same 0.495 clamp and
//   unsigned sample-rate fix-up) and the caller CEffectPart260::Configure name it. The first argument is a long (`add ecx,12Ch ;
//   fild`), so the caller's QAEXMMK spelling is wrong; the coefficients are a low-pass, not a shelf: the numerator is symmetric
//   {y, 2y, y} and the poles are the Butterworth bilinear pair (2(1-kk)/d, -(1+k^4)/d^2 with d = 1 + sqrt(2) k + kk)
//   (NAME_FIXES.tsv). The catalog's "CAudioChannel_ComputeShelveFilterCoefficients" is propagated.
//   Constants: 10.0 (double) 0x0129A7B0, log10(2)/1200 0x012BF068, 0.495 0x012BF064, pi 0x0128F604, sqrt(2) 0x012BF060,
//   1.0 0x0122DED8, 0.25 0x0123078C, 2^32 0x0122DCB4.
// Shape: the sibling's lesson: `frequency = pow(...); frequency *= reference;` as two statements schedules the sample rate's
//   `mov eax,[esp+10h] ; test eax,eax` before `fmul [reference]` (the one-expression product, the parked form, put the fmul
//   first); the pi product in a named local keeps `fmul pi ; fdiv rate`; m_a0 = 1 is stored before the tan.
#include <math.h>
#pragma intrinsic(pow, tan)
struct CEffectFilter {
public:
    float Gain;                                 // +0x00
    float m_a0;                                 // +0x04
    float m_a1;                                 // +0x08
    float m_a2;                                 // +0x0c
    float m_b0;                                 // +0x10
    float m_b1;                                 // +0x14
    float m_b2;                                 // +0x18
    void SetLowPass(long cutoff, float reference, unsigned long sampleRate);
};

void CEffectFilter::SetLowPass(long cutoff, float reference, unsigned long sampleRate)
{
    if (cutoff == 0) {
        m_a0 = 1.0f;
        m_a1 = 0.0f;
        m_a2 = 0.0f;
        m_b0 = 1.0f;
        m_b1 = 0.0f;
        m_b2 = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (double)((cutoff + 300) * 0.00025085834f));
    frequency *= reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float angle = frequency * 3.14159265f;
    m_a0 = 1.0f;
    float k = (float)tan(angle / rate);
    float kk = k * k;
    float d = k * 1.41421356f + kk + 1.0f;
    float a = (1.0f - kk) / d;
    float x = (kk + kk) / (d * d);
    float y = ((1.0f - a) * (1.0f - a) + x) * 0.25f;
    m_a1 = a + a;
    m_a2 = -(a * a + x);
    m_b0 = y;
    m_b1 = y + y;
    m_b2 = y;
}