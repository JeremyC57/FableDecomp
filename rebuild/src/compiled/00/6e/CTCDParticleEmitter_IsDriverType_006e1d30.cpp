// CTCDParticleEmitter::IsDriverType(long) const -- true when the asked driver type is this driver's type (19)
// Leaf: ?IsDriverType@CTCDParticleEmitter@@UBE_NJ@Z
// Identity: slot 28 of CTCDParticleEmitter's vtable (the first virtual CTCDriverBase adds after CTCBase's 28 slots; engine/CTCDriverBase.h
//   FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE, the rough-area scans `push type ; call [edx+70h]`); Ego_r/FableWin name set:
//   ?IsDriverType@CTCDParticleEmitter@@UBE_NJ@Z; the body compares its one argument with a constant (`sete al`, `ret 4`).
// Model: engine/CTCDParticleEmitter.h (opt-in above).
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDParticleEmitter.h"
bool CTCDParticleEmitter::IsDriverType(long type) const
{
    return type == 19;
}
