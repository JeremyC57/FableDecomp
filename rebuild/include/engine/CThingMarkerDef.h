#ifndef FABLE_ENGINE_CTHINGMARKERDEF_H
#define FABLE_ENGINE_CTHINGMARKERDEF_H
// CThingMarkerDef (new shared header, agentNF9, rows 0x0083D190 / 0x0083D280): Ego_r.pdb CThingMarkerDef (struct_layouts_egor.tsv,
// 0x8C bytes; class key V; RTTI .?AVCThingMarkerDef@@ : CThingBaseDef) on the retail CThingBaseDef (0x70, Ego_r 0x74): Graphic +0x70,
// TriggerType +0x80, Properties +0x84 (Ego_r +0x74 / +0x84 / +0x88; retail GetProperties reads +0x84, 0x0083D2C5). The enum's
// enumerators are in no name set on disk.
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
enum ESwitchTriggerType { };                                             // Ego_r.pdb enum (4 bytes); enumerators unproven
class CThingMarkerDef : public CThingBaseDef {
public:
    CEngineGraphic Graphic;                                              // +0x70
    ESwitchTriggerType TriggerType;                                      // +0x80
    unsigned long Properties;                                            // +0x84
};
#ifdef FABLE_RETAIL_CTHINGBASEDEF
FABLE_STATIC_ASSERT(sizeof(CThingMarkerDef)==0x88);
#endif
#endif
