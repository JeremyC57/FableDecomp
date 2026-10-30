// CRingFighter::Main() -- punch-club ring fighter: filter on Me alive while the crowd is not all attacking; wait for the round to start and target the hero; each frame let the crowd cheer when the hero lands a hit and boo when the ring fighter hits the hero, stop on a time-out; once Me's health falls to the def's losing threshold, take scripted control of Me and the hero, play the ROUNDWON cutscene, walk Me to the loser marker, flag the ring cleared and wait for the next round, then walk Me out to the marker and remove Me
// Leaf: ?Main@CRingFighter@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B020, 1748 bytes (int3 padded to 1760).
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840, CIsThingAlive (vtable 0x012C32F0, ~ 0x00E241B0), flag predicate
//   (vtable 0x012EA43C), and-predicate (vtable 0x012EA430, ctor 0x00E7E020), predicate base vtable 0x012C2FF4,
//   SetPredicate 0x00F35B10, IsDone 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
//   CBaseObject 0x0099A380, scripted-thing handle ctor 0x007E72A0 / ~ 0x007E74D0 / IsScripting 0x00CD23B9 / Release 0x00CD2770
//   / IsPerformingScriptTask 0x007E7450 / MoveToThing 0x007E7300 / MoveToPosition 0x007E72F0 / GetScriptThing 0x007E7490,
//   cast map ctor 0x00CDBF70 / ~ 0x00CDBFB0 / operator[] 0x00CD3D2E, handle operator= 0x008ABD10, movie ctor 0x006E7B60 / ~
//   0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CV_FisticuffsClubScript::GetCutsceneName 0x00E8A1D0, RunCutsceneMacro
//   0x00CBFB7D, IsDistanceFromThingToPositionOver 0x00CBE45C; CScriptThing slots 21, 41, 42 (impl slot 6 GetPos inline);
//   pGlobalScriptDef 0x0143E90C (+0x5E0 losing health), zero vector 0x0143E8E0; Game slots 7, 8, 70, 72, 108, 264, 370, 379, 451, 452.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}; Ego_r.pdb CRingFighter ParentClass +0x14; parent
//   CV_FisticuffsClubScript (PDB - 0x14) TylerInFight +0x54, TimeOut +0x56, RingCleared +0x57, RoundStarted +0x58,
//   EverybodyAttack +0x8c, CheerNumber +0xac, BooNumber +0xb0.
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
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;            // imp slot 6 (+0x18)
};
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    __forceinline virtual const C3DVector& GetPos() const   // slot 6 (+0x18) 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual bool MsgIsHitBy(const CCharString& by) const;   // slot 21 (+0x54)
    virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& by) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& by) const;              // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
struct CScriptPredicate {                               // vtable 0x012C2FF4
    virtual bool Evaluate() const = 0;
    virtual CScriptPredicate* Clone() const = 0;
    virtual ~CScriptPredicate() {}
};
namespace NScript {
class CIsThingAlive : public CScriptPredicate { public:               // vtable 0x012C32F0
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                   // 0x00E241B0
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
}
using NScript::CIsThingAlive;
struct CIsFlagFalse : CScriptPredicate {                // vtable 0x012EA43C
    const bool* Flag;                                   // +0x04
    CIsFlagFalse(const bool* flag) : Flag(flag) {}
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
struct CAndPredicate : CScriptPredicate {               // vtable 0x012EA430
    CIsThingAlive First;                                // +0x04
    CIsFlagFalse  Second;                               // +0x14
    CAndPredicate(const CIsThingAlive& first, const CIsFlagFalse& second);   // 0x00E7E020
    ~CAndPredicate() {}
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
enum EScriptEntityMoveType { MOVE_TYPE_WALK };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    virtual bool IsPerformingScriptTask() const;        // 0x007E7450
    virtual CScriptThing GetScriptThing() const;        // 0x007E7490
    virtual void MoveToThing(const CScriptThing& thing, float distance, EScriptEntityMoveType type, bool a, bool b, bool c, bool d);   // 0x007E7300
    virtual void MoveToPosition(const C3DVector& pos, float distance, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CCountedPointerWords PImp;
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
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
struct CV_FisticuffsClubScript {
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
class CEntityScriptBase { public:
    virtual void Main() = 0;
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CScriptPredicate& predicate);   // 0x00F35B10
    bool IsDone();                                      // 0x00F35B30
};
struct CRingFighter : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    void*                    PMasterData;               // +0x18
    bool                     FirstRingFighterFought;    // +0x1c
    virtual void Main();
};

void CRingFighter::Main()
{
    {
        CIsFlagFalse notAttacking(&ParentClass->EverybodyAttack);
        CIsThingAlive alive(Me);
        CAndPredicate both(alive, notAttacking);
        SetPredicate(both);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase fighter;
    while (!ParentClass->RoundStarted) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!IsDone()) {
        bool hit = Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
        if (hit) {
            if (IsDone())
                return;
            ParentClass->CheerNumber += 0.25f;
        }
        if (Game->GetHero().MsgIsHitBy("KHGRingFighter")) {
            if (IsDone())
                return;
            ParentClass->BooNumber += 4.0f;
        }
        if (ParentClass->TimeOut) {
            if (IsDone())
                return;
            Game->ClearThingBestEnemyTarget(Me);
            return;
        }
        const CScriptDef* def = pGlobalScriptDef;
        if (Game->GetHealth(Me) <= def->FisticuffsLosingHealth) {
            if (IsDone())
                return;
            CScriptThing loserPos = Game->GetThingWithScriptName("LoserFighterPos");
            ParentClass->RoundStarted = false;
            {
#pragma inline_depth(0)
                CScriptGameResourceObjectScriptedThingBase hero;
#pragma inline_depth()
                if (fighter.IsScripting())
                    fighter.Release();
                while (!Game->StartScriptingEntity(Me, fighter, (EScriptAIPriority)4)) {
                    Game->NewScriptFrame();
                    if (IsDone())
                        return;
                }
                if (IsDone())
                    return;
                Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
                CCutsceneCastMap cast;
                cast["FIGHTER"] = fighter;
                cast["HERO"] = hero;
#pragma inline_depth(0)
                CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro(ParentClass->GetCutsceneName("ROUNDWON"), cast, 0, 0, false, true);
                while (fighter.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsDone()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsDone()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                fighter.MoveToThing(loserPos, 1.0f, MOVE_TYPE_WALK, false, true, false, true);
                ParentClass->RingCleared = true;
                while (!ParentClass->RoundStarted && !ParentClass->TylerInFight) {
                    Game->NewScriptFrame();
                    if (IsDone()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsDone()) {
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
                    if (IsDone())
                        return;
                    fighter.MoveToPosition(pos, 0.0f, MOVE_TYPE_WALK, false, true);
                    while (fighter.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsDone())
                            return;
                    }
                    if (IsDone())
                        return;
                }
            }
            if (IsDone())
                return;
            Game->RemoveThing(Me, false, true);
            return;
        }
        Game->NewScriptFrame();
    }
}