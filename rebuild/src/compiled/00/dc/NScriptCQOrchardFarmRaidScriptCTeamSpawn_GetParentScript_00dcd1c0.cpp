// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::GetParentScript() const -- the entity script's parent quest script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCD1C0, 4 bytes: `mov eax,[ecx+14h] ; ret`.
// Identity (NF179, PRIORITY q12013): FableWin.pdb 0x03857120 ?GetParentScript@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UBEPAVCScriptBase@@XZ
//   + slot 3 of the CTeamSpawn entity vtable 0x012DB2D4 (stored only by the ctor 0x00DCD160) + the body (ParentClass +0x14, the
//   CQ_OrchardFarmRaidScript* converted to its CScriptBase base at offset 0); engine/CQ_OrchardFarmRaidScript.h declares it. The catalog
//   already names it. Was the gapscan stand-in `T::Get` (?Get@T@@QAEHXZ, no Leaf line); no landed row calls 0x00DCD1C0 (exe scan).
#include "fable_compiler.h"
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
#include "fable_string.h"
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)

CScriptBase* NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::GetParentScript() const
{
    return ParentClass;
}
