// CTCDExperienceOrb::IsDriverType(long) const -- true when the asked driver type is this driver's type (26)
// Leaf: ?IsDriverType@CTCDExperienceOrb@@UBE_NJ@Z
// Identity: slot 28 of CTCDExperienceOrb's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDExperienceOrb@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: engine/CTCDExperienceOrb.h (opt-in above).
#define FABLE_RETAIL_CTCDEXPERIENCEORB_DRIVERSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDExperienceOrb.h"
bool CTCDExperienceOrb::IsDriverType(long type) const
{
    return type == 26;
}
