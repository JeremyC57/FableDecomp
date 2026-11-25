// CVillageDef::Copy(const CDefClassBase* other) -- copy every member of another CVillageDef: `*this = *(const CVillageDef*)other;`
// Leaf: ?Copy@CVillageDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1E01, 5 bytes: a thiscall tail call, `jmp` to CVillageDef::operator= 0x004E1E06.
// Identity (NF425, 2026-11-25): self ?Copy@CVillageDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CVillageDef@@QAEAAV0@ABV0@@Z: bound at 0x004E1E06 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CVillageDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CVillageDef.h with the prelude of 00/4d/CVillageDef_GetSizeofClass_004dff9f.cpp (the header
//   declares Copy and operator=).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CVillageDef.h"

void CVillageDef::Copy(const CDefClassBase* other)
{
    *this = *(const CVillageDef*)other;
}
