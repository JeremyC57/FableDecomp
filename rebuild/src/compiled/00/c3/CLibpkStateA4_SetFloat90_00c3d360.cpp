// CLibpkStateA4::SetFloat90(float value) -- descriptive: libpk audio object float setter: when the value differs from the stored one, store it and raise dirty bit(s) 0x810000 in the flags word at +0xA4
// Leaf: ?SetFloat90@CLibpkStateA4@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D360, 42 bytes, thiscall, `ret 4`.
// Identity: DESCRIPTIVE -- the 0x00C3xxxx-0x00C4xxxx libpk audio library has no PDB names and no RTTI; the class is named
//   after its dirty-flags word and the method after the field it sets. Its only callers are libpk C-API thunks that load the global instance (`mov ecx,0143BB58h ; jmp`).
// Label: the manifest's "sub_00c3d360" is a gap-scan / propagated name.
// Shape: `if (field != value)` (field loaded second-from-top: fld value; fld field; fucompp), the float stored as a dword
//   copy, then one `or` of the dirty mask.
class CLibpkStateA4 {
public:
    unsigned char _pad00[0x90];
    float         Float90;        // +0x90
    unsigned char _pad94[0x8];
    float         Float9C;        // +0x9C
    float         FloatA0;        // +0xA0
    unsigned long DirtyFlags;      // +0xA4
    void SetFloat90(float value);
};
void CLibpkStateA4::SetFloat90(float value)
{
    if (Float90 != value) {
        Float90 = value;
        DirtyFlags |= 0x810000;
    }
}