// CAIStateGroup_Guildmaster::Update() -- the guildmaster boss's AI tick: while helpers live and no shield spell is up, cast the will
//   shield (CCombatAction_WillShieldInto 1800 s / 5 hp); stop if no longer runnable; with no living helpers cancel the shield spell,
//   else once MinSecondsBetweenWillUse has passed either teleport (MinSecondsBetweenTeleport passed and a better position found) or
//   gather bodyguards and cycle shield / heal / speed-up followers (AttackNum % 3), stamping LastTeleportFrame / LastWillFrame;
//   terminate when no longer runnable. The CGuildMasterDef sub-def pointer is released on every exit.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977E00, length 732 bytes (vtable 0x0128D444 slot 12).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (expanded GetTC), CDefClassBase::GetSubDef<CGuildMasterDef>
//   @ 0x00978190 (same-TU COMDAT: visible body, kept out of line), CTCBoss::CountLivingHelpers @ 0x007AEEC0,
//   CCombatAction_WillShieldInto ctor @ 0x00956760, ~CCombatActionBase @ 0x0062E900, CAIStateGroupBase::AIAction_PerformActionAndWait
//   @ 0x0097DB70, IsStillRunnable @ 0x009779A0, CAIStateGroupBase::Terminate @ 0x008FCF40, CAIStateGroupBase::GetWorld @ 0x008FCFC0,
//   CWorld::GetFrame @ 0x0049D870, CTCPhysicalShieldSpell::CancelMove @ 0x0079E300, GetBetterTeleportPos @ 0x00977820,
//   CCreatureAction_TeleportOut ctor @ 0x008BCFB0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, FindAndStoreBodyguards @ 0x00977B10,
//   CCombatAction_Guildmaster_ShieldFollowersInto / HealFollowersInto / SpeedUpFollowersInto ctors @ 0x00978510 / 0x009784A0 / 0x00978580,
//   the def's slot-1 Delete; global CMainGameComponent::ConstantFPS @ 0x01375550.
// Shape: the shield cancel is one `if (GetTC(..., &shield))` (a single bit test); the final runnable test is `{ Terminate(); return; }`,
//   which keeps two copies of the def release epilogue as retail does.
// Leaf: ?Update@CAIStateGroup_Guildmaster@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_GUILDMASTER
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBoss.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatAction_WillShieldInto.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_Guildmaster.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefClassBase_GetSubDef.h"

class CGuildMasterDef : public CSubDefClassBase {                    // RTTI .?AVCGuildMasterDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CGuildMasterDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    float MinSecondsBetweenTeleport;                                  // +0x28
    float MinSecondsBetweenWillUse;                                   // +0x2c
};
class CCombatAction_Guildmaster_ShieldFollowersInto : public CCombatActionBase {   // 0x00978510
public:
    CCombatAction_Guildmaster_ShieldFollowersInto(CThingCreatureBase& owner);
};
class CCombatAction_Guildmaster_HealFollowersInto : public CCombatActionBase {     // 0x009784A0
public:
    CCombatAction_Guildmaster_HealFollowersInto(CThingCreatureBase& owner);
};
class CCombatAction_Guildmaster_SpeedUpFollowersInto : public CCombatActionBase {  // 0x00978580
public:
    CCombatAction_Guildmaster_SpeedUpFollowersInto(CThingCreatureBase& owner);
};
void CAIStateGroup_Guildmaster::Update()
{
    CThingAICreature& creature = GetCreature();
    CTCBoss* boss;
    creature.GetTC(TCI_BOSS, &boss);
    CDefPointer<const CGuildMasterDef> def;
    creature.PeekBaseDef().GetSubDef(&def);
    if (!GetCreature().HasTC(TCI_PHYSICAL_SHIELD_SPELL) && boss->CountLivingHelpers() > 0)
        AIAction_PerformActionAndWait(CCombatAction_WillShieldInto(GetCreature(), 1800.0f, 5.0f), false);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CWorld* world = &GetWorld();
    long fps = CMainGameComponent::GetConstantFPS();
    float seconds_since_will = (float)(world->GetFrame() - LastWillFrame) / fps;
    if (boss->CountLivingHelpers() == 0) {
        CTCPhysicalShieldSpell* shield;
        if (GetCreature().GetTC(TCI_PHYSICAL_SHIELD_SPELL, &shield))
            shield->CancelMove();
    } else if (seconds_since_will > def->MinSecondsBetweenWillUse) {
        CWorld* world2 = &GetWorld();
        long fps2 = CMainGameComponent::GetConstantFPS();
        C3DVector pos;
        if ((float)(world2->GetFrame() - LastTeleportFrame) / fps2 > def->MinSecondsBetweenTeleport
            && GetBetterTeleportPos(pos)) {
            AIAction_PerformActionAndWait(CCreatureAction_TeleportOut(GetCreature(), *PEnemy.Get(), pos, true), false);
            LastTeleportFrame = GetWorld().GetFrame();
        } else {
            FindAndStoreBodyguards();
            switch (AttackNum % 3) {
            case 0:
                AIAction_PerformActionAndWait(CCombatAction_Guildmaster_ShieldFollowersInto(GetCreature()), false);
                break;
            case 1:
                AIAction_PerformActionAndWait(CCombatAction_Guildmaster_HealFollowersInto(GetCreature()), false);
                break;
            case 2:
                AIAction_PerformActionAndWait(CCombatAction_Guildmaster_SpeedUpFollowersInto(GetCreature()), false);
                break;
            }
            ++AttackNum;
            LastWillFrame = GetWorld().GetFrame();
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
}