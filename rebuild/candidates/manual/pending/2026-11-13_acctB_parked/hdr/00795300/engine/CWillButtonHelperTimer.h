#ifndef FABLE_ENGINE_CWILLBUTTONHELPERTIMER_H
#define FABLE_ENGINE_CWILLBUTTONHELPERTIMER_H
#include "engine/CIntelligentPointer.h"
#include "engine/EHeroAbility.h"
#include "stlport/fable_containers.h"
class CThing;
// Layout: original Ego_r complete TPI2ed53; native008edff0 initializes all
// thirteen members in order; retail12-byte vector yields object size34.
class CWillButtonHelperTimer {
public:
    enum EButtonHeldState { BEING_HELD_DOWN,JUST_RELEASED_DELAYED,JUST_RELEASED,RELEASED };
    CWillButtonHelperTimer(EHeroAbility,CThing&);
    ~CWillButtonHelperTimer();
    void FrameUpdate();
#ifdef FABLE_RETAIL_CWILLBUTTONHELPERTIMER_GETSTATE_INLINE
    // Retail opt-in (row 0x00795300): inline in retail (`mov esi,[ebp+30h]`, Timer +0x14 read in place). Body only.
    EButtonHeldState GetCurrentButtonHeldState() const { return CurrentState; }
#else
    EButtonHeldState GetCurrentButtonHeldState() const;
#endif
    void SetCurrentButtonHeldState(EButtonHeldState);
    long GetButtonHeldDownForTime() const;
    float GetButtonHeldDownForTimeInSecs() const;
    long GetButtonHeldDownForTimeClamped() const;
    long GetButtonHeldDownForTimeClampedInSecs() const;
    long GetWillAccumulatedLevel() const;
    float GetWillAccumulatedLevelPercentage() const;
    bool GetWillIncrementedFlag() const;
    void SetUpdateAccumulatedLevel(bool);
    void ForceToNextLevel();
#ifdef FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETAICASTING
    // chunk 770 (row 0x0079F960 CTCWillLightning::SetAICasting): inline in retail (FableWin.pdb's debug build has
    // ?SetAICasting@CWillButtonHelperTimer@@QAEXXZ out of line; Ego_r.pdb and retail have no copy, the caller stores the flag).
    void SetAICasting() { IsAICasting = true; }
#else
    void SetAICasting();
#endif
    void SetAICasting(long);
    void SetAIHoldingDown(bool);
    bool QIsAIHoldingDown() const;
    bool QIsAICasting() const;
private:
    long ButtonHeldDownTimer;
    long WillAccumulatedLevel;
    CIntelligentPointer<CThing> PCreature;
    EHeroAbility MyAbility;
    EButtonHeldState CurrentState;
    std::vector<float> WillPowerUpAccumulatedTimings;
    float MaxNumberLevels;
    float CurMaxLevels;
    bool WillIncrementedFlag;
    bool UpdateAccLevel;
    bool IsAICasting;
    bool IsAIHoldingDown;
    long LevelToCast;
};
FABLE_STATIC_ASSERT(sizeof(CWillButtonHelperTimer)==0x34);
#endif
