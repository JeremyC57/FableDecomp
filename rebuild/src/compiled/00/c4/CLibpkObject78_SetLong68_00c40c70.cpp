// CLibpkObject78::SetLong68(unsigned long value) -- descriptive: when the value differs from the stored one (+0x68), store it, raise dirty bit 0x80000 in the flags word at +0x78 and the global change bit 0x1000000 (0x0143BC28)
// Leaf: ?SetLong68@CLibpkObject78@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40C70, 37 bytes, thiscall `ret 4`.
// Identity: its only caller is the libpk `__stdcall` wrapper 0x00C33C40 `(object, value)`, which first clamps the value to at
//   least the word at +0x6c; the class follows the landed setters CLibpkObject78::SetFloat58..64 0x00C40BB0..0x00C40C40
//   (flags word +0x78). Descriptive (libpk, no PDB/RTTI).
// Shape: `cmp [ecx+68h],eax ; je`, then the flag word read-modify-write interleaved with the value store, then `or` on the global.
extern unsigned long g_flags_0143bc28;   // 0x0143BC28

class CLibpkObject78 {
public:
    unsigned char _pad00[0x68];
    unsigned long Long68;         // +0x68
    unsigned long Long6C;         // +0x6C
    float         Float70;        // +0x70
    unsigned long Long74;         // +0x74
    unsigned long DirtyFlags;     // +0x78
    void SetLong68(unsigned long value);
};

void CLibpkObject78::SetLong68(unsigned long value)
{
    if (Long68 != value) {
        Long68 = value;
        DirtyFlags |= 0x80000;
        g_flags_0143bc28 |= 0x1000000;
    }
}