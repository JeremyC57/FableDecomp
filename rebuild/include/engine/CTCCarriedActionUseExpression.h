#ifndef FABLE_ENGINE_CTCCARRIEDACTIONUSEEXPRESSION_H
#define FABLE_ENGINE_CTCCARRIEDACTIONUSEEXPRESSION_H
// New header (agentC25, row 0x007EF200): Ego_r.pdb CTCCarriedActionUseExpression, 20 bytes (no members of its own), and its
// override ?OnActionUse@CTCCarriedActionUseExpression@@UAEXAAVCThing@@@Z (Ego_r 0x007E8CE0; retail 0x007EF200, vtable slot 28).
// Needs FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS (the base's virtual OnActionUse).
#include "engine/CTCCarriedActionUseBase.h"
class CTCCarriedActionUseExpression : public CTCCarriedActionUseBase {
public:
    virtual void OnActionUse(CThing& user);                              // slot 28, 0x007EF200
};
FABLE_STATIC_ASSERT(sizeof(CTCCarriedActionUseExpression) == 20);
#endif
