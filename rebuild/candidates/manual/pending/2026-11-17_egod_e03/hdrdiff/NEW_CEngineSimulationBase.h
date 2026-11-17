#ifndef FABLE_ENGINE_CENGINESIMULATIONBASE_H
#define FABLE_ENGINE_CENGINESIMULATIONBASE_H
// CEngineSimulationBase (egod e03, row 0x00B81820): Ego_d.pdb LF_CLASS 0x65CFD (field list 0x65CFC: base CParticleContext, overrides
// GetMaximumIterations / GetMaximumDistanceError, pure intro virtuals RunSimulation (vftable offset 24 = slot 6), IsRunning (28),
// CalcAndSetRunState (32, protected), RunState +4, SimStepOffset +8; sizeof 12). Retail vtable 0x012A38FC (rtti-vtables.tsv: slot 0
// 0x00B829D0, slot 4 GetMaximumIterations 0x00B806C0, slot 5 GetMaximumDistanceError 0x00B806D0; Ego_r ?..@CEngineSimulationBase@@UBEJXZ /
// UBEMXZ). Declarations only.
#include "rebuild_abi.h"
#include "engine/CParticleContext.h"
class CEngineSimulationBase : public CParticleContext {
public:
    enum ERunState { RUNSTATE_RUNNING = 0, RUNSTATE_STOPPED = 1, RUNSTATE_MAX = 2 };   // Ego_d LF_ENUM 0x65CE7 (field list 0x65CE6)
    virtual long GetMaximumIterations() const;                           // slot 4  0x00B806C0
    virtual float GetMaximumDistanceError() const;                       // slot 5  0x00B806D0
    virtual void RunSimulation() = 0;                                    // slot 6 (+0x18)
    virtual bool IsRunning() const = 0;                                  // slot 7
protected:
    virtual void CalcAndSetRunState() = 0;                               // slot 8
private:
    ERunState RunState;                                                  // +0x04
    unsigned char SimStepOffset;                                         // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEngineSimulationBase) == 12);
#endif
