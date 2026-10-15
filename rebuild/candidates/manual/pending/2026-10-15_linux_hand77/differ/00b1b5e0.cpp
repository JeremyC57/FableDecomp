// CSprite_00b1b5e0::GetHeight() const -- the float height at +0x48 rounded to a long with the FPU's current rounding mode (fistp)
// Grade: DIFFER(ours v retail) -- 8v34: retail is an inlined inline-asm rounding helper (float and result address spilled to a 12-byte frame, `fld ; mov edx,[ptr] ; fistp dword ptr [edx]`); the pure C++ conversion tail-jumps __ftol2. Not reproducible without __asm (asm_bake)
// Leaf: GetHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B5E0, 34 bytes. No callees, no callers by direct call (virtual or
// table-referenced). The engine/CSprite.h layout (PDB: Height is an unsigned long at +0x19C) does not fit a float read at
// +0x48, so the owner is modelled locally; the manifest label "CSprite::GetHeight" is kept as the display name.
// Shape: retail spills the float into a 12-byte frame together with the address of the result slot, then `fld` / `mov
// edx,[ptr] ; fistp dword ptr [edx]` -- the code VC7.1 emits for an inlined helper whose body is `__asm` (fld value ;
// mov edx,out ; fistp [edx]) operating on its parameters. Plain C++ can only convert through __ftol2 (truncation).
struct CSprite_00b1b5e0 {
    unsigned char _pad00[0x48];
    float         m_height;                     // +0x48
    long GetHeight() const;
};

long CSprite_00b1b5e0::GetHeight() const
{
    return (long)m_height;
}
