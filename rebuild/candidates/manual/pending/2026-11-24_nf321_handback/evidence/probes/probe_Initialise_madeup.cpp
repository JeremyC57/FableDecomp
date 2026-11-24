#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER
#include "engine/CAIStateGroup_BirdGroundWander.h"
struct P : CAIStateGroup_BirdGroundWander { virtual void Initialise_madeup(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) override; };
