// CSprite_00b1b5e0::GetHeight() const -- the float height at +0x48 rounded to a long with the FPU's current rounding mode
// Leaf: GetHeight
// Retail-asm: 0x00B1B5F1 `fld dword ptr [esp+8] ; mov edx,[esp+4] ; fistp dword ptr [edx]` -- the engine's inline-assembly float-to-long helper taking the value and the result address by value (both homed in the 12-byte frame); VC7.1 C++ `(long)x` tail-jumps __ftol2 and /QIfist emits `fistp qword` without the pointer indirection.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFastFloatToLong.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B5E0, 34 bytes. No callees. The engine/CSprite.h layout (PDB: Height is an
// unsigned long at +0x19C) does not fit a float read at +0x48, so the owner is modelled locally; the manifest label
// "CSprite::GetHeight" is kept as the display name.
struct CSprite_00b1b5e0 {
    unsigned char _pad00[0x48];
    float         m_height;                     // +0x48
    long GetHeight() const;
};
// the engine's pointer-out float-to-long helper: expanded in place, its two by-value parameters get frame copies
#include "engine/GFFastFloatToLong.h"
long CSprite_00b1b5e0::GetHeight() const
{
    long height;
    height = GFFastFloatToLong(m_height);
    return height;
}