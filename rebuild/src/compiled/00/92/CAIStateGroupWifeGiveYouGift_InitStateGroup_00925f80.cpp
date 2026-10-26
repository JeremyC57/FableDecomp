#include "engine/CAIStateGroup_WifeGiveYouGift.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld; class CThingAICreature;
struct CAIStateGroup_WifeGiveYouGift_Methods : CAIStateGroup_WifeGiveYouGift {
    void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_WifeGiveYouGift_Methods::InitStateGroup(const CWorld&, CThingAICreature&){ FrameToSpeakNext=0; GiftToGiveDef=0; DeliveredGiveSpeech=0; }