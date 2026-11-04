// NScript::CV_FisticuffsClubScript::CTyler::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CTyler@CV_FisticuffsClubScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E83610, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e82338 "Tyler" in 00e82220; Alloc 00e83570; vtable 012eb0c0 slot 3; PDB ?GetParentScript@CTyler@CV_FisticuffsClubScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e83610.
//   ?GetParentScript@CTyler@CV_FisticuffsClubScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/NScript_CV_FisticuffsClubScript.h (shared header: nested class CTyler, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/e8/NScriptCVFisticuffsClubScriptCTyler_FightWarmupRounds_00e8a2b0.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include <string.h>
#include "engine/EOpinionDeedType.h"
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
#include "engine/NScript_CV_FisticuffsClubScript.h"
CScriptBase* NScript::CV_FisticuffsClubScript::CTyler::GetParentScript() const
{
    return &ParentClass;
}
