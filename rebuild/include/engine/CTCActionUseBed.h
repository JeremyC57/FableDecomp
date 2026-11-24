#ifndef FABLE_ENGINE_CTCACTIONUSEBED_H
#define FABLE_ENGINE_CTCACTIONUSEBED_H
#ifdef FABLE_RETAIL_BED_USE_PASS28
#include "engine/CTCActionUseBase.h"
#include "engine/CIntelligentPointer.h"
class CTCActionUseBed : public CTCActionUseBase { public:
    bool WaitingForFadeOut;                             // +0x14
    bool WaitingForFadeIn;                              // +0x15
    bool InAtomicAction;                                // +0x16
    bool WaitingForConfirmation;                        // +0x17
    long PlayerNumberOfThingUsingBed;                   // +0x18
    long Cost;                                          // +0x1c
    bool UseableByHero;                                 // +0x20
    bool OwnedByHero;                                   // +0x21
    bool ForceTrespassOnceFadedOut;                     // +0x22
    CIntelligentPointer<CThing> PBuilding;              // +0x23
    CIntelligentPointer<CThing> PThingUser;             // +0x2b
    virtual void OnActionUse(CThing& user);
};
FABLE_STATIC_ASSERT(offsetof(CTCActionUseBed,PBuilding)==0x23);
#endif
#endif
