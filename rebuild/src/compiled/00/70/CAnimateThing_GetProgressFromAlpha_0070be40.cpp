// CAnimateThing::GetProgressFromAlpha(float alpha, float totalAlpha) const -- the fraction alpha / totalAlpha while the total lies in [epsilon, 1 + epsilon], else 1
// Leaf: GetProgressFromAlpha
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BE40, length 58 bytes (+ int3 padding). __fastcall (`this` unused), `ret 8`,
//         result on the FPU stack. Ego_r.pdb: `?GetProgressFromAlpha@CAnimateThing@@ABEMMM@Z` (private const).
// Callees: none. Globals: the 1e-4f epsilon @ 0x0129BA3C (a non-const global: `epsilon + 1.0f` is computed at run time, the
//          landed sources name it DELTA_NOTIONAL_ZERO), 1.0f @ 0x0122DED8.
// Identity: the manifest label "SafeDivideFloat" is descriptive. Single caller CAnimateThing::CalcInterpolatedMeshAnim
//   (0x0070D940, call site 0x0070DB07, `this` passed in ecx and never read), the Ego_r neighbour of GetProgressFromAlpha
//   (CalcMeshAnims / GetProgressFromAlpha / CalcInterpolatedMeshAnim, 0x00575170..0x00576AD0).
// Shape: both range tests are `>=` compares (`test ah,1 ; jne` to the shared `return 1.0f` tail), the upper bound written
//   `epsilon + 1.0f >= totalAlpha` (the epsilon loaded first, then `fadd 1.0`, then `fcomp` with the argument).
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)

class CAnimateThing {
public:
    float GetProgressFromAlpha(float alpha, float totalAlpha) const;
};

float CAnimateThing::GetProgressFromAlpha(float alpha, float totalAlpha) const
{
    if (totalAlpha >= DELTA_NOTIONAL_ZERO && DELTA_NOTIONAL_ZERO + 1.0f >= totalAlpha)
        return alpha / totalAlpha;
    return 1.0f;
}