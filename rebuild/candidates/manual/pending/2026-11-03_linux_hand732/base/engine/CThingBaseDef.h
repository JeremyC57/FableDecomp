#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H
#include "engine/CParentDefClassBase.h"
// Original Ego_r CThingBaseDef derives CParentDefClassBase; native STLport sizeof70.
// Complete original Components through DefaultScriptData range retained opaque.
class CThingBaseDef : public CParentDefClassBase {
    unsigned char _pad_0x39[0x37];
};
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);
#ifdef FABLE_RETAIL_CTHING
// Retail opt-in (2026-11-03, row 0x005EB9E0): the body of CThing::PeekBaseDef (declared in engine/CThing.h; Ego_r.pdb
// ?PeekBaseDef@CThing@@QBEABVCThingBaseDef@@XZ, inline in retail: CTCTavernGame::FrameUpdate reads PBaseDef +0x70 in place,
// `mov edx,[esi+4]; mov ecx,[edx+70h]`, as the landed 0x007CF7E0 local model does). It needs the complete CThingBaseDef.
#include "engine/CThing.h"
inline const CThingBaseDef& CThing::PeekBaseDef() const { return *PBaseDef; }
#endif

#endif
