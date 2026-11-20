// CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveLight& light) -- queue a point light for this frame: skipped when its radius is below 0.1 or its colour is black (every channel below 1/255); otherwise a CManagerLocalLight from the frame heap is pushed onto LocalLightList
// Leaf: ?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveLight@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47FD0, length 260 bytes (`ret 4`; called from RenderPrimitive 0x00B481E0 for type 15).
// Identity: Ego_r.pdb 0x009BD400 / FableWin.pdb 0x02CEFE10 ?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveLight@@@Z.
// Port (NF151, q4506): the local `this` / CManagerLocalLight / heap / CPointerVector stand-ins (and the made-up same-TU overflow body that called
//   GrowVectorStorage_s7) are replaced by the shared headers, in the landed 0x00B47BC0 UpdateShadowedSpotLights form:
//   engine/CEngineLightingManager.h (LocalLightList +0xac, the nested CManagerLocalLight with its LOCAL_HEAP operator new; NF151 opt-in
//   FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE declares this member), engine/CDynamicObjectHeap.h, engine/CEngineInternalPrimitiveLight.h
//   (NF151 opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVELIGHT_TYPED: WorldPos / Colour typed) and STLport vector<CManagerLocalLight*>::push_back,
//   whose _M_insert_overflow 0x00B49DF0 (__true_type) stays out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE).
// Callees: CDynamicObjectHeap<72,16>::CreateMoreBlocks 0x00B48F90 (heap 0x01436FB0), CEngineLocalLightElement::CEngineLocalLightElement
//   0x00B44E30, vector<CManagerLocalLight*>::_M_insert_overflow 0x00B49DF0. Constants 0.1f 0x012A20D0, 1/255 0x01231724.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_STLP_TAG_CTORS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVELIGHT_TYPED
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineInternalPrimitiveLight.h"

void CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveLight& light)
{
    if (light.Radius < 0.1f)
        return;
    if (light.Colour.R < 1.0f / 255.0f && light.Colour.G < 1.0f / 255.0f && light.Colour.B < 1.0f / 255.0f)
        return;
    LocalLightList.push_back(new CManagerLocalLight(light.WorldPos, light.Radius, light.SpotRadius, light.Colour, light.ChannelMask));
}
