// CWillWhisper::Main() -- Whisper during Will training in the guild woods: conscious filter on Me, scripted control (priority 4, retried each frame), make her unkillable, combo-neutral, hero faction and boss-phase locked; then while the bandits are alive, re-join the hero faction and befriend everything when the hero hits her, and play the level-1 force-spell cast whenever the quest raises the whisper-animate flag
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D68810, 719 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseObject ctor 0x0099A380, the scripted-thing handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation
//   0x007E73D0 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 21 MsgIsHitBy, 41
//   MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom, 67 SetFriendsWithEverythingFlag; GameCalls slots 7, 8,
//   517 EntitySetAsKillable, 521 EntitySetAllowBossPhaseChanges, 526 EntitySetAsToAddToComboMultiplierWhenHit, 601
//   EntitySetInFaction. Strings "FACTION_HERO" 0x0126C3A4, "SCRIPT_NAME_HERO" 0x0125D1C8,
//   "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1" 0x0127B850; g_bScriptAnimationFlag 0x01375748.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_GuildTrainingWoodsWillScript: WhisperAnimate
//   PDB +0x5f = retail +0x4b, BanditsAlive PDB +0x60 = retail +0x4c, the 0x14 script-base shift of the landed
//   CMuteGuardOutsideWoods::Main 0x00D86150), PMasterData +0x18. Filter CIsThingConscious (vtable 0x012C2FE8).
// Shape: the sibling's model; the hit test owns three conditional CCharString temporaries (flag word in ebx) and is
//   materialised as a bool; the loop exit's terminate test has no use, so its result is dropped.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing;
class CScriptGameResourceObjectScriptedThingBase;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029();
    virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037();
    virtual void s038(); virtual void s039(); virtual void s040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    virtual void s043(); virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050();
    virtual void s051(); virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058();
    virtual void s059(); virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066();
    virtual void SetFriendsWithEverythingFlag(bool on);   // slot 67 (+0x10c)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
class CBaseObject { public:                                   // CBaseIntelligentPointer root
    virtual void b00();
    void* m_pIntelligentPointer;                       // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
public:
    virtual void PlayAnimation(const CCharString&, bool, bool, bool, bool, bool, bool, bool);   // 0x007E73D0
};
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
extern bool g_bScriptAnimationFlag;                   // 0x01375748
struct CQ_GuildTrainingWoodsWillScript {
    unsigned char _pad[0x48];
    bool MissionSucceeded;                             // +0x48
    bool MissionFailed;                                // +0x49
    bool MissionOver;                                  // +0x4a
    bool WhisperAnimate;                               // +0x4b
    bool BanditsAlive;                                 // +0x4c
};
struct CWillWhisper : CEntityScriptBase {
    CQ_GuildTrainingWoodsWillScript* ParentClass;      // +0x14
    void*                            PMasterData;      // +0x18
    void Main();
};

void CWillWhisper::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
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
                && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetInFaction(Me, "FACTION_HERO");
            Me.SetFriendsWithEverythingFlag(true);
        }
        if (ParentClass->WhisperAnimate == true) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->WhisperAnimate = false;
            scripted.PlayAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", false, false, false, true, g_bScriptAnimationFlag, false, false);
        }
    }
    if (IsActiveThreadTerminating()) return;
}