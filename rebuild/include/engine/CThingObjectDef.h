#ifndef FABLE_ENGINE_CTHINGOBJECTDEF_H
#define FABLE_ENGINE_CTHINGOBJECTDEF_H
// CThingObjectDef (new shared header, chunk 867, row 0x00590F0B CTCInventory::GetItemsListUIComponentPC): the def class of a
// plain game object. RTTI .?AVCThingObjectDef@@ : CThingBaseDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase
// (ghidra_out/rtti_bases.tsv). The row holds one in a CDefPointer<const CThingObjectDef> (the typed handle
// ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z 0x0042B040 fills) and only
// calls the CDefClassBase sub-def lookup on it, so the class adds no members here: the Ego_r.pdb donor carries no member layout
// for it (struct_layouts_egor.tsv lists padding only) and no landed row reads one. No sizeof assertion (unproven).
#include "engine/CThingBaseDef.h"
class CThingObjectDef : public CThingBaseDef {
};
#endif // FABLE_ENGINE_CTHINGOBJECTDEF_H
