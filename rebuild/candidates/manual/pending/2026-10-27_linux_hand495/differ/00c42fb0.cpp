// CAudioObject::SetMillibels(long millibels) -- descriptive: when the level (+0x1c) changes, store it, cache its amplitude (+0x18 = MillibelsToAmplitude) and raise dirty bit 2 in the flags word at +0x14
// Grade: DIFFER(40v36) -- retail keeps `this` in ecx across the cdecl MillibelsToAmplitude call (`push eax ; mov [ecx+1Ch],eax ; call ; fstp [ecx+18h]`, no esi frame): an LTCG register contract VC7.1 does not emit without whole-program optimisation; ours saves this in esi
// Leaf: ?SetMillibels@CAudioObject@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42FB0, 36 bytes, thiscall `ret 4`.
// Identity: called from the libpk play wrapper at 0x00C35AC9; the converter 0x00C42C90 is the landed
//   MillibelsToAmplitude_00c42c90 (cdecl, float in st0). Class follows the landed CAudioObject::SetGain 0x00C42F50.
//   Descriptive (libpk, no PDB/RTTI).
// Shape: retail keeps `this` in ecx across the cdecl call (`fstp [ecx+18h]` right after it): an LTCG register contract.
float __cdecl MillibelsToAmplitude_00c42c90(long millibels);   // 0x00C42C90

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
