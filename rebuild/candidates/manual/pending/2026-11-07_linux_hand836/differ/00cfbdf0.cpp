// NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main() -- a Necropolis grave ghost in Awakening the Oracle: live while alive; take the next grave number and the next free digging point, teleport to its GhostPos marker (marked SpotTaken); then, each round, while the digging spot is enabled flash Ghost_Appear_01 every 8 s and, every 16 s when the hero is within 15 m and the grave number is below 13, add a ghost conversation line TEXT_QST_B03_GHOST_(MALE_)NEAR_GRAVE<n>; once it is dug, flash again and move to the first digging point whose GhostPos is not taken (new grave number, face the point) or remove itself
// Grade: DIFFER(2278v2317) -- 303 masked difflines, one cause: the chunk 822 nested predicate-dtor case. Retail expands ~CIsThingAlive (filter vtable hoisted in ebx, stored by the ctor and the dtor) and calls the nested ~CScriptThing 0x004AA840; VC7.1 with the row-local inline ~CScriptThing (needed: retail expands ~CScriptThing at the DiggingPoint / GhostStandsHere / GetNearestWithScriptName temporaries) declines the whole outer dtor (calls 0x00E241B0-style COMDAT), so ebx is free and every later register / frame choice shifts (tail pads short). Diagnostic meter only: /d2inlT60 brings it to 54 difflines (2306v2317).
// Next idea: none in plain C++ known; chunk 836 tried __forceinline ~CIsThingAlive (a2: both levels expand, md247; a row-local forceinline is not allowed anyway), definition order of the inline ~CScriptThing / ~CIsThingAlive after Main (a3/a4: identical), dropping the inline ~CScriptThing (a5: predicate right, the other ~CScriptThing sites become calls, 223 bytes short)
// Leaf: ?Main@CNecropolisGhost@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFBDF0, 2317 bytes, thiscall, plain `ret` (seven epilogue copies), frame 0x94.
// Identity (proven): Ego_r.pdb ?Main@CNecropolisGhost@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ; slot 1 of the CNecropolisGhost
//   entity vtable stored by Alloc 0x00CFBCE0.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CQ_AWAKENINGTHEORACLESCRIPT_CNECROPOLISGHOST
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);   // 0x00704580
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline void CScriptThing::SetDataString(const CCharString& text) { if (PImp.Data) PImp.Data->SetDataString(text); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
CCharString FABLE_FASTCALL GFIntToCharString(long);                                                              // 0x0099F830
CCharString FABLE_FASTCALL operator+(const char*, const CCharString&);                                           // 0x0099F690

void NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing DiggingPoint;
    ++ParentClass.GraveNumbersGivenOut;
    MyGraveNumber = ParentClass.GraveNumbersGivenOut;
    if (ParentClass.DiggingPoints.size() > (unsigned long)ParentClass.PositionTaken) {
        if (IsActiveThreadTerminating())
            return;
        DiggingPoint = ParentClass.DiggingPoints[ParentClass.PositionTaken];
        ++ParentClass.PositionTaken;
    }
    {
        CScriptThing GhostStandsHere = Game->GetNearestWithScriptName(DiggingPoint, "GhostPos");
        GhostStandsHere.SetDataString("SpotTaken");
        Game->EntityTeleportToThing(Me, GhostStandsHere, false);
    }
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        CTimer DiggingFlash;
        CTimer ConvDelay;
        while (Game->IsDiggingSpotEnabled(DiggingPoint)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (DiggingFlash.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateEffect("Ghost_Appear_01", Me.GetPos(), "", 0.0f, false, false);
                DiggingFlash = 8;
            }
            if (ConvDelay.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f) && MyGraveNumber < 13) {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateEffect("Ghost_Appear_01", Me.GetPos(), "", 0.0f, false, false);
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                if (Game->EntityGetSex(Me) == 1) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->AddLineToConversation(conversation, "TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE" + GFIntToCharString(MyGraveNumber), false, Me, Game->GetHero());
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->AddLineToConversation(conversation, "TEXT_QST_B03_GHOST_NEAR_GRAVE" + GFIntToCharString(MyGraveNumber), false, Me, Game->GetHero());
                }
                ConvDelay = 16;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->CreateEffect("Ghost_Appear_01", Me.GetPos(), "", 0.0f, false, false);
        if (ParentClass.DiggingPoints.size() != 0) {
            if (IsActiveThreadTerminating())
                return;
            for (unsigned long i = 0; i < ParentClass.DiggingPoints.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                DiggingPoint = ParentClass.DiggingPoints[i];
                if (Game->GetNearestWithScriptName(DiggingPoint, "GhostPos").GetDataString() != "SpotTaken") {
                    if (IsActiveThreadTerminating())
                        return;
                    break;
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing GhostStandsHere = Game->GetNearestWithScriptName(DiggingPoint, "GhostPos");
            if (GhostStandsHere.GetDataString() != "SpotTaken") {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityTeleportToThing(Me, GhostStandsHere, false);
                GhostStandsHere.SetDataString("SpotTaken");
                ++ParentClass.GraveNumbersGivenOut;
                MyGraveNumber = ParentClass.GraveNumbersGivenOut;
                Game->EntitySetFacingAngleTowardsThing(Me, DiggingPoint, false);
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveThing(Me, false, true);
            }
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Me, false, true);
        }
    }
}
