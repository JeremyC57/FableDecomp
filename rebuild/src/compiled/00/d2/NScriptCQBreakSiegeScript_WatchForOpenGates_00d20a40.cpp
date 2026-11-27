// 00d20a40 NScript::CQ_BreakSiegeScript::WatchForOpenGates
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_BREAK_SIEGE_TIMER_PASS29
// Leaf: ?WatchForOpenGates@CQ_BreakSiegeScript@NScript@@QAEXXZ
// Flags: /O2 /Oy /Gr
// Inline-budget: depth one on each forwarder preserves the calls at the NewBandit assignment
// and the complete 25-byte conversion and 74-byte assignment COMDATs.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
#include <stdlib.h>
#include "engine/fable_script_thing_predicates.h"
inline CScriptThing::CScriptThing(){}
inline CScriptThing::~CScriptThing(){}
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
FABLE_FORCEINLINE CDefString::operator CCharString() const { return GetString(); }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other)
{
    CBaseClass::operator=(other);
    PImp = other.PImp;
    return *this;
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

inline CTimer::CTimer(){TimerIndex=pDSTGame->RegisterTimer();}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer& CTimer::operator=(long value){pDSTGame->SetTimer(TimerIndex,value);return *this;}
inline long CTimer::GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
inline bool CTimer::operator==(long value)const{return GetTime()==value;}
void NScript::CQ_BreakSiegeScript::WatchForOpenGates()
{
    TRetVoid _Fail_Val;
    while (!BanditSiegeStart) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    std::vector<CScriptThing> BanditSpawns;
    Game.GetAllThingsWithScriptName("MB_BanditSpawn", BanditSpawns);
    CScriptThing NewBandit;
    CScriptThing EnteredCamp = Game.GetThingWithScriptName("MK_BS_INTRO_LEADER");
    if (!HasFirstWaveAttacked) {
        if (IsActiveThreadTerminating())
            {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        CTimer BanditStreamDelay;
        bool GotIntoCamp = false;
        if (false) {} else for (long i = 0; i < pGlobalScriptDef->BS_NoOfWavesInFirstAttack; ++i) {
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            long BanditStream = 0;
            while (BanditStream < pGlobalScriptDef->BS_NoOfBanditsInWave) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (BanditStreamDelay == 0) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                    NewBandit = Game.CreateCreatureNearby(pGlobalScriptDef->BS_BanditTypeInFirstAttack,
                        BanditSpawns[rand() % BanditSpawns.size()].GetPos(),
                        5.0f, "BanditFirstWave", false);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    Game.EntityAttachToScript(NewBandit, Game.GetActiveQuestName());
                    BanditStreamDelay = 2;
                    ++BanditStream;
                }
                if (IsDistanceBetweenThingsUnder(EnteredCamp, Game.GetHero(), 20.0f)) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
                    GotIntoCamp = true;
                    break;
                }
            }
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            std::vector<CScriptThing> Bandits;
            Game.GetAllThingsWithScriptName("BanditFirstWave", Bandits);
            while (!GotIntoCamp && !AreAllThingsInVectorDead(Bandits)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                if (IsDistanceBetweenThingsUnder(EnteredCamp, Game.GetHero(), 20.0f)) {
                    if (IsActiveThreadTerminating())
                        {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                        }
                    GotIntoCamp = true;
                }
            }
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            if (GotIntoCamp) {
                if (IsActiveThreadTerminating())
                    {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    break;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (IsActiveThreadTerminating())
            {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        HasFirstWaveAttacked = true;
    }
    std::vector<CScriptThing> BanditVector;
    Game.GetAllThingsWithScriptName("BanditFirstWave", BanditVector);
    if (false) {} else for (;;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (AreAllThingsInVectorDead(BanditVector) && Game.IsRegionLoaded("KnotholeGlade")) {
            if (IsActiveThreadTerminating())
                {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            StartSecondWave = true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}