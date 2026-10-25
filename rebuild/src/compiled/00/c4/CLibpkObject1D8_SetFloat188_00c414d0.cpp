// CLibpkObject1D8::SetFloat188(float value) -- descriptive: libpk audio object float setter: when the value differs from the stored one, store it and raise dirty bit(s) 0x40000 in the flags word at +0x1D8
// Leaf: ?SetFloat188@CLibpkObject1D8@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C414D0, 42 bytes, thiscall, `ret 4`.
// Identity: DESCRIPTIVE -- the 0x00C3xxxx-0x00C4xxxx libpk audio library has no PDB names and no RTTI; the class is named
//   after its dirty-flags word and the method after the field it sets. Its callers are libpk `__stdcall` wrappers that ask the object (virtual slot 1) whether it accepts and call the setter on `accepts ? object : 0` (the landed 0x00C34050 / 0x00C33F20 shape; the same flags word +0x1D8 as the landed setter 0x00C417A0).
// Label: the manifest's "sub_00c414d0" is a gap-scan / propagated name.
// Shape: `if (field != value)` (field loaded second-from-top: fld value; fld field; fucompp), the float stored as a dword
//   copy, then one `or` of the dirty mask.
class CLibpkObject1D8 {
public:
    unsigned char _pad00[0x17C];
    float         Float17C;       // +0x17C
    float         Float180;       // +0x180
    float         Float184;       // +0x184
    float         Float188;       // +0x188
    float         Float18C;       // +0x18C
    unsigned char _pad190[0x8];
    float         Float198;       // +0x198
    unsigned char _pad19C[0x3C];
    unsigned long DirtyFlags;      // +0x1D8
    void SetFloat188(float value);
};
void CLibpkObject1D8::SetFloat188(float value)
{
    if (Float188 != value) {
        Float188 = value;
        DirtyFlags |= 0x40000;
    }
}