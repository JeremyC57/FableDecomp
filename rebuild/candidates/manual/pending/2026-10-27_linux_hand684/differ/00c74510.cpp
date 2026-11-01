// CDspGain::ApplyRamps(const float* feedback, const float* feedforward, float* state, const float* source, float* block, bool hasInput, unsigned int count) -- run one channel of the gain's smoothing filter over a block: a 1-offset IIR (state 1.0 = rest) with feedback a1/a2 and feedforward b0/b1/b2; first order (a2 == b2 == 0): pure gain when a1 == b1 == 0 (copy or ScaleCopy, state = last output), else the inline one-pole loop with or without input; second order: the ops table's biquad when there is input, else the inline two-pole decay loop
// Grade: DIFFER(828v828) -- every block byte-equal except the two-pole decay loop (0x00C747B5..0x00C74848): retail keeps y0 (the state[0] value) in a frame slot (`mov eax,[edx] ; mov [esp+30h],eax`, `fstp [esp+30h]` in place, `mov ecx,[esp+30h] ; mov [edx],ecx` out) and carries the old y0 on the x87 stack into the y1 slot at the loop top (`fstp [esp+2Ch]`, exit `fstp [edx+4]`); VC7.1 here enregisters the state[0]-derived variable and keeps y1 in memory in every spelling tried (28 spellings: rotation orders, yOld temporaries, float[2] / struct states, function-scope states, inline reference helper, embedded assignment, hoisted constants); the mirrored roles also cost the fourth frame slot (0xC v 0x10)
// Leaf: ?ApplyRamps@CDspGain@@QAEXPBM0PAM01_NI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74510, 828 bytes, thiscall `ret 1Ch` that never reads `this` (seven stack
//   arguments), 0x10-byte frame whose four locals and two dead argument homes hold the coefficient copies.
// Identity (descriptive: the libpk reverb module has no PDB or RTTI): the callee of the landed CDspGain::Apply 0x00C74CE0
//   and CDspGain::Crossfade 0x00C74D30 (params+4 / params+0x10 / the gain's ramp state, source, block, mode, count) and
//   of CDspGain::Reset 0x00C744E0's family; same ops table CDspObject::s_pOps 0x0143CCDC. The catalog label
//   "Audio_IIRFilterProcess" (forge-autoname2) describes the body; the reloc binding ?ApplyRamps@CDspGain@@QAEXPBM0PAM01HI@Z
//   is the landed callers' spelling with `int mode`: the sixth argument is read as a byte (`mov al,[esp+30h] ; test al,al`
//   at 0x00C74573 and 0x00C74784), i.e. a bool -- see NAME_FIXES.
// Callees: CDspOps slot 3 ScaleCopy (+0xC, 0x00C745FA) and slot 8 (+0x20, 0x00C747AA: the biquad with input,
//   (block, source, feedback, feedforward, state, count)), memcpy as `rep movsd` (block != source, count dwords).
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Shape: every coefficient is copied to a float local up front (a1, a2 from the feedback set, b0, b1, b2 from the
//   feedforward set; feedback[0] is never read); the zero tests are `fucompp ; test ah,44h ; jp`; the one-pole loops keep
//   the running state on the x87 stack and store it back once (`fstp [ebx]`), the input loop unrolled by four by VC7.1; the
//   two-pole decay loop keeps y0 on the stack and y1 in a frame slot. The filter is 1-offset: y' = a1*y + x + (1 - a1),
//   out = b0*y' + b1*y - (b0 + b1), so a rest state of 1.0 (Reset) passes b0*x.
struct CDspOps {                           // x87 vtable 0x012BF02C (the landed CDspGain::Apply / Crossfade model)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    virtual void Biquad(float* dst, const float* src, const float* feedback, const float* feedforward, float* state,
                        unsigned int count);                                                        // slot 8 (+0x20)
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

struct SRampState {                        // one settings' per-channel ramp state (two floats)
    float m_values[2];
};

extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

struct CDspGain : CDspObject {             // vtable 0x012B9578
    SRampState m_old;                      // +0x04
    SRampState m_new;                      // +0x0C
    void ApplyRamps(const float* feedback, const float* feedforward, float* state, const float* source, float* block,
        bool hasInput, unsigned int count);
};

void CDspGain::ApplyRamps(const float* feedback, const float* feedforward, float* state, const float* source, float* block,
    bool hasInput, unsigned int count)
{
    float a1 = feedback[1];
    float a2 = feedback[2];
    float b0 = feedforward[0];
    float b1 = feedforward[1];
    float b2 = feedforward[2];
    if (a2 == 0.0f && b2 == 0.0f) {
        float c = 1.0f - a1;
        float d = -(b1 + b0);
        if (hasInput) {
            if (a1 == 0.0f && b1 == 0.0f) {
                if (b0 == 1.0f) {
                    if (block != source)
                        memcpy(block, source, count * sizeof(float));
                } else {
                    s_pOps->ScaleCopy(block, source, b0, count);
                }
                state[0] = block[count - 1];
            } else {
                float y = state[0];
                for (unsigned int i = 0; i < count; i++) {
                    float t = b1 * y;
                    y = y * a1 + *source++ + c;
                    *block++ = b0 * y + t + d;
                }
                state[0] = y;
            }
        } else {
            float y = state[0];
            for (unsigned int i = 0; i < count; i++) {
                float t = b1 * y;
                y = y * a1 + c;
                block[i] = b0 * y + t + d;
            }
            state[0] = y;
        }
    } else if (hasInput) {
        s_pOps->Biquad(block, source, feedback, feedforward, state, count);
    } else {
        float y0 = state[0];
        float c = 1.0f - (a2 + a1);
        float d = -(b2 + b1 + b0);
        float y1 = state[1];
        for (unsigned int i = 0; i < count; i++) {
            float y = y1 * a2 + y0 * a1 + c;
            *block++ = y1 * b2 + y0 * b1 + y * b0 + d;
            y1 = y0;
            y0 = y;
        }
        state[1] = y1;
        state[0] = y0;
    }
}
