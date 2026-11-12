#ifndef FABLE_ENGINE_CSTOPWATCH_H
#define FABLE_ENGINE_CSTOPWATCH_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CStopWatch (0x20 bytes): m_fTimerPeriod +0x00,
// m_nStartTick +0x08, m_nPrevElapsedTicks +0x10, m_bIsRunning +0x18. Out of line in retail: StartZero 0x0062F800, Stop 0x0062F850,
// Reset 0x0062F8B0, GetElapsedSeconds 0x0062F900 (Ego_r / FableWin QAE / QBE spellings). IsRunning (FableWin.pdb
// ?IsRunning@CStopWatch@@QBE_NXZ) is inline in retail (Run 0x0042F07F reads +0x18 in place).
#include "rebuild_abi.h"
class CStopWatch {
public:
    void Start();
    void StartZero();                                                         // 0x0062F800
    void Stop();                                                              // 0x0062F850
    void Reset();                                                             // 0x0062F8B0
    float GetElapsedSeconds() const;                                          // 0x0062F900
    bool IsRunning() const { return m_bIsRunning; }
private:
    float m_fTimerPeriod;                                                     // +0x00
    __int64 m_nStartTick;                                                     // +0x08
    __int64 m_nPrevElapsedTicks;                                              // +0x10
    bool m_bIsRunning;                                                        // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CStopWatch) == 0x20);
#endif
