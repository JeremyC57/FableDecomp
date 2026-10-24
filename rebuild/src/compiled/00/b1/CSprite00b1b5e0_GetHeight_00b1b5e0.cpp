// CSprite_00b1b5e0::GetHeight() const -- the float height at +0x48 rounded to a long with the FPU's current rounding mode
// Leaf: GetHeight
// Retail-asm: 0x00B1B5F1 `fld dword ptr [esp+8] ; mov edx,[esp+4] ; fistp dword ptr [edx]` -- the engine's inline-assembly float-to-long helper taking the value and the result address by value (both homed in the 12-byte frame); VC7.1 C++ `(long)x` tail-jumps __ftol2 and /QIfist emits `fistp qword` without the pointer indirection.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B5E0, 34 bytes. No callees. The engine/CSprite.h layout (PDB: Height is an
// unsigned long at +0x19C) does not fit a float read at +0x48, so the owner is modelled locally; the manifest label
// "CSprite::GetHeight" is kept as the display name.
struct CSprite_00b1b5e0 {
    unsigned char _pad00[0x48];
    float         m_height;                     // +0x48
    long GetHeight() const;
};
// the engine's pointer-out float-to-long helper: expanded in place, its two by-value parameters get frame copies
__forceinline void FloatToLong(long* result, float value)
{
    __asm fld value
    __asm mov edx, result
    __asm fistp dword ptr [edx]
}
long CSprite_00b1b5e0::GetHeight() const
{
    long height;
    FloatToLong(&height, m_height);
    return height;
}