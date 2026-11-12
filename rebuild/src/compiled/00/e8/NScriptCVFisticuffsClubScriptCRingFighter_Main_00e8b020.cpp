// CRingFighter::Main() -- punch-club ring fighter: filter on Me alive while the crowd is not all attacking; wait for the round to start and target the hero; each frame let the crowd cheer when the hero lands a hit and boo when the ring fighter hits the hero, stop on a time-out; once Me's health falls to the def's losing threshold, take scripted control of Me and the hero, play the ROUNDWON cutscene, walk Me to the loser marker, flag the ring cleared and wait for the next round, then walk Me out to the marker and remove Me
// Leaf: ?Main@CRingFighter@CV_FisticuffsClubScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 27 (family main, DECISIONS_W26 D1): Ego_r.pdb / FableWin.pdb nest the entity in its script class and spell
//   Main a private virtual (?Main@CRingFighter@CV_FisticuffsClubScript@NScript@@EAEXXZ); the local CEntityScriptBase carries the retail slot list of vtable 0x012C3224
//   (engine/CEntityScriptBase.h). Was: ?Main@CRingFighter@@UAEXXZ over a global CV_FisticuffsClubScript (GetCutsceneName outside NScript).
// Naming wave 26 (family pred): CEntityScriptBase is the global PDB class (Ego_r.pdb / FableWin.pdb) with
//   ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z 0x00F35B10 and the const
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30; the predicate parameter is an NScript::CPredicateBase.
//   Was: SetPredicate(const CScriptPredicate&) over a global CScriptPredicate { Evaluate } stand-in of the predicate base.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B020, 1748 bytes (int3 padded to 1760).
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840, CIsThingAlive (vtable 0x012C32F0, ~ 0x00E241B0), flag predicate
//   (vtable 0x012EA43C), and-predicate (vtable 0x012EA430, ctor 0x00E7E020), predicate base vtable 0x012C2FF4,
//   SetPredicate 0x00F35B10, IsDone 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
//   CBaseObject 0x0099A380, scripted-thing handle ctor 0x007E72A0 / ~ 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770
//   / IsPerformingScriptTask 0x007E7450 / MoveToThing 0x007E7300 / MoveToPosition 0x007E72F0 / GetScriptThing 0x007E7490,
//   cast map ctor 0x00CDBF70 / ~ 0x00CDBFB0 / operator[] 0x00CD3D2E, handle operator= 0x008ABD10, movie ctor 0x006E7B60 / ~
//   0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CV_FisticuffsClubScript::GetCutsceneName 0x00E8A1D0, RunCutsceneMacro
//   0x00CBFB7D, IsDistanceFromThingToPositionOver 0x00CBE45C; CScriptThing slots 21, 41, 42 (impl slot 6 GetPos inline);
//   pGlobalScriptDef 0x0143E90C (+0x5E0 losing health), zero vector 0x0143E8E0; Game slots 7, 8, 70, 72, 108, 264, 370, 379, 451, 452.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}; Ego_r.pdb CRingFighter ParentClass +0x14; parent
//   CV_FisticuffsClubScript (PDB - 0x14) TylerInFight +0x54, TimeOut +0x56, RingCleared +0x57, RoundStarted +0x58,
//   EverybodyAttack +0x8c, CheerNumber +0xac, BooNumber +0xb0.
// Name fix (agentNF24, land1011de): the local CIsThingAlive / CIsFlagFalse / CAndPredicate models replaced by the shared NScript
//   predicate headers: 0x012EA43C binds ??_7CIsVariableFalse@NScript@@6B@ (was ??_7CIsFlagFalse@@6B@), 0x012EA430 / 0x00E7E020 the
//   CPredicate_And<CIsThingAlive,CIsVariableFalse> vtable / ctor (was CAndPredicate).
#include "fable_compiler.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
// NScript predicates: the shared models (agentNF24, land1011de): CIsThingAlive (vtable 0x012C32F0, ~ 0x00E241B0; its CScriptThing ctor
// is inline in retail), CIsVariableFalse (vtable 0x012EA43C, the const bool& at +4) and their And (vtable 0x012EA430, ctor 0x00E7E020,
// destroyed inline: opt-in FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR) over CPredicateBase (vtable 0x012C2FF4).
#define FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR
#include "engine/CIsThingAlive.h"
#include "engine/CIsVariableFalse.h"
#include "engine/CPredicate_And.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
using NScript::CIsThingAlive;
using NScript::CIsVariableFalse;
using NScript::CPredicate_And;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
enum EScriptEntityMoveType { MOVE_TYPE_WALK };
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x5e0];
    long FisticuffsLosingHealth;                        // +0x5e0
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
class CV_FisticuffsClubScript { public:                 // Ego_r.pdb / FableWin.pdb NScript::CV_FisticuffsClubScript
    struct CRingFighter;                                // nested entity script
    unsigned char _pad[0x54];
    bool TylerInFight;                                  // +0x54
    bool HeroBeatTyler;                                 // +0x55
    bool TimeOut;                                       // +0x56
    bool RingCleared;                                   // +0x57
    bool RoundStarted;                                  // +0x58
    unsigned char _pad59[0x8c - 0x59];
    bool EverybodyAttack;                               // +0x8c
    unsigned char _pad8d[0xac - 0x8d];
    float CheerNumber;                                  // +0xac
    float BooNumber;                                    // +0xb0
    CCharString GetCutsceneName(CCharString name);      // 0x00E8A1D0
};
}
using NScript::CV_FisticuffsClubScript;
class CScriptBase; class CPersistContext;
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();                       // slot 0  0x00F35B40
    virtual void Main() = 0;                            // slot 1 (vtable 0x012C3224, engine/CEntityScriptBase.h)
    virtual void Init();                                // slot 2  0x00CDEBB0
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext&);           // slot 4  0x00CDEBC0
    virtual void OnPredicateFail();                     // slot 5  0x00CDEBD0
    virtual void OnInterrupted();                       // slot 6  0x00CDEBE0
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
};
struct CV_FisticuffsClubScript::CRingFighter : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    void*                    PMasterData;               // +0x18
    bool                     FirstRingFighterFought;    // +0x1c
