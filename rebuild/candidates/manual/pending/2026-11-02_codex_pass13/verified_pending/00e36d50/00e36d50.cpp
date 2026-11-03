// NScript::CV_ArcheryCompetitionScript::CAC_DummyMoving::Main() -- move a scoring dummy between its two endpoints during an archery round.
// Leaf: ?Main@CAC_DummyMoving@CV_ArcheryCompetitionScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(1471v1471) -- genuine header equality expansion and original generated loop shape.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE_EXPANDED
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include <string.h>
#include "engine/C3DVector.h"
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;

#pragma auto_inline(off)
template<> long __fastcall CBasicString<char>::Compare(const char* first,const char* second)
{
    while (*first || *second) {
        if (*first < *second) return -1;
        if (*first > *second) return 1;
        ++first;
        ++second;
    }
    return 0;
}
#pragma auto_inline(on)

inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data) return NullCoord;
    return PImp.Data->GetPos();
}

void NScript::CV_ArcheryCompetitionScript::CAC_DummyMoving::Main()
{
    CCharString targetThingName=Me.GetDataString();
    CScriptThing targetThing=Game.GetThingWithScriptName(targetThingName);
    C3DVector startPos=Me.GetPos();
    C3DVector targetPos=targetThing.GetPos();
    CCharString dummyScriptName=Me.GetDataString();
    if (dummyScriptName=="M_AC_DummyTargetCentre") {
        if (IsActiveThreadTerminating()) return;
        PointsValue=pGlobalScriptDef->ArcheryScoreDummyCentre;
        Steps=pGlobalScriptDef->ArcheryDummySpeedFast;
    } else if (dummyScriptName=="M_AC_DummyTargetRight") {
        if (IsActiveThreadTerminating()) return;
        PointsValue=pGlobalScriptDef->ArcheryScoreDummyRight;
        Steps=pGlobalScriptDef->ArcheryDummySpeedMedium;
    } else if (dummyScriptName=="M_AC_DummyTargetLeft") {
        if (IsActiveThreadTerminating()) return;
        PointsValue=pGlobalScriptDef->ArcheryScoreDummyLeft;
        Steps=pGlobalScriptDef->ArcheryDummySpeedSlow;
    } else if (dummyScriptName=="M_AC_DummyTargetFront") {
        if (IsActiveThreadTerminating()) return;
        PointsValue=pGlobalScriptDef->ArcheryScoreDummyFront;
        Steps=pGlobalScriptDef->ArcheryDummySpeedMedium;
    } else {
        if (IsActiveThreadTerminating()) return;
    }
    bool reverse=false;
    C3DVector vector((targetPos.X-startPos.X)/Steps,(targetPos.Y-startPos.Y)/Steps,0.0f);
    C3DVector reversevector((startPos.X-targetPos.X)/Steps,(startPos.Y-targetPos.Y)/Steps,0.0f);
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    long count=0;
    float hitPower=0.0f;
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        while (ParentClass.ContestState!=CONTEST_STARTED) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass.ContestState!=CONTEST_FINISHED && ParentClass.ContestState!=CONTEST_WAITING) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (reverse) {
                if (IsActiveThreadTerminating()) return;
                Game.EntityTeleportToPosition(Me,targetPos+C3DVector(reversevector.X*(float)count,reversevector.Y*(float)count,0.0f),0.0f,false,false);
            } else {
                if (IsActiveThreadTerminating()) return;
                Game.EntityTeleportToPosition(Me,startPos+C3DVector(vector.X*(float)count,vector.Y*(float)count,0.0f),0.0f,false,false);
            }
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("AC_PlayerRing"),false);
            if (Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO",hitPower)) {
                if (IsActiveThreadTerminating()) return;
                Game.PlaySoundOnThing(Game.GetHero(),"SND_ARROWIMPACT_02");
                ParentClass.CalculateScoreForHit(PointsValue,hitPower);
                Game.EntityPlayObjectAnimation(Me,"GET_HIT",false);
            }
            ++count;
            if (count==Steps) {
                if (IsActiveThreadTerminating()) return;
                reverse=!reverse;
                count=0;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
}
