// CLibpkObject78::SetLong74(unsigned long value) -- descriptive: when the value differs from the stored one (+0x74), store it and raise the global change bit 0x1000000 (0x0143BC28)
// Leaf: ?SetLong74@CLibpkObject78@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40CF0, 25 bytes, thiscall `ret 4`.
// Identity: its only caller is the libpk `__stdcall` wrapper 0x00C33CA0 `(object, value)`; the class follows the landed
//   setters CLibpkObject78::SetFloat58..64 0x00C40BB0..0x00C40C40. Descriptive (libpk, no PDB/RTTI).
// Shape: `cmp [ecx+74h],eax ; je`, the store, then `or` on the global.
extern unsigned long g_flags_0143bc28;   // 0x0143BC28

class CLibpkObject78 {
public:
    unsigned char _pad00[0x68];
    unsigned long Long68;         // +0x68
    unsigned long Long6C;         // +0x6C
    float         Float70;        // +0x70
    unsigned long Long74;         // +0x74
    unsigned long DirtyFlags;     // +0x78
    void SetLong74(unsigned long value);
};

void CLibpkObject78::SetLong74(unsigned long value)
{
    if (Long74 != value) {
        Long74 = value;
        g_flags_0143bc28 |= 0x1000000;
    }
}