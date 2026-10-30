#ifndef FABLE_ENGINE_CTIMER_H
#define FABLE_ENGINE_CTIMER_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): a 4-byte non-polymorphic handle (Ego_r.pdb CTimer
// {long TimerIndex}; ??1CTimer@@QAE@XZ is non-virtual). Ctor 0x00CD4450 = TimerIndex = interface slot 87 RegisterTimer();
// dtor 0x00CD4470 = interface slot 88 DeregisterTimer(TimerIndex) (both through the global interface pointer 0x0143E8F8).
// The operators are the Ego_r.pdb set; retail expands operator= inline (slot 89 SetTimer): a row that needs the inline body
// defines it out of class after the include.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CTimer.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"

class CTimer {
public:
    CTimer();                                                            // 0x00CD4450
    ~CTimer();                                                           // 0x00CD4470
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
    long TimerIndex;                                                     // +0x00
};
FABLE_STATIC_ASSERT(sizeof(CTimer) == 0x4);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CTIMER_H
