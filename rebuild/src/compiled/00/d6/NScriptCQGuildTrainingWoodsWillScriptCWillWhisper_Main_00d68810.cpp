// NScript::CQ_GuildTrainingWoodsWillScript::CWillWhisper::Main() -- Whisper during Will training in the guild woods: conscious filter on Me, scripted control (priority 4, retried each frame), make her unkillable, combo-neutral, hero faction and boss-phase locked; then while the bandits are alive, re-join the hero faction and befriend everything when the hero hits her, and play the level-1 force-spell cast whenever the quest raises the whisper-animate flag
// Leaf: ?Main@CWillWhisper@CQ_GuildTrainingWoodsWillScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first); MsgIsHitBySpecialAbilityFrom takes EHeroAbility (slot 41, 0x004AAD20: ?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z in Ego_r.pdb and FableWin.pdb); the literal 14 is HERO_ABILITY_HEAL_LIFE_SPELL (engine/EHeroAbility.h, Ego_r enum); engine/EHeroAbility.h included (Ego_r enum EHeroAbility).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D68810, 719 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseObject ctor 0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation
//   0x007E73D0 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 21 MsgIsHitBy, 41
//   MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom, 67 SetFriendsWithEverythingFlag; GameCalls slots 7, 8,
//   517 EntitySetAsKillable, 521 EntitySetAllowBossPhaseChanges, 526 EntitySetAsToAddToComboMultiplierWhenHit, 601
//   EntitySetInFaction. Strings "FACTION_HERO" 0x0126C3A4, "SCRIPT_NAME_HERO" 0x0125D1C8,
//   "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1" 0x0127B850; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_GuildTrainingWoodsWillScript: WhisperAnimate
//   PDB +0x5f = retail +0x4b, BanditsAlive PDB +0x60 = retail +0x4c, the 0x14 script-base shift of the landed
//   CMuteGuardOutsideWoods::Main 0x00D86150), PMasterData +0x18. Filter CIsThingConscious (vtable 0x012C2FE8).
// Shape: the sibling's model; the hit test owns three conditional CCharString temporaries (flag word in ebx) and is
//   materialised as a bool; the loop exit's terminate test has no use, so its result is dropped.
// Naming wave 35: identity reg 00d678f5 "WillWhisper" in 00d67890; Alloc 00d68720; vtable 012d1f60 slot 1; PDB ?Main@CWillWhisper@CQ_GuildTrainingWoodsWillScript@NScript@@; refs=1; was under-qualified CWillWhisper::Main (wave3/labels_alloc.tsv); PDB ?Main@CWillWhisper@CQ_GuildTrainingWoodsWillScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CWillWhisper@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_GuildTrainingWoodsWillScript moved into NScript); same bytes. Also: global 0x01375748 respelled NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/C3DVector.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/EHeroAbility.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectScriptedThingBase;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingConscious : public NScript::CPredicateBase {                                 // vtable 0x012C2FE8: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748 (established spelling; naming wave 35: was g_bScriptAnimationFlag)
namespace NScript {
class CQ_GuildTrainingWoodsWillScript {
public:
    unsigned char _pad[0x48];
    bool MissionSucceeded;                             // +0x48
    bool MissionFailed;                                // +0x49
    bool MissionOver;                                  // +0x4a
    bool WhisperAnimate;                               // +0x4b
    bool BanditsAlive;                                 // +0x4c
public:
    class CWillWhisper;              // the entity class below (naming wave 35)
};
}
using NScript::CQ_GuildTrainingWoodsWillScript;
namespace NScript {
class CQ_GuildTrainingWoodsWillScript::CWillWhisper : public CEntityScriptBase {
public:
    CQ_GuildTrainingWoodsWillScript* ParentClass;      // +0x14
    void*                            PMasterData;      // +0x18
private:
    virtual void Main();                          // PDB ?Main@CWillWhisper@CQ_GuildTrainingWoodsWillScript@NScript@@EAEXXZ
};
}

void NScript::CQ_GuildTrainingWoodsWillScript::CWillWhisper::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAllowBossPhaseChanges(Me, false);
    while (ParentClass->BanditsAlive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetInFaction(Me, "FACTION_HERO");
            Me.SetFriendsWithEverythingFlag(true);
        }
        if (ParentClass->WhisperAnimate == true) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->WhisperAnimate = false;
            scripted.PlayAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
        }
    }
    if (IsActiveThreadTerminating()) return;
}