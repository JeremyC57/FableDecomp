#ifndef FABLE_ENGINE_CTHINGOBJECTDEF_H
#define FABLE_ENGINE_CTHINGOBJECTDEF_H
// CThingObjectDef (new shared header, chunk 867, row 0x00590F0B CTCInventory::GetItemsListUIComponentPC): the def class of a
// plain game object. RTTI .?AVCThingObjectDef@@ : CThingBaseDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase
// (ghidra_out/rtti_bases.tsv). The row holds one in a CDefPointer<const CThingObjectDef> (the typed handle
// ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z 0x0042B040 fills) and only
// calls the CDefClassBase sub-def lookup on it, so the class adds no members here: the Ego_r.pdb donor carries no member layout
// for it (struct_layouts_egor.tsv lists padding only) and no landed row reads one. No sizeof assertion (unproven).
#include "engine/CThingBaseDef.h"
#ifdef FABLE_RETAIL_CTHINGOBJECTDEF_GRAPHIC
// Retail opt-in (chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the first CThingObjectDef member, Ego_r.pdb
// CEngineGraphic Graphic at donor +0x74, retail +0x70 (CThingBaseDef is 0x70 in retail): the row reads Graphic.BankIndex at +0x70
// (`mov edx,[ecx+70h]`, 0x0060B3B5) and Graphic.Type at +0x7D. The members after it are not modelled (no sizeof assertion).
#include "engine/CEngineGraphic.h"
class CThingObjectDef : public CThingBaseDef {
public:
    CEngineGraphic Graphic;                                              // +0x70
};
#else
class CThingObjectDef : public CThingBaseDef {
};
#endif
#endif // FABLE_ENGINE_CTHINGOBJECTDEF_H
