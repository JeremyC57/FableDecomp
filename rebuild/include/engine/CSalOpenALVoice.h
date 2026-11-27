#ifdef FABLE_RETAIL_CSALOPENALVOICE_ENVELOPE_PASS29
#ifndef FABLE_ENGINE_CSALOPENALVOICE_H
#define FABLE_ENGINE_CSALOPENALVOICE_H
#include "engine/CSalBaseVoice.h"
#include "engine/CodaCaps.h"
// SDK debug TPI 1577 and native member accesses in the envelope state machine.
class CSalOpenALVoice : public CSalBaseVoice {
protected:
    unsigned int m_uiVoice;
    unsigned int m_nQueueAdd;
    unsigned int m_nQueued;
private:
    CREFTIME m_rtLevelEnvEnd, m_rtLevelEnvDuration;
    CREFTIME m_rtPitchLFODelayEnd, m_rtPitchLFOCycleLength;
    CREFTIME TimeToRefTime(float time) const {return (CREFTIME)(g_SalCaps.RefTimeTicksPerSec*time);}
    void CalcLevelEnvAttack(CREFTIME,float);
    void CalcLevelEnvDecay(CREFTIME,float);
    bool CalcLevelEnvelope(const _CLEVELENVELOPE*,bool,CREFTIME);
};
#endif
#endif
