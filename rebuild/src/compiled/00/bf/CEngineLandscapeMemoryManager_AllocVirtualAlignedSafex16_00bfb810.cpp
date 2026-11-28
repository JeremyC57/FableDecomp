// Flags: 
// Leaf: ??$AllocAlignedArrayOfObjectsNoConstuctors@VC4DVector@@@CEngineLandscapeMemoryManager@@QAEPAVC4DVector@@JJPAVCMovableResource@@@Z
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#include "engine/C4DVector.h"
#include "engine/CEngineLandscapeMemoryManager.h"

template C4DVector* CEngineLandscapeMemoryManager::AllocAlignedArrayOfObjectsNoConstuctors<C4DVector>(long, long, CMovableResource*);
