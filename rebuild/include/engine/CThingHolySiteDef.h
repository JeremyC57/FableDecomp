#ifndef FABLE_ENGINE_CTHINGHOLYSITEDEF_H
#define FABLE_ENGINE_CTHINGHOLYSITEDEF_H
// CThingHolySiteDef (new shared header, agentNF9, rows 0x0083BAB0 / 0x0083BB40 / 0x0083BBA0): Ego_r.pdb CThingHolySiteDef
// (struct_layouts_egor.tsv, 0x84 bytes; class key V; RTTI .?AVCThingHolySiteDef@@ : CThingBaseDef) on the retail CThingBaseDef (0x70,
// Ego_r 0x74): Graphic +0x70 (Ego_r +0x74, the 14-byte byte-packed CEngineGraphic) and PlayerStart +0x7E (Ego_r +0x82); retail reads the
// byte at +0x7E (0x0083BB8A / 0x0083BBDB `mov al,[eax+7Eh]`). Members only (no PDB member function is called by a landed row).
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
class CThingHolySiteDef : public CThingBaseDef {
public:
    CEngineGraphic Graphic;                                              // +0x70
    bool PlayerStart;                                                    // +0x7E
};
#ifdef FABLE_RETAIL_CTHINGBASEDEF
FABLE_STATIC_ASSERT(sizeof(CThingHolySiteDef)==0x80);                // the typed (4-aligned) retail CThingBaseDef; the opaque one is 1-aligned
#endif
#endif
