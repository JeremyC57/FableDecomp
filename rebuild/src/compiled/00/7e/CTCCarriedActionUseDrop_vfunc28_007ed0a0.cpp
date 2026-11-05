// CTCCarriedActionUseDrop::vfunc_28(CThingAICreature* user) -- run the base use hook; if the dropped thing is an alarm item (interface 0x48) and the user is flagged to react, make the user alert the player, else take the thing out of the user's carrying component
// Leaf: ?vfunc_28@CTCCarriedActionUseDrop@@QAEXPAVCThingAICreature@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
//
// Retail 0x007ED0A0, 151 bytes (`ret 4`). Vtable slot 28 of CTCCarriedActionUseDrop. Callees: the
// base hook @ 0x007EAB90 (empty `ret 4` in another TU), CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x0040F020 (this module's copy), CCreatureAction_AlertToPlayer::
// CCreatureAction_AlertToPlayer(CThingAICreature*) @ 0x007ECFD0, CThingCreatureBase::SetCurrentAction
// @ 0x006644F0, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0, CTCCarrying::RemoveThing
// (CThing*) @ 0x006A19B0.
// Shape: the user's CTCCarrying (interface 0x46: `test byte [edi+28h],40h`) comes from the inline
// out-parameter GetInterface whose out pointer is NOT initialised (a clear bit reads the stale frame
// word `mov eax,[esp+8]`); the parent thing's interface-0x48 test reads the whole bitset word
// (`mov ecx,[edx+28h] ; test ch,1`) and the parent stays in edx for the RemoveThing push. The
// 0xa8-byte alert action is a temporary passed straight from its constructor.
class CTCBase;
class CCreatureActionBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
};

class CTCCarrying { public:
    void RemoveThing(CThing* thing);                        // 0x006A19B0
};

class CCreatureActionBase { public:                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                                 // 0x00693EF0
};
class CCreatureAction_AlertToPlayer : public CCreatureActionBase { public:
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);   // 0x007ECFD0
};

class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseDrop : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseDrop::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CTCCarrying* carrying;
    user->GetTC(TCI_CARRYING, &carrying);                    // TC_CARRYING
    if (ParentThing->HasTC(TCI_WEAPON) && (user->GetTypeFlags() & 8))
        user->SetCurrentAction(&CCreatureAction_AlertToPlayer(user));
    else
        carrying->RemoveThing(ParentThing);
}