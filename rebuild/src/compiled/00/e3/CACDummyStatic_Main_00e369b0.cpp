// CAC_DummyStatic::Main() -- archery static dummy: remember the facing angle, then each contest round wait for the contest to start, wobble the dummy, wait for a hero arrow hit, score it, play the impact sound and spin, turn back and pause
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
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual float GetAngleXY() const;                    // slot 10 (+0x28) 0x004AAA40
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24();
    virtual bool MsgIsHitByWithProjectileWeapon(const CCharString& name, float& distance) const;   // slot 25 (+0x64) 0x004AABB0
    void* PImpData;
    void* PImpInfo;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x37c];
    long          ArcheryDummyStaticPoints;              // +0x37c (descriptive)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C
struct CV_ArcheryCompetitionScript {
    unsigned char _pad00[0xa0];
    long          ContestState;                          // +0xa0
    void CalculateScoreForHit(long points, float distance);   // 0x00E36CA0
};
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CAC_DummyStatic : public CEntityScriptBase {
public:
    void*                        __vftable;              // +0x00
    CGameScriptInterface*        Game;                   // +0x04
    CScriptThing                 Me;                     // +0x08
    CV_ArcheryCompetitionScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;            // +0x18
    long                         PointsValue;            // +0x1c
    void Main();
};

void CAC_DummyStatic::Main()
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