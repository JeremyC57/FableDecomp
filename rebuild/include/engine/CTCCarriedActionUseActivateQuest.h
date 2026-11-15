#ifndef FABLE_ENGINE_CTCCARRIEDACTIONUSEACTIVATEQUEST_H
#define FABLE_ENGINE_CTCCARRIEDACTIONUSEACTIVATEQUEST_H
// New header (agentC30, row 0x007B57C0): Ego_r.pdb CTCCarriedActionUseActivateQuest (no members of its own) and its override
// ?OnActionUse@CTCCarriedActionUseActivateQuest@@UAEXAAVCThing@@@Z (Ego_r 0x007B59A0 after GetSubDef<CActivateQuestDef> 0x007B5890 /
// CActivateQuestDef::Transfer 0x007B5950; retail 0x007B57C0 after GetSubDef<CActivateQuestDef> 0x007B5680, vtable slot 28 like the
// sibling CTCCarriedActionUseExpression 0x007EF200). Needs FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS.
#include "engine/CTCCarriedActionUseBase.h"
class CTCCarriedActionUseActivateQuest : public CTCCarriedActionUseBase {
public:
    virtual void OnActionUse(CThing& user);                              // slot 28, 0x007B57C0
};
FABLE_STATIC_ASSERT(sizeof(CTCCarriedActionUseActivateQuest) == 20);
#endif
