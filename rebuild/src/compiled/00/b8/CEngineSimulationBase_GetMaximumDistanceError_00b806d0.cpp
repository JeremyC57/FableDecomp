// CEngineSimulationBase::GetMaximumDistanceError() -- the simulation manager's maximum distance error, shared by every simulation
// Leaf: ?GetMaximumDistanceError@CEngineSimulationBase@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B806D0, 9 bytes, float in st0. Slot 5 of the CEngineSimulationBase vtable
//   0x012A38FC (four more derived vtables reference it); Ego_r.pdb `?GetMaximumDistanceError@CEngineSimulationBase@@UBEMXZ`
//   (public virtual const) forwarding to the inline `?GetMaximumDistanceError@CEngineSimulationManager@@QBEMXZ`.
// Layout: the manager global 0x01436E68 (`g_pEngineSimulationManager`, the binding of the landed 0x00B806E0);
//   engine/CEngineSimulationManager.h carries the retail STLport offsets (MaxDistanceError +0x24, next to the
//   MaxIterations +0x20 of the landed twin GetMaximumIterations 0x00B806C0).
#define CEngineSimulationManager CEngineSimulationManagerDonor
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager

class CEngineSimulationManager : public CEngineSimulationManagerDonor {
public:
    float GetMaximumDistanceError() const { return MaxDistanceError; }
};

extern CEngineSimulationManager* g_pEngineSimulationManager;   // 0x01436E68

class CEngineSimulationBase {
public:
    virtual float GetMaximumDistanceError() const;
};

float CEngineSimulationBase::GetMaximumDistanceError() const
{
    return g_pEngineSimulationManager->GetMaximumDistanceError();
}