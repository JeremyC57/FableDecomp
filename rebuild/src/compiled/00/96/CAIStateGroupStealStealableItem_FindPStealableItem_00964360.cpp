// CAIStateGroup_StealStealableItem::FindPStealableItem() -- nearest object with the stealable property and a free action point
// Leaf: ?FindPStealableItem@CAIStateGroup_StealStealableItem_Methods@@QAEPAVCThingObject@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964360, length 107 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThing<CFilter_Bind2<CThingFilter_ObjectHasProperty,
// CThingFilter_HasFreeActionPoint>, CThingCompare_Nearest, CThingObject> @ 0x00964230 (manifest name).
//
// Shape: the result pointer is a zero-initialised local returned after the search. The Nearest comparator
// copies the creature's position; the bound filter pair is a CFilter_Bind2 holding two const references: to
// the property filter (one word, property mask 0x08000000, stored as an immediate after the argument pushes)
// and to the EMPTY free-action-point filter, a 1-byte object VC places in the dead high byte of the frame's
// lowest word (`lea edx,[esp+7]`) only when it is a TEMPORARY in the Bind2 construction (a named local takes
// a whole word and shifts the layout). Frame: 0x20 bytes. Tried for the copy order: memberwise copy constructor,
// NF137 (2026-11-20, q4994 / q5140): PeekThingSearchTools 0x008FD000 now binds the PDB ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ
//   (engine/CAIStateGroupBase.h _PEEK), was the _Methods pointer stand-in. The stand-in _Methods (no header; HEAD token, own leaf unchanged: its
//   one landed caller 0x00964DA0 binds a third spelling, so the PDB self rename ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ
//   waits: NAME_FIXES) now derives from the retail engine/CAIStateGroup_StealStealableItem.h class. The search 0x00964230 is the real same-TU
//   noinline GetPBestThing<Filter,Compare,T> body of engine/CThingSearchTools.h (BESTTHING_BODY; retail compiled it in this TU) under its PDB
//   template spelling (Ego_r.pdb 0x00988B00 + FableWin.pdb 0x02C3B050, QBE), was a QAE stand-in with a made-up search helper; the comparator
//   is engine/CThingCompare_Nearest.h, the filter pair engine/CFilter_Bind2.h; the two filters have no shared header and stay row-local.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_StealStealableItem.h"

class CThingFilter_ObjectHasProperty { public:         // one word: the property mask (no shared header)
    unsigned long Property;
    CThingFilter_ObjectHasProperty(unsigned long property) : Property(property) {}
    bool operator()(const CThingObject& thing) const;   // 0x006A7590
};

class CThingFilter_HasFreeActionPoint { public:        // empty filter (no shared header)
    CThingFilter_HasFreeActionPoint() {}
    bool operator()(const CThing& thing) const;         // 0x006A8B40
};

typedef CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint> CStealableFilter;

struct CAIStateGroup_StealStealableItem_Methods : CAIStateGroup_StealStealableItem {
    CThingObject* FindPStealableItem();
};

CThingObject* CAIStateGroup_StealStealableItem_Methods::FindPStealableItem()
{
    CThingObject* item = 0;
    CThingCompare_Nearest compare(GetCreature().GetPos());
    CThingFilter_ObjectHasProperty hasProperty(0x08000000);
    PeekThingSearchTools().GetPBestThing(CStealableFilter(hasProperty, CThingFilter_HasFreeActionPoint()), compare, &item);
    return item;
}
