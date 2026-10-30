// CMurderTwistSheriffGuard::DoSayingThankYouToHero() -- a sheriff's guard after the assassin is dealt with: talked to by the hero
//   -> paused movie sequence, a cycling per-guard "grateful" line ("..._SHERIFF" + Name + NextLine 1..2) while alive, NextLine
//   advanced; hit by the hero (or by a special ability other than 14) without having been enflamed -> paused "attacked" line,
//   SheriffState 2
// Leaf: DoSayingThankYouToHero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3740, 1108 bytes (`ret`).
// Twin: CMurderTwistAssassin::DoRemindingHeroToKillSheriff 0x00EB4F80 (this chunk) and the landed CMurderTwistSheriff
//   ::DoRemindingHeroToKillSheriff 0x00EB15C0; same model.
// Callees: as 0x00EB4F80, plus operator+(const CCharString&, const CCharString&) 0x0099F570 and
//   CV_MurderTwistFinalScript::HasHeroEnflamedEntity 0x00EB07D0.
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "TEXT_QST_B07_GUARD_GRATEFUL_SHERIFF" 0x012F0098,
//   "TEXT_QST_B07_GUARD_GRATEFUL_SHERIFF_ATTACKED" 0x012F0068.
// Layout: PDB CMurderTwistSheriffGuard: ParentClass +0x14, MyEntityHandle +0x1c, NextLine +0x2c, ID +0x30, Name +0x34.
// Shape: as 0x00EB4F80: the line text lives in an inner block opened right after the movie declaration and left by goto on
//   the terminate exits; the hit test ANDs !HasHeroEnflamedEntity onto the landed hit condition.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690 (hidden result in ecx)
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570 (hidden result in ecx)
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
enum EAssassinState { ASSASSIN_STATE_0, ASSASSIN_STATE_1, ASSASSIN_STATE_2, ASSASSIN_STATE_3 };
struct CV_MurderTwistFinalScript {                     // retail sits 0x14 below the PDB (SheriffState PDB +0x5c)
    unsigned char _pad00[0x48];
    ESheriffState  SheriffState;                       // +0x48
    EAssassinState AssassinState;                      // +0x4c
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing);   // 0x00EB0700
    bool HasHeroEnflamedEntity(const CScriptThing& thing);               // 0x00EB07D0
};
struct CMurderTwistSheriffGuard : CEntityScriptBase {
    CV_MurderTwistFinalScript* ParentClass;            // +0x14
    void*                      PMasterData;            // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long              NextLine;               // +0x2c
    unsigned long              ID;                     // +0x30
    CCharString                Name;                   // +0x34
    void DoSayingThankYouToHero();
};

void CMurderTwistSheriffGuard::DoSayingThankYouToHero()
{
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        CGameScriptInterface* game;
        {
            Game->StartMovieSequence("", movie);
            game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (NextLine > 2)
                NextLine = 1;
            CCharString text = "TEXT_QST_B07_GUARD_GRATEFUL_SHERIFF" + Name + GFIntToCharString(NextLine);
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), text, TEXT_GROUP_SELECTION_RANDOM, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        goto terminatedInLoop;
                }
                if (IsActiveThreadTerminating())
                    goto terminatedAfterLoop;
            }
            NextLine++;
        }
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
        if (0) {
terminatedInLoop:
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
            return;
terminatedAfterLoop:
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyOutOfLine();
            return;
        }
    }
    if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")))
        && !ParentClass->HasHeroEnflamedEntity(Me)) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_GUARD_GRATEFUL_SHERIFF_ATTACKED" + Name, TEXT_GROUP_SELECTION_RANDOM, false, true, false);
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