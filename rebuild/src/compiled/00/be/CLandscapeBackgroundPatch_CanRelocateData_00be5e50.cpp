// Leaf: ?CanRelocateData@CLandscapeBackgroundPatch@@UBE_NJ@Z
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_CLANDSCAPEBACKGROUNDPATCH_RTTI_DECLS
#include "engine/CLandscapeBackgroundPatch.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner { char pad[0x5c]; unsigned char flag; };
class CLandscapeBackgroundTreeNode { public: char pad[0xa4]; Inner* inner; };

bool CLandscapeBackgroundPatch::CanRelocateData(long a) const
{
    CLandscapeBackgroundPatch* patch = const_cast<CLandscapeBackgroundPatch*>(this);
    (void)a;
    return patch->Parent->inner->flag == 0;
}
// Flatten route (flatten_pilot v4, NF324): CLandscapeBackgroundPatch_CanRelocateData -> CLandscapeBackgroundPatch::CanRelocateData, the RTTI-placed PDB member; class model: engine/CLandscapeBackgroundPatch.h opt-in FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH, FABLE_RETAIL_CLANDSCAPEBACKGROUNDPATCH_RTTI_DECLS.
