// CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool appear) const -- create an AI creature from the named def at the position (the def's player, a CThingAICreatureInit carrying the script name), return its script thing; optionally start the creature's appear action, then wait CreateCreatureDelayFrames script frames
// Leaf: ?CreateCreature@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@0_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9100, length 532 bytes (virtual, `ret 14h`: hidden result + 4 args).
// Callees: CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefinitionManager::GetDef<CThingCreatureDef>
//   0x00513160 (same-TU auto_inline(off) stand-in; it only fills the out slot), CCharString ctor/copy/dtor/operator=
//   0x0099EBF0 / 0x0099EC30 / 0x0099EAE0 / 0x0099EFB0, CThingAICreature::Create 0x00833800 (static __fastcall),
//   CThing::GetScriptThing 0x004C7E50, CScriptThing::operator= 0x008AB980, CBaseClass ctor/dtor 0x0099A2D0 /
//   0x0099A2E0, operator delete 0x00BFE9BC, the appear action's ctor 0x0074DBB0 (manifest label
//   "CCombatAction_Guildmaster_HealFollowersInto" is propagated) and dtor 0x0062E900, CThingCreatureBase::SetCurrentAction
//   0x006644F0, GSI slot 7 NewScriptFrame. CScriptThing vtable 0x01238C8C; "" 0x0122D70E.
// Layout: CThingAICreatureInit is the Ego_r.pdb record (28 bytes). CGameScriptInterface is modelled locally: retail
//   STLport maps are 12 bytes where the PDB-generated engine/CGameScriptInterface.h has 16, so the retail
//   CreateCreatureDelayFrames is at +0x4C (the header's +0x50); the loop reads [this+4Ch].
// Shape: the init's two CCharString default arguments are `""` literals (converted temporaries built before the
//   copies, passed by `lea`); `result` is declared before the def pointer; the delay loop counts down from the member.
// Port (NF98, round 8, q11261 / q11262): the local CCharString / CThingCreatureDef / CDefPointer / CDefinitionManager / CThingAICreatureInit /
//   CThingAICreature / CCreatureActionAppear models and the same-TU GetDef stand-in replaced by the shared headers: the appear action is
//   CCreatureAction_OnGenerate (ctor 0x0074DBB0 stores vtable 0x01268DEC, RTTI .?AVCCreatureAction_OnGenerate@@; Ego_r.pdb / FableWin.pdb
//   ??0CCreatureAction_OnGenerate@@QAE@AAVCThingCreatureBase@@@Z), was a CCreatureActionAppear stand-in; SetCurrentAction 0x006644F0 and
//   ~CCombatActionBase 0x0062E900 bind the PDB spellings; Create is ?Create@CThingAICreature@@SIPAV1@JABVC3DVector@@JABVCThingAICreatureInit@@@Z
//   (init by reference); the def's +0x11C is CThingCreatureDef::DefaultOwner; GetDef<CThingCreatureDef> is the header member template with
//   its noinline body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, retail 0x00513160).
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS  // NF264 (q12750): CCombatActionBase::AllowsCombinationAnims override, so emitted tables bind slot 66 = 0x0062EA10 as retail
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the landed row's choice, kept): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_OnGenerate.h"
class CWorld; class CMainGameComponent; class CDisplayEngine;
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position,
                                                  const CCharString& scriptName, bool appear) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CScriptThing result;
    CDefPointer<const CThingCreatureDef> def;
    if (DefinitionManager->GetDef(index, &def)) {
        CThingAICreatureInit init;
        init.ScriptName = scriptName;
        CThingAICreature* creature = CThingAICreature::Create(index, position, def->DefaultOwner, init);
        result = creature->GetScriptThing();
        if (appear)
            creature->SetCurrentAction(CCreatureAction_OnGenerate(*creature));
        for (long i = CreateCreatureDelayFrames; i > 0; --i)
            const_cast<CGameScriptInterface*>(this)->NewScriptFrame();
    }
    return result;
}
