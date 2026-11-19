// CCombatAction_WillShieldInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastPhysicalShieldSpell(creature, 2, true, "", shield frames, ShieldHealth) as its current action
// Leaf: ?FinishAction@CCombatAction_WillShieldInto@@UAEXXZ
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C29F0, 144-byte slot (143 bytes of code). FinishAction is vtable slot 4.
// Callees: __ftol2 @ 0x00BFEA70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, a named local
//          fetched before the base call), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0 (empty literal),
//          CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool,
//          const CCharString&, long, float) @ 0x008BB6F0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//          the action temporary's destructor @ 0x0062E900, CCharString::~CCharString @ 0x0099EAE0.
// Globals: the game frames-per-second long @ 0x01375550.
// Shape: ShieldUpTime (seconds, retail +0x114 / PDB +0x12c) converts to frames first, kept in ebx across the calls:
// -1.0 (forever) maps to -1 (`or ebx,-1`), anything else to (long)(ShieldUpTime * fps) (`fild fps ; fmul`).
// The -1 test is an integer compare of the bit pattern (`cmp dword ptr [esi+114h],0BF800000h`, no FPU), so the
// source compares the float's bits. ShieldHealth (+0x118) is the float pushed last by value. The 0x128-byte frame
// holds the string word below the 0x124-byte action temporary (the cast-spell temporaries of 0x008C2A80 /
// 0x008C2980 are 0x11c bytes; the shield spell adds its two arguments as members).
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Port (NF93, q11261 / q11262): the local CThingCreatureBase_Calls / CCreatureAction_CastPhysicalShieldSpell_Retail / CCombatActionBase /
//   CCharString / CMainGameComponent stand-ins replaced by the shared headers (the 0x008C2980 recipe) with the retail WillShieldInto and
//   CastPhysicalShield (NF74 _CTOR) blocks and the NF93 opt-in CCOMBATACTION_WILLSHIELDINTO_FINISHACTION: SetCurrentAction 0x006644F0
//   and the temporary's destructor 0x0062E900 (~CCombatActionBase) now bind the PDB spellings; own leaf QAE -> UAE (slot-4 override).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD_CTOR
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCombatAction_WillShieldInto.h"
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"

void CCombatAction_WillShieldInto::FinishAction()
{
    long shield_frames;
    if (*(const long*)&ShieldUpTime == 0xBF800000)
        shield_frames = -1;
    else
        shield_frames = (long)(ShieldUpTime * CMainGameComponent::GetConstantFPS());
    CThingCreatureBase* pcreature = PCreature.Get();
    CCombatActionBase::FinishAction();
    CCharString anim_name("", -1);
    pcreature->SetCurrentAction(CCreatureAction_CastPhysicalShieldSpell(*pcreature, 2, true, anim_name, shield_frames, ShieldHealth));
}
