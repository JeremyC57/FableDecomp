// NScript::CV_ChickenKickingScript::CSpectator::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CSpectator@CV_ChickenKickingScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E63700, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e62c15 "Spectator" in 00e62a00; Alloc 00e63660; vtable 012e74b4 slot 3; PDB ?GetParentScript@CSpectator@CV_ChickenKickingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e63700.
//   ?GetParentScript@CSpectator@CV_ChickenKickingScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/NScript_CV_ChickenKickingScript.h (shared header: nested class CSpectator, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/e6/NScriptCVChickenKickingScriptCSpectator_Main_00e63890.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#include "engine/EHeroAbility.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/ECutsceneBehaviour.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
#include "engine/NScript_CV_ChickenKickingScript.h"
CScriptBase* NScript::CV_ChickenKickingScript::CSpectator::GetParentScript() const
{
    return &ParentClass;
}
