// Flags: 
// Leaf: ??$AllocArrayOfObjectsNoConstuctors@E@CEngineLandscapeMemoryManager@@QAEPAEJPAVCMovableResource@@@Z
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#include "engine/CEngineLandscapeMemoryManager.h"

template unsigned char* CEngineLandscapeMemoryManager::AllocArrayOfObjectsNoConstuctors<unsigned char>(long, CMovableResource*);
