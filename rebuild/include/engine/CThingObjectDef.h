#ifndef FABLE_ENGINE_CTHINGOBJECTDEF_H
#define FABLE_ENGINE_CTHINGOBJECTDEF_H
#ifdef FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
class CThingObjectDef: public CThingBaseDef {
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 virtual void Validate(const CValidateDefinitionInitBase & p0, const CCharString & p1) const;
 virtual void Transfer(CPersistContext & p0);
 CThingObjectDef(const CThingObjectDef & p0);
 CThingObjectDef();
 virtual ~CThingObjectDef();
 CThingObjectDef & operator=(const CThingObjectDef & p0);
public:
 CEngineGraphic Graphic;
 float RotationTime;
 long Material;
 long FloorMaterial;
 long OnDeathObject;
 unsigned long Property;
 long DieOffTimer;
 bool IsFragile:1,IsClothing:1,IsSelfIlluminating:1,UseHighDetailQuadTree:1,Electrocutable:1;
};
FABLE_STATIC_ASSERT(sizeof(CThingObjectDef)==0x9c);

#else
// CThingObjectDef (new shared header, chunk 867, row 0x00590F0B CTCInventory::GetItemsListUIComponentPC): the def class of a
// plain game object. RTTI .?AVCThingObjectDef@@ : CThingBaseDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase
// (ghidra_out/rtti_bases.tsv). The row holds one in a CDefPointer<const CThingObjectDef> (the typed handle
// ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z 0x0042B040 fills) and only
// calls the CDefClassBase sub-def lookup on it, so the class adds no members here: the Ego_r.pdb donor carries no member layout
// for it (struct_layouts_egor.tsv lists padding only) and no landed row reads one. No sizeof assertion (unproven).
#include "engine/CThingBaseDef.h"
class CThingObjectDef : public CThingBaseDef {
};
#endif
#endif // FABLE_ENGINE_CTHINGOBJECTDEF_H
