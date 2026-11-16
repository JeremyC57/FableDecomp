// CCreatureAction_UnsheatheToShowToSheatheTrophy::~CCreatureAction_UnsheatheToShowToSheatheTrophy() -- the implicit destructor's own COMDAT: destroy
// Trophy (+0xBC, CIntelligentPointer<CThing>, dtor 0x00A01C10) and ShowAnimName (+0xB8, CCharString, dtor 0x0099EAE0), then tail-jump to
// ~CCreatureAction_UnsheatheItemFromInventory 0x005CC091 (no vptr store)
// Leaf: ??1CCreatureAction_UnsheatheToShowToSheatheTrophy@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF860, 33 bytes.
// NF39 (2026-11-16): moved off its `CAggregate_006df860` stand-in (a generated aggregate model, catalog label `_Dest_val<...>` propagated) onto the
//   shared engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h, which names 0x006DF860. Evidence: Ego_r.pdb + FableWin.pdb
//   ??1CCreatureAction_UnsheatheToShowToSheatheTrophy@@UAE@XZ; members ShowAnimName +0xB8 / Trophy +0xBC (Ego_r.pdb, header); CTCTrophy::GlobalUpdate
//   0x006DEEE0 destroys its action temporary with it (already bound). Opt-in FABLE_RETAIL_CCREATUREACTION_UNSHEATHETOSHOWTOSHEATHETROPHY_IMPLICIT_DTOR:
//   retail's COMDAT has no vptr store, so it is the compiler-generated destructor (a user-declared one stores the vptr first).
// Defines: the landed 0x006DEEE0 GlobalUpdate preamble.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_FACTIONMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CFACTIONMANAGER_BYCRC
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EMessageEventType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHETOSHOWTOSHEATHETROPHY_IMPLICIT_DTOR
#include "engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFactionManager.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"

// The implicit destructor emitted as its own COMDAT by these emitters (not part of the graded body), as the landed map rows 0x009AC130 /
//   0x006C8270 do: Use_ generates it, Kill_ calls it out of line (qualified: no virtual dispatch).
void Use_006df860(CCreatureAction_UnsheatheToShowToSheatheTrophy* object) { object->CCreatureAction_UnsheatheToShowToSheatheTrophy::~CCreatureAction_UnsheatheToShowToSheatheTrophy(); }
void Kill_006df860(CCreatureAction_UnsheatheToShowToSheatheTrophy* object) { object->CCreatureAction_UnsheatheToShowToSheatheTrophy::~CCreatureAction_UnsheatheToShowToSheatheTrophy(); }
