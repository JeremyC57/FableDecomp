#define FABLE_RETAIL_STLP_LIST
// Original complete shared owner/member classes and actual CThing& parameter; no stand-in list implementation.
// Leaf: ??0CAnimateThing@@QAE@AAVCThing@@@Z
// Flags: (none)
#define FABLE_RETAIL_CANIMATETHING_CTOR_PASS26
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CTOR_BODY_PASS26
#define FABLE_RETAIL_CANIMFROMFILEINFO_CTOR_PASS26
#include "engine/CAnimateThing.h"
CAnimateThing::CAnimateThing(CThing& parent_thing)
 :ChangeAlphaBy(1.0f),AnimDelay(2),Interruptable(true),ParentThing(parent_thing)
{
    CurrentAnimRefreshStamp=1;
    LastAnimRefreshStamp=0;
    while(DelayedAnimQueue.size()!=3)
        DelayedAnimQueue.push_back(CDelayedAnimation());
}