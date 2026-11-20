// CCombatActionBase::SetUpCombatAnimationNotes() -- build the strike events and interrupts; with strike events present and HasPerformAction clear, ActionTime becomes -1
// Leaf: ?SetUpCombatAnimationNotes@CCombatActionBase@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859EC0, length 44 bytes (+ int3 padding).
// Callees: CCombatActionBase::SetupCombatStrikeEvents @ 0x00859A70, CCombatActionBase::SetupCombatInterrupts @ 0x00858A20 (both non-virtual on this).
// StrikeEvents is the retail std::list at +0xbc; its emptiness is tested as begin() != end() (`mov eax,[esi+0BCh] ; cmp [eax],eax`), not through
//   the size. ActionTime +0x14 (CCreatureActionBase); HasPerformAction +0xDE (Ego_r.pdb CCombatActionBase member list).
// Re-port (coordinator, 2026-11-20): moved from a local layout model onto engine/CCombatActionBase.h with the TU-mate 0x00859EF0's define set
//   (TU-mate contract), so 0x00859EF0's same-TU copy of this body is verbatim. m_keepActionTime respelled HasPerformAction (p25N NAME_FIXES).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CANIMCOMPONENTCOMBATHANDEDNESS_GETTERS
#define FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CTCFLOURISHTARGET_SETTERS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SETGAMESCRIPTEDMODE
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEBASE_SETTERS
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULESPLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingSearchTools.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentCombatHandedness.h"
#include "engine/CAnimComponentCombatMisc.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCombat.h"
#include "engine/CTCFlourishTarget.h"
#include "engine/CThingSearchTools.h"
#include "engine/GFRandom.h"
#include "engine/NCameraModes_CScriptModuleBase.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"

void CCombatActionBase::SetUpCombatAnimationNotes()
{
    SetupCombatStrikeEvents();
    SetupCombatInterrupts();
    if (StrikeEvents.begin() != StrikeEvents.end() && !HasPerformAction)
        ActionTime = -1;
}
