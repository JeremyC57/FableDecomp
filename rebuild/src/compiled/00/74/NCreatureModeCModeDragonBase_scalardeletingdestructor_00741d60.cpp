// 00741d60 NCreatureMode::CModeDragonBase scalar deleting destructor
#ifdef _MSC_VER
#pragma optimize("g",on)
#endif
#define FABLE_RETAIL_CANIMFILTERBASE_SIZE_PASS27
#include "engine/CAnimFilterBase.h"
#ifdef _MSC_VER
#pragma optimize("g",on)
#endif
#define FABLE_RETAIL_DRAGONBASE_DONOR_PROTECTED_PASS29
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE
// Leaf: ??_GCModeDragonBase@NCreatureMode@@UAEPAXI@Z
// Inline-budget: native 00741d60 expands both filter cleanups and the mode-base destructor.
// With the restored global optimization, /d2vol4 emits 30 bytes and /d2vol5 emits all 69.
// Flags: /Og- /d2vol5
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE
#define FABLE_RETAIL_CMODEBASE_CTOR
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CModeDragonBase.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"

inline CTCCreatureModeManager::CModeBase::~CModeBase() {}

void NCreatureMode::CModeDragonBase::OnActivate()
{
    ParentThing.GetPhysicsTC()->EnableGravity(false);
    CModeBase::OnActivate();
}

void NCreatureMode::CModeDragonBase::OnDeactivate()
{
    ParentThing.GetPhysicsTC()->EnableGravity(true);
    CModeBase::OnDeactivate();
}

NCreatureMode::CModeDragonBase::CModeDragonBase(CThing& thing, const CCharString& group_name)
    : CModeBase(thing), Filter(group_name) {}

const CAnimFilterBase& NCreatureMode::CModeDragonBase::GetAnimFilter() const
{
    FABLE_STATIC_ASSERT(offsetof(CModeDragonBase, Filter) == 0x1c);
    FABLE_STATIC_ASSERT(offsetof(CModeDragonBase, PTarget) == 0x28);
    return Filter;
}