// CEngineInternalPrimitive2DViewport::ClearChildPrimitive(unsigned long index) -- when child slot `index` holds a primitive, clear its handle (last-handle notification + counted release) and recompute the render-layer mask
// Leaf: ?ClearChildPrimitive@CEngineInternalPrimitive2DViewport@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2120, 106 bytes, `ret 4` (the hand2c oracle row was cut to 97 bytes, the group twin's
//   length; graded against the full 106 from Fable.exe via oracle_override.tsv).
// Identity: Ego_r.pdb / FableWin.pdb ?ClearChildPrimitive@CEngineInternalPrimitive2DViewport@@UAEXK@Z; the body is the landed
//   CEngineInternalPrimitiveGroup::ClearChildPrimitive 0x00BC62D0 with the handle array at +0x94.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_AT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_MEMBERS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_CLEARCHILD
#include "engine/CEngineInternalPrimitive2DViewport.h"

void CEngineInternalPrimitive2DViewport::ClearChildPrimitive(unsigned long index)
{
    if(HasChildPrimitive(index)){
        Primitives.At(index).Clear();
        UpdateRenderLayerMask();
    }
}
