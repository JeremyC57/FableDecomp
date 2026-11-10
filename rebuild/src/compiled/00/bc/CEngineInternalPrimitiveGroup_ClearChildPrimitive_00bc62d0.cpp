// 00bc62d0 CEngineInternalPrimitiveGroup::ClearChildPrimitive
// Leaf: ?ClearChildPrimitive@CEngineInternalPrimitiveGroup@@UAEXK@Z
// Evidence: Ego_r00a7b480, FableWin02ebbf70 and native group virtual slot10.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_AT
#include "engine/CEngineInternalPrimitiveGroup.h"
void CEngineInternalPrimitiveGroup::ClearChildPrimitive(unsigned long index)
{
    if(HasChildPrimitive(index)){
        Primitives.At(index).Clear();
        UpdateRenderLayerMask();
    }
}