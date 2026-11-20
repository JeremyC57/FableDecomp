// CThingCreatureBase::GetProperty() const -- returns the creature definition's Property (EThingCreatureProperty, opaque in the shared header)
// Leaf: ?GetProperty@CThingCreatureBase@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662010, 13 bytes, thiscall: `mov eax,[ecx+0E0h] ; mov eax,[eax+0F0h] ; ret`.
// Names (NF115, PRIORITY q4488; was the free stand-in `int __fastcall CountReferences(Outer*)`, catalog CPixelShader::CountReferences):
//   FableWin.pdb + Ego_r.pdb ?GetProperty@CThingCreatureBase@@QBEKXZ (declared in engine/CThingCreatureBase.h's retail branch); the body
//   reads the def's Property word (the virtual CThingAICreature::GetProperties 0x00830140, slot 48, has the same body and lands with the same
//   expression); CTCVillage::AddThingToVillage 0x0065B854 compares it with 3 to count guards. The neighbouring 0x00662000 is
//   GetCreatureType (the def's +0xC4 word).
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
unsigned long CThingCreatureBase::GetProperty() const
{
    return *reinterpret_cast<const unsigned long*>(PDef->Property);
}
