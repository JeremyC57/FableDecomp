// Leaf: ?GetDebugText@CAIStateGroupBase@@UBEPBVCCharString@@XZ
// CAIStateGroupBase::GetDebugText() const -- returns 0.
// Names: RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) + Ego_r/FableWin name set; the landed stand-in
//   ?CAIStateGroupBase_GetDebugText@@YIPBVCCharString@@PBVCAIStateGroupBase@@@Z becomes the virtual member of the polymorphic class in
//   engine/CAIStateGroupBase.h (branch FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS; defines FABLE_RETAIL_CAISTATEGROUPBASE,
//   FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS). NF429 G1 (CONST shape).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
const CCharString* CAIStateGroupBase::GetDebugText() const
{
    return 0;
}
