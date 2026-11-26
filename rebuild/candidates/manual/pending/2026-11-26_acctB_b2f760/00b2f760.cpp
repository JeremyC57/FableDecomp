// CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager()
// Leaf: ??0CEnginePrimitiveOcclusionManager@@QAE@XZ
// Identity: Ego_r.pdb 0x009D0020 and FableWin.pdb 0x02DC4B70. Retail Fable.exe 0x00B2F760, 95 bytes,
//   vtable 0x012A124C, base constructor 0x00B59710. PrimitiveOccludeDelay = 4, LastCameraDirection = (1, 0, 0),
//   LastCameraPosition = (0, 0, 0), MaxCameraAngleStep = 1/6, MaxCameraPositionStep = 4, both flags false.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#include "engine/CEnginePrimitiveOcclusionManager.h"
CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager()
{
    LastCameraDirection = C3DVector(1.0f, 0.0f, 0.0f);
    InvalidateOcclusion = false;
    NewSceneFlag = false;
    PrimitiveOccludeDelay = 4;
    LastCameraPosition = C3DVector(0.0f, 0.0f, 0.0f);
    MaxCameraAngleStep = 1.0f / 6.0f;
    MaxCameraPositionStep = 4.0f;
}
