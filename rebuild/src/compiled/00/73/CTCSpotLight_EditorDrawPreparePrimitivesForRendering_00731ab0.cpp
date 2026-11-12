// CTCSpotLight::EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &) -- does nothing
// Leaf: ?EditorDrawPreparePrimitivesForRendering@CTCSpotLight@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00731AB0, thiscall, `ret 4`.
// Identity: slot 28 of CTCSpotLight's vtable 0x01266FBC (defined here: the slot holds this body in 1 vtable(s), all of CTCSpotLight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &) at this slot; Ego_r/FableWin name set: ?EditorDrawPreparePrimitivesForRendering@CTCSpotLight@@UAEXABVCEnginePrimitivePrepareInfo@@@Z; `ret` matches the signature.
// Header: engine/CTCSpotLight.h (opt-ins as in the landed row CTCSpotLight_FrameUpdate_00731bf0.cpp).
class CEnginePrimitivePrepareInfo;
#define FABLE_RETAIL_CTCSPOTLIGHT_VSLOTS
#include "engine/CTCSpotLight.h"
void CTCSpotLight::EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo const &)
{
}
