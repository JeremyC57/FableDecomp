#ifndef FABLE_ENGINE_CCAMERA_H
#define FABLE_ENGINE_CCAMERA_H
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
class CCamera {
private: C3DVector WorldPosition;
private: CRightHandedSet RHSet;
private: bool HeightLocked;
private: bool ViewVecZLocked;
private: unsigned long FOVFlags;
private: float HorizontalFOV;
private: float VerticalFOV;
private: float Zoom;
};
FABLE_STATIC_ASSERT(sizeof(CCamera)==56);
#endif
