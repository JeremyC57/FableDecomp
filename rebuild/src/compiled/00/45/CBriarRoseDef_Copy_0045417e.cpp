// CBriarRoseDef::Copy(const CDefClassBase* other) -- copy every member of another CBriarRoseDef: `*this = *(const CBriarRoseDef*)other;`
// Leaf: ?Copy@CBriarRoseDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045417E, 5 bytes: a thiscall tail call, `jmp` to CBriarRoseDef::operator= 0x00454183.
// Identity (NF425, 2026-11-25): self ?Copy@CBriarRoseDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CBriarRoseDef@@QAEAAV0@ABV0@@Z: bound at 0x00454183 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CBriarRoseDef_Copy@@YIXPAVCBriarRoseDef@@PBV1@@Z; now the real virtual member, so the
//   RTTI-placed name is defined.
// Header: the class's own header engine/CBriarRoseDef.h with the prelude of 00/44/CBriarRoseDef_GetSizeofClass_0044c653.cpp (the header
//   declares Copy and operator=).
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#include "engine/CThingAICreature.h"
#include "engine/CBriarRoseDef.h"

void CBriarRoseDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBriarRoseDef*)other;
}
