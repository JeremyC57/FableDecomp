// CLibpkObject78::SetFloat70(float value) -- descriptive: when the value differs from the stored one (+0x70), store it and raise the global change bit 0x1000000 (0x0143BC28)
// Leaf: ?SetFloat70@CLibpkObject78@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40CC0, 36 bytes, thiscall `ret 4`.
// Identity: its only caller is the libpk `__stdcall` wrapper 0x00C33C80 `(object, value)`; the class follows the landed
//   setters CLibpkObject78::SetFloat58..64 0x00C40BB0..0x00C40C40. Descriptive (libpk, no PDB/RTTI).
// Shape: `if (field != value)` (fld value; fld field; fucompp), the float stored as a dword copy, then `or` on the global.
extern unsigned long g_flags_0143bc28;   // 0x0143BC28

class CLibpkObject78 {
public:
    unsigned char _pad00[0x68];
    unsigned long Long68;         // +0x68
    unsigned long Long6C;         // +0x6C
    float         Float70;        // +0x70
    unsigned long Long74;         // +0x74
    unsigned long DirtyFlags;     // +0x78
    void SetFloat70(float value);
};

void CLibpkObject78::SetFloat70(float value)
{
    if (Float70 != value) {
        Float70 = value;
        g_flags_0143bc28 |= 0x1000000;
    }
}