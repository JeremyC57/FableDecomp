// CTCDRegionExit::IsDriverType(long) const -- true when the asked driver type is this driver's type (25)
// Leaf: ?IsDriverType@CTCDRegionExit@@UBE_NJ@Z
// Identity: slot 28 of CTCDRegionExit's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDRegionExit@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: engine/CTCDRegionExit.h (opt-in above).
#define FABLE_RETAIL_CTCDREGIONEXIT_DRIVERSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDRegionExit.h"
bool CTCDRegionExit::IsDriverType(long type) const
{
    return type == 25;
}
