// CAudioObject::SetMillibels(long millibels) -- descriptive: when the level (+0x1c) changes, store it, cache its amplitude (+0x18 = MillibelsToAmplitude) and raise dirty bit 2 in the flags word at +0x14
// Shape (retry1): parked as an "LTCG register contract" gap (retail keeps `this` in ecx across the cdecl MillibelsToAmplitude call).
//   With the callee's body (the landed MillibelsToAmplitude_00c42c90, 0x00C42C90) defined earlier in the same TU, VC7.1 /O2
//   knows it leaves ecx alone and keeps `this` there itself. The callee carries libpk's inline-asm 2^x block, so this file
//   holds that retail_asm body too (evidence line below, copied from the landed callee); SetMillibels itself is plain C++.
// Retail-asm: 0x00C42CC5..0x00C42CF4 `fld [esp+4] ; fmul [esp] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp] ; fsub [esp+8] ; f2xm1 ; fadd [esp+18h] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block (the landed retail_asm twin MillibelsToAmplitude_00c40ee0 0x00C40EE0, byte-identical); frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only; the prologue test, the exponent and the two constants are C++.
// Leaf: ?SetMillibels@CAudioObject@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42FB0, 36 bytes, thiscall `ret 4`.
// Identity: called from the libpk play wrapper at 0x00C35AC9; the converter 0x00C42C90 is the landed
//   MillibelsToAmplitude_00c42c90 (cdecl, float in st0). Class follows the landed CAudioObject::SetGain 0x00C42F50.
//   Descriptive (libpk, no PDB/RTTI).
// Retail keeps `this` in ecx across the cdecl call (`fstp [ecx+18h]` right after it); the same-TU callee body above reproduces it.
float __cdecl MillibelsToAmplitude_00c42c90(long millibels)
{
    if (millibels <= -10000)
        return 0.0f;
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float exponent = millibels * 0.0005f;
    float one = 1.0f;
    __asm {
        fld   log2of10
        fmul  exponent
        frndint
        fst   whole
        wait
        fld1
        fscale
        fxch  st(1)
        fstp  scale
        fld   log2of10
        fmul  exponent
        fsub  whole
        f2xm1
        fadd  one
        fmulp st(1), st
        fstp  result
        wait
    }
    return result;
}
class CAudioObject {
public:
    unsigned char _pad00[0x14];
    unsigned long DirtyFlags;      // +0x14
    float         Amplitude;       // +0x18
    long          Millibels;       // +0x1c
    void SetMillibels(long millibels);
};

void CAudioObject::SetMillibels(long millibels)
{
    if (Millibels != millibels) {
        Millibels = millibels;
        Amplitude = MillibelsToAmplitude_00c42c90(millibels);
        DirtyFlags |= 2;
    }
}