private:
    virtual void Main();                                // private virtual (Ego_r / FableWin EAEXXZ)
};

void CV_FisticuffsClubScript::CRingFighter::Main()
{
    {
        CIsVariableFalse notAttacking(ParentClass->EverybodyAttack);
        CIsThingAlive alive(Me);
        CPredicate_And<CIsThingAlive, CIsVariableFalse> both(alive, notAttacking);
        SetPredicate(both);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase fighter;
    while (!ParentClass->RoundStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!IsActiveThreadTerminating()) {
        bool hit = Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"));
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CheerNumber += 0.25f;
        }
        if (Game->GetHero().MsgIsHitBy("KHGRingFighter")) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->BooNumber += 4.0f;
        }
        if (ParentClass->TimeOut) {
            if (IsActiveThreadTerminating())
                return;
            Game->ClearThingBestEnemyTarget(Me);
            return;
        }
        const CScriptDef* def = pGlobalScriptDef;
        if (Game->GetHealth(Me) <= def->FisticuffsLosingHealth) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing loserPos = Game->GetThingWithScriptName("LoserFighterPos");
            ParentClass->RoundStarted = false;
            {
                CScriptGameResourceObjectScriptedThingBase hero;
                if (fighter.IsValid())
                    fighter.Release();
                while (!Game->StartScriptingEntity(Me, fighter, (EScriptAIPriority)4)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
                CCutsceneCastMap cast;
                cast["FIGHTER"] = fighter;
                cast["HERO"] = hero;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro(ParentClass->GetCutsceneName("ROUNDWON"), cast, 0, 0, false, true);
                while (fighter.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                fighter.MoveToThing(loserPos, 1.0f, MOVE_TYPE_WALK, false, true, false, true);
                ParentClass->RingCleared = true;
                while (!ParentClass->RoundStarted && !ParentClass->TylerInFight) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                hero.Release();
                game->PauseAllNonScriptedEntities(false);
            }
            {
                C3DVector pos = loserPos.GetPos();
                while (IsDistanceFromThingToPositionOver(fighter.GetScriptThing(), pos, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    fighter.MoveToPosition(pos, 0.0f, MOVE_TYPE_WALK, false, true);
                    while (fighter.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
            return;
        }
        Game->NewScriptFrame();
    }
}