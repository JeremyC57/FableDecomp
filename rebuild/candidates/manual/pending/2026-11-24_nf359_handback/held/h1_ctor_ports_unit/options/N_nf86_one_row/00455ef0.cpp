// NF359 probe (evidence only, not a candidate): the landed nf86 / nf88 per-TU anonymous-namespace helper form (GOTCHAS), here the Alloc
// body under a hashed helper name instead of ?Alloc@CThingNoiseDef (so no strong Alloc); measures what the set change would be.
// Leaf: ??0CThingNoiseDef@@QAE@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF_COMPONENTS
#define FABLE_RETAIL_CTHINGBASEDEF_CTOR_DECL
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#define FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL
#include "engine/CDefPointeeBase.h"
#pragma optimize("s", on)
#include "engine/CThingNoiseDef.h"

namespace {
// Fidelity: scaffolding, not retail
CDefClassBase* ReferenceThingNoiseDefCtor()
{
    return new CThingNoiseDef;
}
}
