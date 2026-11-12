// CTCDPhysicalObstruction::IsDriverType(long) const -- true when the asked driver type is this driver's type (29)
// Leaf: ?IsDriverType@CTCDPhysicalObstruction@@UBE_NJ@Z
// Identity: slot 28 of CTCDPhysicalObstruction's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDPhysicalObstruction@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: engine/CTCDPhysicalObstruction.h (opt-in above).
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_DRIVERSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDPhysicalObstruction.h"
bool CTCDPhysicalObstruction::IsDriverType(long type) const
{
    return type == 29;
}
