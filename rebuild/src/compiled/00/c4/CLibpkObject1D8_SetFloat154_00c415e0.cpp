// CLibpkObject1D8::SetFloat154(float value) -- descriptive: libpk audio object float setter: when the value differs from the stored one (+0x154), store it and raise dirty bit 0x800000 in the flags word at +0x1D8
// Leaf: ?SetFloat154@CLibpkObject1D8@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C415E0, 42 bytes, thiscall `ret 4`.
// Identity: called from the libpk `__stdcall` wrapper at 0x00C34259 (the accepts-the-object API shape of the landed
//   0x00C33F20 / 0x00C34050); the class follows the landed setters CLibpkObject1D8::SetFloat17C..198 0x00C41440..0x00C41570
//   (flags word +0x1D8). Descriptive (libpk, no PDB/RTTI).
// Shape: `if (field != value)` (fld value; fld field; fucompp), the float stored as a dword copy, then `or` of the mask.
class CLibpkObject1D8 {
public:
    unsigned char _pad00[0x154];
    float         Float154;       // +0x154
    unsigned char _pad158[0x1D8 - 0x158];
    unsigned long DirtyFlags;      // +0x1D8
    void SetFloat154(float value);
};

void CLibpkObject1D8::SetFloat154(float value)
{
    if (Float154 != value) {
        Float154 = value;
        DirtyFlags |= 0x800000;
    }
}