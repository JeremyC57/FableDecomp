#ifndef FABLE_ENGINE_CTHINGPHYSICALSWITCHDEF_H
#define FABLE_ENGINE_CTHINGPHYSICALSWITCHDEF_H
// CThingPhysicalSwitchDef (new shared header, agentNF9, rows 0x0083D900 / 0x0083DDC0 / 0x0083DF20): Ego_r.pdb CThingPhysicalSwitchDef
// (struct_layouts_egor.tsv, 0x98 bytes; class key V; RTTI .?AVCThingPhysicalSwitchDef@@ : CThingBaseDef) on the retail CThingBaseDef
// (0x70, Ego_r 0x74): TriggerType +0x70, UntriggeredGraphic +0x74, TriggeredGraphic +0x82 (two byte-packed 14-byte CEngineGraphic),
// TriggerSound +0x90 (Ego_r +0x74 / +0x78 / +0x86 / +0x94). The enum's enumerators are in no name set on disk.
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
enum EPSwitchTriggerType { };                                            // Ego_r.pdb enum (4 bytes); enumerators unproven
class CThingPhysicalSwitchDef : public CThingBaseDef {
public:
    EPSwitchTriggerType TriggerType;                                     // +0x70
    CEngineGraphic UntriggeredGraphic;                                   // +0x74
    CEngineGraphic TriggeredGraphic;                                     // +0x82
    long TriggerSound;                                                   // +0x90
};
#ifdef FABLE_RETAIL_CTHINGBASEDEF
FABLE_STATIC_ASSERT(sizeof(CThingPhysicalSwitchDef)==0x94);
#endif
#endif
