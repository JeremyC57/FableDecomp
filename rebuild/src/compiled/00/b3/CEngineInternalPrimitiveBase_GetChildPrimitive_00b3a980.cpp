// Leaf: ?GetChildPrimitive@CEngineInternalPrimitiveBase@@UAEPAVCEnginePersistentPrimitiveBase@@K@Z
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEngineInternalPrimitiveBase.h"
class CEnginePersistentPrimitiveBase;

class CEnginePersistentPrimitiveBase* CEngineInternalPrimitiveBase::GetChildPrimitive(unsigned long index)
{
    (void)index;
    return 0;
}
// Flatten route (flatten_pilot v4, NF324): CEngineInternalPrimitiveBase_GetChildPrimitive -> CEngineInternalPrimitiveBase::GetChildPrimitive, the RTTI-placed PDB member; class model: engine/CEngineInternalPrimitiveBase.h under FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED (preamble of CEngineInternalPrimitiveBase_IsInsideFrustum_00b3aa10.cpp).
