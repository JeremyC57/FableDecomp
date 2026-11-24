// CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory -- configure unsheathe animation, item and owned follow-up action
// Retail 0069F0F0,252 bytes. PDB signature confirms creature&, string&, item&, long,long, action*.
// Leaf: ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@ABVCCharString@@AAVCThing@@JJPAVCCreatureActionBase@@@Z
// NF95 (2026-11-19, q11253): the lookup is the PDB protected static CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (Ego_r.pdb
//   0x00518E00 / FableWin.pdb 0x01DB7950; was the free stand-in FindAnimation); the base constructor 0x00693B30 takes the PDB
//   creature reference (??0CCreatureActionBase@@QAE@AAVCThingCreatureBase@@MMABVCAnimationEntry@@ABVCSoundPair@@JJ@Z).
// NF270 (2026-11-22, slot 1 port): moved off its local models (CCreatureActionBase, CSoundPair, CAnimationEntry, CIVCountedPointeeBase,
//   CBaseIntelligentPointer / CIntelligentPointer, the derived class) onto the shared headers, in the define set of its landed sibling
//   constructor 0x006A0150 (same class, same header): engine/CCreatureAction_UnsheatheItemFromInventory.h on the retail
//   CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE + _ANIMCTOR), engine/CAnimationEntry.h (TRANSITION_CTOR ??0CAnimationEntry@@QAE@ABV0@J@Z
//   0x005DEC70, its CIVCountedPointer<CAnimComponentMap> released in place), CSoundPair, CIntelligentPointer<CThing> (PUnsheatheItem +0xA8)
//   and CCountedPointer<CCreatureActionBase> (PFollowUpAction +0xB0, DeleteData 0x00668610). The emitted vtable now inherits slot 1 as
//   ?Validate@CBaseObject@@UBEXXZ (retail 0x0125CCBC slot 1 = 0x0099A400; no PDB has a CCreatureActionBase::Validate) and binds slot 12
//   as the private ?Clone@CCreatureAction_UnsheatheItemFromInventory@@EBEPAVCCreatureActionBase@@XZ (Ego_r 0x006C59B0 / FableWin 0x02211670).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CANIMATIONENTRY_TRANSITION_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature, const CCharString& animation,
        CThing& item, long first, long second, CCreatureActionBase* followUp)
    : CCreatureActionBase(creature, -1.0f, 0.0f, CAnimationEntry(GetRandomAnimFromName(creature, animation), 0), CSoundPair(), first, second)
    , PUnsheatheItem(&item)
    , PFollowUpAction(followUp)
{
}
