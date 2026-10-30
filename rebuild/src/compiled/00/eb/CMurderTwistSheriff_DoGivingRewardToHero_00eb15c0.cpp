// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoRemindingHeroToKillSheriff() -- the sheriff's reaction once the assassin is dead: talked to by the hero
//   -> paused movie sequence, "assassin killed" line while alive, 1000 gold, SheriffState 3; hit by the hero (or by any of his
//   special abilities except ability 14) -> wait for the enflame reaction, paused "hero sussed" line, SheriffState 2
// Leaf: ?DoGivingRewardToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Identity (naming wave 11): ?DoGivingRewardToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ (Ego_r.pdb / FableWin.pdb name lists); Ego_r.pdb / FableWin.pdb member order (DoBecomingSuspiciousOfHero 0x00EB10C0 < DoGivingRewardToHero < DoSayingWellDoneToHero 0x00EB19C0 < AttackHero 0x00EB1EC0); this body says TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED (the hero killed the assassin: the reward); invented DoRemindingHeroToKillSheriff
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB15C0, 1010 bytes (`ret`).
// Label: the catalog files 0x00EB15C0 under CMurderTwistAssassin, but the landed CMurderTwistSheriff::Main 0x00EAEF30 calls it
//   on the sheriff and it writes the parent quest's SheriffState (+0x48); the original method name is not in any PDB dump.
// Callees: CScriptThing slots 21 MsgIsHitBy / 27 MsgIsTalkedToBy / 41 MsgIsHitBySpecialAbilityFrom / 42
//   MsgIsHitByAnySpecialAbilityFrom (vtable 0x01238C8C), scripted-thing handle slots 12 GetScriptThing / 13 Speak / 26
//   IsPerformingScriptTask (vtable 0x0127094C), ~CScriptThing 0x004AA840, GameCalls slots 7 NewScriptFrame, 70 GetHero, 126
//   GiveHeroGold, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, IsActiveThreadTerminating 0x00F35B30,
//   CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary 0x00EB0700, CBaseObject 0x0099A380 / ~ 0x0099A430, movie PImp
//   dtor 0x006E7AB0, movie dtor 0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED" 0x012EFE78,
//   "TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED_HERO_SUSSED" 0x012EFE44.
// Shape: the movie object is the landed CMeetSisterMessenger::Main 0x00E292C0 destructor-less view: retail expands its
//   destructor at the first block's in-loop exit and both normal ends and calls the out-of-line copy elsewhere.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
enum EHeroAbility { HERO_ABILITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
    void DestroyOutOfLine();                            // the implicit destructor's out-of-line copy 0x006E7B80
};
class CScriptGameResourceObjectScriptedThingBase { public:    // 0x10 bytes, vtable 0x0127094C
    virtual void h000(); virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004(); virtual void h005(); virtual void h006(); virtual void h007();
    virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011();
    virtual CScriptThing GetScriptThing() const;   // slot 12 (+0x30) 0x007E7490
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34) 0x007E7390
    virtual void h014(); virtual void h015(); virtual void h016(); virtual void h017(); virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021();
    virtual void h022(); virtual void h023(); virtual void h024(); virtual void h025();
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68) 0x007E7450
    unsigned char _body[0xc];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
enum ESheriffState { SHERIFF_STATE_0, SHERIFF_STATE_1, SHERIFF_STATE_2, SHERIFF_STATE_3 };   // enumerator names not in the PDB dump
namespace NScript {
class CV_MurderTwistFinalScript {
public:
    unsigned char _pad00[0x48];
    ESheriffState  SheriffState;                       // +0x48
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing);   // 0x00EB0700
public:
    class CMurderTwistSheriff;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_MurderTwistFinalScript;
class CTimer { public: unsigned long _handle; };
namespace NScript {
class CV_MurderTwistFinalScript::CMurderTwistSheriff : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;            // +0x14
    void*                      PMasterData;            // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer                     AttractTimer;           // +0x2c
    unsigned long              NextLine;               // +0x30
    bool HasGivenReminderToHero;                       // +0x34
    bool HasBeingRumbledCutscenePlayed;                // +0x35
    bool HasExpressedSuspicionToHero;                  // +0x36
    bool HasSaidWellDoneToHero;                        // +0x37
private:
    void DoGivingRewardToHero();   // PDB ?DoGivingRewardToHero@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ (private)
public:
};
}

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoGivingRewardToHero()
{
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED", TEXT_GROUP_SELECTION_RANDOM, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyInline();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
            }
        }
        Game->GiveHeroGold(1000);
        ParentClass->SheriffState = SHERIFF_STATE_3;
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
        || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->WaitForEnflameReactionIfNecessary(Me);
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_ASSASSIN_KILLED_HERO_SUSSED", TEXT_GROUP_SELECTION_RANDOM, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
            }
        }
        ParentClass->SheriffState = SHERIFF_STATE_2;
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
}