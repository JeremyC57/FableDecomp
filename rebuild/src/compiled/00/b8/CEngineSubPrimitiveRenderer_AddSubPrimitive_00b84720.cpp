// CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager) -- queue a sub-primitive in its type's sort list: an integer sort key from the clamped scaled distance, push_back of the (key, sub-primitive) sort element, and the manager recorded on the sub-primitive
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@PAVCEngineSubPrimitiveBase@@MPAVCEnginePrimitiveManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84720, 142 bytes, __fastcall, `ret 10h`.
// Retail-asm: 0x00B84735 `fistp dword ptr [esp]` (after 0x00B84731 `fld dword ptr [esp+14h]`) -- engine/GFFloatToLongNear.h, the
//   engine's inline-assembly float-to-long on the homed distance (VC7.1 `(long)x` calls __ftol2, /QIfist gives `fistp qword`).
// Identity: FableWin.pdb ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@PAVCEngineSubPrimitiveBase@@MPAVCEnginePrimitiveManager@@@Z
//   (the typed overload; the (sub-primitive, distance, alpha, flags, manager) overload is 0x00B846E0). Members from Ego_r.pdb:
//   MaxSortDistance +0x10, SortDistanceMultiplier +0x14, SubPrimitiveLists[15] +0x18 (24-byte retail lists), the sub-primitive's
//   Manager +0x14.
// Callees: vector<CSortElement>::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x00B85EC0 (push_back's grow).
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_RENDERER_FRIEND
#include "engine/CEngineRenderEnums.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/GFFloatToLongNear.h"

void CEngineSubPrimitiveRenderer::AddSubPrimitive(ESubPrimitiveType type, CEngineSubPrimitiveBase* subPrimitive, float distance,
                                                  CEnginePrimitiveManager* manager)
{
    float sortDistance = (MaxSortDistance - distance) * SortDistanceMultiplier;
    long sortValue = GFFloatToLongNear(sortDistance);
    if (sortValue < 0)
        sortValue = 0;
    else if (sortValue > 0x3FFFFFFF)
        sortValue = 0x3FFFFFFF;
    SubPrimitiveLists[type].Elements.push_back(CSortElement(sortValue, subPrimitive));
    subPrimitive->Manager = manager;
}