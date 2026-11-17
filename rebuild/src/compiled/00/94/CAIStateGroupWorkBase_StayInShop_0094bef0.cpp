// CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop() const -- true when the creature's shop (shop keeper interface 0x90 -> shop)
//   reports its keeper available
// Leaf: ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BEF0, length 83 bytes (+ int3 padding): the out-of-line COMDAT of the class-template member.
// Identity (NF56, round 7, queue q10439): Ego_r.pdb ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (protected const; donor
//   0x009702A0 right before CAIStateGroup_SellToBuyer::Cleanup 0x00970310, as retail 0x0094BEF0 before Cleanup 0x0094BF50). The body is
//   engine/CAIStateGroup_WorkBase.h FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP (the shop keeper interface, its shop, the shop's
//   keeper-available flag); the landed CAIStateGroup_SellToBuyer::Update 0x0094C050 expands it at three sites and calls this COMDAT at the
//   fourth (0x0094C687), binding this spelling. The catalog name IsInWorkHours was wrong: Ego_r's IsInWorkHours is the village work-time test
//   (CTCVillage::IsWorkTime), not the shop one. Was a local CAIStateGroupBase / CAIStateGroup_WorkBase model with a public non-const
//   IsInWorkHours.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (out of line here), CTCShopKeeper::GetShopTC @ 0x00798F50,
//   CTCShop::IsShopKeeperAvailable @ 0x00773AA0. The explicit instantiation emits the member COMDAT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOP_AVAILABLE
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCShop.h"
#include "engine/CAIStateGroup_WorkBase.h"

template bool CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop() const;
