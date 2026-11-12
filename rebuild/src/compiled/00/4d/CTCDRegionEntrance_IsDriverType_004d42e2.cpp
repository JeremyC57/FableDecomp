// CTCDRegionEntrance::IsDriverType(long) const -- true when the asked driver type is this driver's type (24)
// Leaf: ?IsDriverType@CTCDRegionEntrance@@UBE_NJ@Z
// Identity: slot 28 of CTCDRegionEntrance's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDRegionEntrance@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: no engine/ header exists for CTCDRegionEntrance; declared over the shared engine/CTCDriverBase.h with just this override.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
class CTCDRegionEntrance : public CTCDriverBase {
public:
    virtual bool IsDriverType(long type) const;
};
bool CTCDRegionEntrance::IsDriverType(long type) const
{
    return type == 24;
}
