// CLibpkObject78::SetFloat58(float value) -- descriptive: libpk audio object float setter: when the value differs from the stored one, store it and raise dirty bit(s) 0x8000 in the flags word at +0x78
// Leaf: ?SetFloat58@CLibpkObject78@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40BB0, 33 bytes, thiscall, `ret 4`.
// Identity: DESCRIPTIVE -- the 0x00C3xxxx-0x00C4xxxx libpk audio library has no PDB names and no RTTI; the class is named
//   after its dirty-flags word and the method after the field it sets. Its only caller is a libpk `__stdcall` wrapper `(object, value)` that calls it on the object.
// Label: the manifest's "sub_00c40bb0" is a gap-scan / propagated name.
// Shape: `if (field != value)` (field loaded second-from-top: fld value; fld field; fucompp), the float stored as a dword
//   copy, then one `or` of the dirty mask.
class CLibpkObject78 {
public:
    unsigned char _pad00[0x40];
    float         Float40;        // +0x40
    unsigned char _pad44[0x4];
    float         Float48;        // +0x48
    float         Float4C;        // +0x4C
    float         Float50;        // +0x50
    float         Float54;        // +0x54
    float         Float58;        // +0x58
    float         Float5C;        // +0x5C
    float         Float60;        // +0x60
    float         Float64;        // +0x64
    unsigned char _pad68[0x10];
    unsigned long DirtyFlags;      // +0x78
    void SetFloat58(float value);
};
void CLibpkObject78::SetFloat58(float value)
{
    if (Float58 != value) {
        Float58 = value;
        DirtyFlags |= 0x8000;
    }
}