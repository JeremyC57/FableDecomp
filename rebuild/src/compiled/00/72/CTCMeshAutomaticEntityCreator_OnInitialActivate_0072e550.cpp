// CTCMeshAutomaticEntityCreator::OnInitialActivate() -- on initial activation: CreateChildThings
// Leaf: ?OnInitialActivate@CTCMeshAutomaticEntityCreator@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072E550, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of CTCMeshAutomaticEntityCreator's vtable 0x0123A54C (the slot holds this body in 1 vtable(s), owner CTCMeshAutomaticEntityCreator); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCMeshAutomaticEntityCreator@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCMeshAutomaticEntityCreator::vfunc_6 (rebuild/src/compiled/00/72/gapscan_sub0072e550_0072e550.cpp).
// No shared header for CTCMeshAutomaticEntityCreator: minimal local definition (RTTI base CTCBase), the override and the CreateChildThings it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCMeshAutomaticEntityCreator : public CTCBase {
public:
    virtual void OnInitialActivate();
private:
    void CreateChildThings();
};
void CTCMeshAutomaticEntityCreator::OnInitialActivate()
{
    CreateChildThings();
}
