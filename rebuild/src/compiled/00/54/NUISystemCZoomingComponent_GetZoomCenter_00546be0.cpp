// NUISystem::CZoomingComponent::GetZoomCenter() -- returns ZoomCenter
// Leaf: ?GetZoomCenter@CZoomingComponent@NUISystem@@UAE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546BE0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 146 of NUISystem::CZoomingComponent's vtable 0x01247DC4 (the slot holds this body in 1 vtable(s), owner NUISystem::CZoomingComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 67 TLC-named slots of that vtable puts GetZoomCenter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetZoomCenter@CZoomingComponent@NUISystem@@UAE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: NUISystem::CZoomingComponent::vfunc_146 (rebuild/src/compiled/00/54/NUISystemCZoomingComponent_vfunc146_00546be0.cpp).
#define FABLE_RETAIL_CZOOMINGCOMPONENT_VSLOTS
#define FABLE_RETAIL_CZOOMINGCOMPONENT_MEMBERS
#include "engine/NUISystem_CZoomingComponent.h"
namespace NUISystem {
C2DVector CZoomingComponent::GetZoomCenter()
{
    return ZoomCenter;
}
}
