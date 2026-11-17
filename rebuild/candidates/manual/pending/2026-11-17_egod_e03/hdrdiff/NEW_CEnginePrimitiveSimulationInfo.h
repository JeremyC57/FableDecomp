#ifndef FABLE_ENGINE_CENGINEPRIMITIVESIMULATIONINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVESIMULATIONINFO_H
// CEnginePrimitiveSimulationInfo (egod e03, row 0x00B81820): Ego_r.pdb layout (struct_layouts_egor.tsv: two words, then
// Simulations CArray<CCountedPointer<CEngineSimulationBase> > at +8; held through CIVCountedPointer, so the two words are the
// CIVCountedPointeeBase vptr / IVRefCount; scalar deleting destructor in Ego_d) and ?RunSimulation@CEnginePrimitiveSimulationInfo@@QAEXXZ
// (Ego_r / FableWin; retail out-of-line copy 0x00B80B40, expanded by CEngineSimulationManager::FrameStart; its body is the row's,
// Ego_d locals itr / sim). Declarations only.
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CEngineSimulationBase.h"
class CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase {
public:
    void RunSimulation();                                                // 0x00B80B40
    CArray<CCountedPointer<CEngineSimulationBase> > Simulations;         // +0x08
};
#endif
