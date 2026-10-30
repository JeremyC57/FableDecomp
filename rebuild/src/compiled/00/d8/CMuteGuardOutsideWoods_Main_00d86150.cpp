// CMuteGuardOutsideWoods::Main() -- a mute guard outside the Guildmaster's woods: alive filter on Me, scripted control (priority 4, retried each frame), then idle until the hero hits him (directly, or with any special ability but number 14) or the outside-wood guards are already attacking; then raise the attack flag, target the hero and drop scripted control
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D86150, 563 bytes (+ int3 padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseObject ctor 0x0099A380, the scripted-thing handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0,
//   CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 21 MsgIsHitBy, 41 MsgIsHitBySpecialAbilityFrom, 42
//   MsgIsHitByAnySpecialAbilityFrom; GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 451
//   GiveThingBestEnemyTarget. String "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_HeroSoulsGuildmasterScript: OutsideWoodGuardsAttack
//   PDB +0x5f = retail +0x4b, as in the landed CGuardInWoods::Main 0x00D864B0), PMasterData +0x18. Models from the landed
//   CBRIAR_ROSE_IMPOSTER::Main 0x00D81820.
// Shape: the wait condition owns three conditional CCharString temporaries (flag word in ebx, destroyed in reverse) and is
//   materialised as a bool before the branch.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void s000();   // slot 0 (+0x0)
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008();
    virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016();
    virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029();
    virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037();
    virtual void s038(); virtual void s039(); virtual void s040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
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
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CQ_HeroSoulsGuildmasterScript {
    unsigned char _pad[0x48];
    bool PlayedBriarCS;                                // +0x48
    bool PlayedGuildMasterCS;                          // +0x49
    bool PlayedGuildCS;                                // +0x4a
    bool OutsideWoodGuardsAttack;                      // +0x4b
};
struct CMuteGuardOutsideWoods : CEntityScriptBase {
    CQ_HeroSoulsGuildmasterScript* ParentClass;        // +0x14
    void*                          PMasterData;        // +0x18
    void Main();
};

void CMuteGuardOutsideWoods::Main()
{
    {
        CIsThingAlive filter(Me);
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
    while (!(Me.MsgIsHitBy("SCRIPT_NAME_HERO")
             || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                 && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))
             || ParentClass->OutsideWoodGuardsAttack)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->OutsideWoodGuardsAttack = true;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (scripted.IsScripting())
        scripted.Release();
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}