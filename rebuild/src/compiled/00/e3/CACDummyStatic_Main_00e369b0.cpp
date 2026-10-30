// NScript::CV_ArcheryCompetitionScript::CAC_DummyStatic::Main() -- archery static dummy: remember the facing angle, then each contest round wait for the contest to start, wobble the dummy, wait for a hero arrow hit, score it, play the impact sound and spin, turn back and pause
// Identity (naming wave 11): ?Main@CAC_DummyStatic@CV_ArcheryCompetitionScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012E315C slot 1 is stored by the factory 0x00E33BC0 registered as "AC_DummyStatic"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E369B0, 752 bytes. Entity-script slot 1 (Main) of the V_ArcheryCompetition family.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CV_ArcheryCompetitionScript::CalculateScoreForHit
//   0x00E36CA0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; Me's CScriptThing slots 10 GetAngleXY
//   (+0x28) and 25 MsgIsHitByWithProjectileWeapon (+0x64); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero,
//   376 Pause, 474 EntitySetFacingAngle, 487 EntityPlayObjectAnimation, 690 PlaySoundOnThing (cgsi_slots_aligned.tsv).
//   Literals "SCRIPT_NAME_HERO" @ 0x0125D1C8, "SND_ARROWIMPACT_03" @ 0x012E346C, "GET_HIT_SPIN" @ 0x01288BD4; 0.25 @ 0x01238010,
//   0.5 @ 0x012316F0 (doubles).
// Layout: Ego_r.pdb CAC_DummyStatic (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//   PMasterData +0x18, PointsValue +0x1c. Retail CV_ArcheryCompetitionScript ContestState +0xa0 (PDB +0xb4: debug
//   containers; the landed WatchForQuestFinished 0x00E32F30 model). The quest-globals word at +0x37c of 0x0143E90C is
//   descriptive (no PDB type for that block).
// Shape: script-macro waits (NewScriptFrame then the terminating check) in nested top-tested loops; the string arguments
//   are plain literals converted implicitly to const CCharString&, which VC builds before any other argument is
//   evaluated (an explicit CCharString("...") temporary is built in argument order instead and loads the interface
//   vtable first); VC itself keeps angle + 0.25 in ebp for its second use.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x37c];
    long          ArcheryDummyStaticPoints;              // +0x37c (descriptive)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C
namespace NScript {
class CV_ArcheryCompetitionScript {
public:
    unsigned char _pad00[0xa0];
    long          ContestState;                          // +0xa0
    void CalculateScoreForHit(long points, float distance);   // 0x00E36CA0
public:
    class CAC_DummyStatic;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_ArcheryCompetitionScript;
struct CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_ArcheryCompetitionScript::CAC_DummyStatic : public CEntityScriptBase {
public:
        CV_ArcheryCompetitionScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;            // +0x18
    long                         PointsValue;            // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_ArcheryCompetitionScript::CAC_DummyStatic::Main()
{
    float angle = Me.GetAngleXY();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    PointsValue = pGlobalScriptDef->ArcheryDummyStaticPoints;
    float distance = 0.0f;
    while (!IsActiveThreadTerminating()) {
        while (ParentClass->ContestState != 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (ParentClass->ContestState != 2 && ParentClass->ContestState != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle + 0.25, true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle, true);
            while (!Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", distance)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CalculateScoreForHit(PointsValue, distance);
            Game->PlaySoundOnThing(Game->GetHero(), "SND_ARROWIMPACT_03");
            Game->EntityPlayObjectAnimation(Me, "GET_HIT_SPIN", false);
            Game->EntitySetFacingAngle(Me, angle + 0.25, true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngle(Me, angle + 0.5, true);
            Game->Pause((float)PointsValue);
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}