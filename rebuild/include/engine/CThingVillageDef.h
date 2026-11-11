#ifndef FABLE_ENGINE_CTHINGVILLAGEDEF_H
#define FABLE_ENGINE_CTHINGVILLAGEDEF_H
// CThingVillageDef (new shared header, agentNF9, row 0x0083B520 CThingVillage::GetCurrentGraphic): Ego_r.pdb CThingVillageDef
// (struct_layouts_egor.tsv, 0x84 bytes; class key V; RTTI .?AVCThingVillageDef@@ : CThingBaseDef) on the retail CThingBaseDef (0x70,
// Ego_r 0x74): Graphic +0x70 (Ego_r +0x74; retail copies the 14 bytes from +0x70, 0x0083B5C8).
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
class CThingVillageDef : public CThingBaseDef {
public:
    CEngineGraphic Graphic;                                              // +0x70
};
#endif
