#ifndef FABLE_ENGINE_CTIMER_H
#define FABLE_ENGINE_CTIMER_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): a 4-byte non-polymorphic handle (Ego_r.pdb CTimer
// {long TimerIndex}; ??1CTimer@@QAE@XZ is non-virtual). Ctor 0x00CD4450 = TimerIndex = interface slot 87 RegisterTimer();
// dtor 0x00CD4470 = interface slot 88 DeregisterTimer(TimerIndex) (both through the global interface pointer 0x0143E8F8).
// The operators are the Ego_r.pdb set; retail expands operator= inline (slot 89 SetTimer): a row that needs the inline body
// defines it out of class after the include.
// Wave 8 (family TIMER, 185 rows): rows whose retail expands the ctor (RegisterTimer), the dtor (DeregisterTimer),
// GetTime (GetTimer) or operator= (SetTimer) define exactly those inline after the include; the invented setters
// SetTime / Set are this operator= (Ego_r.pdb ??4CTimer@@QAEAAV0@J@Z; no SetTime / Set in either PDB).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CTimer.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"

#ifdef FABLE_RETAIL_BREAK_SIEGE_TIMER_PASS29
class CGameScriptInterfaceBase;
extern CGameScriptInterfaceBase* pDSTGame; // Ego_r and FableWin S_PUB32; native pointer 0143e8f8.
#endif

#ifdef FABLE_RETAIL_ORCHARD_FARMER_INLINE_VIRTUAL
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;
#endif

class CTimer {
public:
    CTimer();                                                            // 0x00CD4450
#ifdef FABLE_RETAIL_ORCHARD_FARMER_INLINE_VIRTUAL
    ~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }                     // 0x00CD4470
#else
    ~CTimer();                                                           // 0x00CD4470
#endif
    CTimer& operator=(long);
    bool operator==(long) const;
    bool operator!=(long) const;
    bool operator<(long) const;
    bool operator>(long) const;
    bool operator<=(long) const;
    bool operator>=(long) const;
    long GetTime() const;
    long GetIndex() const;
    void Pause();
    void Unpause();
#ifdef FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
private:
#endif
    long TimerIndex;                                                     // +0x00
};
FABLE_STATIC_ASSERT_AT(36, sizeof(CTimer) == 0x4);

#ifdef FABLE_RETAIL_CQ_HOBBETOOTHCONTESTSCRIPT_ORCHARD_FARMER
#include "fable_compiler.h"
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;

inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
#ifndef FABLE_RETAIL_ORCHARD_FARMER_INLINE_VIRTUAL
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#endif
// Inline-budget: both original assignments expand the SetTimer call in the farmer.
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) {
    pDSTGame->SetTimer(TimerIndex, time);
    return *this;
}
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#endif

#endif // FABLE_RETAIL_NSCRIPT
#ifdef FABLE_RETAIL_HANGING_TIMER_INLINES_PASS30
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer& CTimer::operator=(long time)
{
    pDSTGame->SetTimer(TimerIndex, time);
    return *this;
}
#endif
#ifdef FABLE_RETAIL_SCYTHE_INFO_PASS30
#include "engine/CGameScriptInterfaceBase.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long seconds) const { return GetTime() == seconds; }
#endif
#endif // FABLE_ENGINE_CTIMER_H
