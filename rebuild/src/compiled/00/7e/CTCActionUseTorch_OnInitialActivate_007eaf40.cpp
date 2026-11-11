// CTCActionUseTorch::OnInitialActivate() -- the torch action use removes its targeted component (TCI_TARGETED) from the parent thing when activated
// Leaf: ?OnInitialActivate@CTCActionUseTorch@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EAF40, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCActionUseTorch's vtable 0x0123F124 (the slot holds this body in 1 vtable(s), owner CTCActionUseTorch); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCActionUseTorch@@UAEXXZ; `ret` matches the signature.
// Was: CTCActionUseTorch::vfunc_6 (rebuild/src/compiled/00/7e/CTCActionUseTorch_vfunc6_007eaf40.cpp).
// No shared header for CTCActionUseTorch: minimal local definition on its RTTI base CTCActionUseBase.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"
class CTCActionUseTorch : public CTCActionUseBase {
public:
    virtual void OnInitialActivate();                        // slot 6
};
void CTCActionUseTorch::OnInitialActivate()
{
    ParentThing->RemoveTC(TCI_TARGETED);
}
