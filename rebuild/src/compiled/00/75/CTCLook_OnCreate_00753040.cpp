// CTCLook::OnCreate() -- fetch the look and appearance sub-defs of the owning thing's def into PDef / PAppearanceDef, then centre the head and zero the eye angles
// Leaf: ?OnCreate@CTCLook@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753040, length 53 bytes.
// Callees: CDefClassBase::GetSubDef<CLookDef> 0x007550F0 and GetSubDef<CAppearanceDef> 0x0042B0A2 (on the parent thing's base def,
// CThing::PeekBaseDef inline: PBaseDef +0x70), CTCLook::CentreHead 0x007528A0, CTCLook::ChangeEyeAngles(float, float) 0x00752200
// (0.0f, 0.0f pushed as zero dwords).
// Layout: engine/CTCLook.h (PDef +0x14, PAppearanceDef +0x18). Shape: the owner chain is re-read for each lookup (the calls may change it).
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CThing / CTCLook stand-ins onto the shared headers: the sub-def
// lookups bind ??$GetSubDef@VCLookDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCLookDef@@@@@Z (was
// ?GetSubDef@CDefClassBase@@QAE_NAAUCLookDefPointer@@@Z) and GetSubDef<CAppearanceDef> 0x0042B0A2; CentreHead / ChangeEyeAngles bind the
// PDB private spellings (AAE; were QAE).
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CTCLook.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CLookDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CDefinitionManager.h"

void CTCLook::OnCreate()
{
    ParentThing->PeekBaseDef().GetSubDef(&PDef);
    ParentThing->PeekBaseDef().GetSubDef(&PAppearanceDef);
    CentreHead();
    ChangeEyeAngles(0.0f, 0.0f);
}
