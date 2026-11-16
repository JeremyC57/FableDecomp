// NUISystem::CComponent::GetCurrentState() -- the base component's current state is always its state 0 (created on first use)
// Leaf: ?GetCurrentState@CComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00533B30, 23 bytes (+9 int3 padding); vtable 0x0124608C slot 50.
// Identity: FableWin.pdb ?GetCurrentState@CComponent@NUISystem@@UAEPAVCUIState@2@XZ; the only override of slot 50,
//          CSprite 0x0052CF10, finds its own current-state index (+0x144) in the same map and returns the entry -- the
//          GetCurrentState shape -- while this base form indexes state 0.
// Callees: STLport map<unsigned long, CUIState>::operator[](const unsigned long&) @ 0x0042D5B1 (manifest
//          "CPersistTraits::Transfer" is a propagated label; the landed CSlider::GetValueFromProfile @ 0x0054BBB0 binds it
//          as operator[]).
// Layout: retail CComponent States (map, 12 bytes) +0x24, as the landed 0x0054BBB0 model; no engine/ header exists.
// Shape: the literal key is a frame temporary whose address is pushed before its zero store; the reference result is
//          returned as the pointer.
// NF44 (round 6): the local C2DVector / CUIState / state-map stand-ins replaced by the shared engine/C2DVector.h,
// engine/NUISystem_CUIState.h and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP + _INDEX_DECL_ONLY: operator[] out of line), so the
// subscript binds STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1 (the landed row spelling).
#include "engine/C2DVector.h"
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"
typedef std::map<unsigned long, NUISystem::CUIState> CStateMap;   // STLport, 12 bytes

namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual CUIState* GetCurrentState();           // slot 50 in retail
    unsigned char _pad04[0x24 - 0x04];
    CStateMap     States;                          // +0x24
};

CUIState* CComponent::GetCurrentState()
{
    return &States[0];
}
}