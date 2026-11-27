#ifndef FABLE_ENGINE_CTCACTIONUSEBED_H
#define FABLE_ENGINE_CTCACTIONUSEBED_H
#ifdef FABLE_RETAIL_BED_USE_PASS28
#include "engine/CTCActionUseBase.h"
#include "engine/CIntelligentPointer.h"
class CTCActionUseBed : public CTCActionUseBase { public:
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
private:
#endif
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
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
public:
#endif
    virtual void OnActionUse(CThing& user);
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
    virtual void GlobalUpdate(bool paused);
#endif

};
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
FABLE_STATIC_ASSERT(sizeof(CTCActionUseBed)==0x34);
#else
FABLE_STATIC_ASSERT_AT(20, offsetof(CTCActionUseBed,PBuilding)==0x23);
#endif
#endif
#endif
