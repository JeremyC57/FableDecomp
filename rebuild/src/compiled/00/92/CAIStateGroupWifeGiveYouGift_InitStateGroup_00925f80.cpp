// CAIStateGroup_WifeGiveYouGift::InitStateGroup(const CWorld&, CThingAICreature&) -- resets the gift state: no speech frame, no gift def, speech not delivered
// Leaf: ?InitStateGroup@CAIStateGroup_WifeGiveYouGift@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00925F80, thiscall, `ret 8`; vftable 0x0128AC48 slot 13 (its only retail reference).
// Identity (NF475, r467b unit with ctor 0x008FE72B): RTTI .?AVCAIStateGroup_WifeGiveYouGift@@ vtable 0x0128AC48 slot 13 = 0x00925F80 + Ego_r.pdb / FableWin.pdb
//   ?InitStateGroup@CAIStateGroup_WifeGiveYouGift@@UAEXABVCWorld@@AAVCThingAICreature@@@Z (public override; both arguments unused). Was the stand-in class
//   ?InitStateGroup@CAIStateGroup_WifeGiveYouGift_Methods@@QAEXABVCWorld@@AAVCThingAICreature@@@Z (T3 nf319: the class existed only under
//   FABLE_RETAIL_WIFE_GIFT_UPDATE_PASS24, the view the ctor row 0x008FE72B compiles; InitStateGroup is declared in its add-only
//   FABLE_RETAIL_CAISTATEGROUP_WIFEGIVEYOUGIFT_CTOR block, r474c).
// Model: engine/CAIStateGroup_WifeGiveYouGift.h retail view FABLE_RETAIL_WIFE_GIFT_UPDATE_PASS24 + FABLE_RETAIL_CAISTATEGROUP_WIFEGIVEYOUGIFT_CTOR (the ctor row's class).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_WIFE_GIFT_UPDATE_PASS24
#define FABLE_RETAIL_CAISTATEGROUP_WIFEGIVEYOUGIFT_CTOR
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WifeGiveYouGift.h"

void CAIStateGroup_WifeGiveYouGift::InitStateGroup(const CWorld&, CThingAICreature&)
{
    FrameToSpeakNext = 0;
    GiftToGiveDef = 0;
    DeliveredGiveSpeech = false;
}
