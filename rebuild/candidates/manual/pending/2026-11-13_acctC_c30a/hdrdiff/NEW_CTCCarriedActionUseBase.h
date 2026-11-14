#ifndef FABLE_ENGINE_CTCCARRIEDACTIONUSEBASE_H
#define FABLE_ENGINE_CTCCARRIEDACTIONUSEBASE_H
// New header (agentC25, row 0x007EF200): Ego_r.pdb CTCCarriedActionUseBase, 20 bytes (struct_layouts_egor.tsv: no members of its
// own), the base of the carried-item action-use components (CTCCarriedActionUseLamp / Show / Expression ...). It derives the
// shared CTCActionUseBase (20 bytes; its OnActionUse slot 28 0x007EAB90 is the one the carried classes call as their base).
#include "engine/CTCActionUseBase.h"
class CTCCarriedActionUseBase : public CTCActionUseBase {
};
FABLE_STATIC_ASSERT(sizeof(CTCCarriedActionUseBase) == 20);
#endif
