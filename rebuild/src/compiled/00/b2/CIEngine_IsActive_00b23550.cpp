// Leaf: ?IsActive@CIEngine@@UBE_NXZ

#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
bool CIEngine::IsActive() const
{
    const CIEngine* self = this;
  return self->Active;
}
// Flatten route (flatten_pilot v4, NF324): CIEngine_IsActive -> CIEngine::IsActive, the RTTI-placed PDB member; class model: engine/CIEngine.h under FABLE_RETAIL_CIENGINE.
