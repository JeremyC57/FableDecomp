// Flags: 
// Leaf: ??$AllocAlignedArrayOfObjectsNoConstuctors@M@CEngineLandscapeMemoryManager@@QAEPAMJJPAVCMovableResource@@@Z
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#include "engine/CEngineLandscapeMemoryManager.h"

template float* CEngineLandscapeMemoryManager::AllocAlignedArrayOfObjectsNoConstuctors<float>(long, long, CMovableResource*);
