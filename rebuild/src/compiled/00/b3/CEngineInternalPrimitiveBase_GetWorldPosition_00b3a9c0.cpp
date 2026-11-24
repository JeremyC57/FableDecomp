// Leaf: ?GetWorldPosition@CEngineInternalPrimitiveBase@@UAE_NAAVC3DVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"

bool CEngineInternalPrimitiveBase::GetWorldPosition(class C3DVector& out)
{
    (void)out;
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CEngineInternalPrimitiveBase_GetWorldPosition -> CEngineInternalPrimitiveBase::GetWorldPosition, the RTTI-placed PDB member; class model: engine/CEngineInternalPrimitiveBase.h under FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED (preamble of CEngineInternalPrimitiveBase_IsInsideFrustum_00b3aa10.cpp).
