// CTCBuyableHouse::GetSignPostPos(bool main, C3DVector& pos, CRightHandedSet* orientation)
// Grade: RELOCATION_MATCH (764 bytes) -- vector step via the file-local AddOffset(offset, position-by-value) helper
// Where a buyable house's sign post goes: on the existing sign-post marker among the house's owned things (by def name), else
// beside the house's door (3 ahead along the door's forward, 2 (main) or 4 (dressing) to its left, at ground height), trying the
// door's facing and a half turn about Z until the spot lies outside the building.
// Retail 0x006BFCD0, 764 bytes, __fastcall `ret 0Ch`, no frame pointer, no SEH, default flags.
// Callees: CVectorMap::LowerBound 0x0040F020, CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CTCPhysicsBase::GetRHSet() (vtable +0x120),
//   CRightHandedSet::Rotate 0x00A22F80, CTCBase::PeekWorldMap 0x00686D20, CWorldMap::GetGroundSizeZAt 0x0063A300,
//   CThingBuilding::IsPointInside 0x0082D000. Local inline: AddOffset (made-up, file-static, fully inlined).
// Leaf: ?GetSignPostPos@CTCBuyableHouse@@AAE_N_NAAVC3DVector@@PAVCRightHandedSet@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/C2DVector.h"
#include "engine/CThingBuilding.h"
#include "engine/CWorldMap.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBuyableHouse.h"

// AddOffset: made-up descriptive name (no symbol in Ego_r / FableWin / FSE / EgoCore). Retail's final add is sum-first with the
// sideways offset kept on the x87 stack (`fxch st(2) ; fadd st,st(2)` / `fstp st(1)`): VC7.1 emits that only when the position is
// a by-value copy evaluated before the offset, which is bound by const reference (toy probes in NOTES; every one-expression,
// two-statement, named-offset and member-operator spelling gives the offset-first add).
static inline C3DVector AddOffset(const C3DVector& offset, C3DVector position) { return position + offset; }

bool CTCBuyableHouse::GetSignPostPos(bool main, C3DVector& pos, CRightHandedSet* orientation)
{
    float side = main ? 2.0f : 4.0f;
    CTCThingOwner* owner;
    if (ParentThing->GetTC(TCI_THING_OWNER, &owner)) {
        CThing* door = 0;
        CThing* signPost = 0;
        long index;
        if (main)
            index = GetDefinitionManager().GetDefGlobalIndexFromName("MARKER_BUYABLE_HOUSE_MAIN_SIGNPOST");
        else
            index = GetDefinitionManager().GetDefGlobalIndexFromName("MARKER_BUYABLE_HOUSE_DRESSING_SIGNPOST");
        std::list<CIntelligentPointer<CThing> >& things = owner->GetObjectList();
        std::list<CIntelligentPointer<CThing> >::iterator end = things.end();
        for (std::list<CIntelligentPointer<CThing> >::iterator it = things.begin(); it != end; ++it) {
            CThing* thing = it->Get();
            if (thing && !thing->IsDead()) {
                if (thing->HasTC(TCI_DOOR))
                    door = thing;
                else if (thing->GetDefGlobalIndex() == index)
                    signPost = thing;
            }
        }
        if (signPost) {
            pos = signPost->GetPos();
            if (orientation)
                *orientation = signPost->GetPhysicsTC()->GetRHSet();
            return true;
        }
        if (door) {
            for (long i = 0; i < 2; ++i) {
                CRightHandedSet set = door->GetPhysicsTC()->GetRHSet();
                if (i)
                    set.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 0.5f);
                C3DVector forward = set.GetForward();
                C3DVector candidate = AddOffset(C3DVector(-forward.Y, forward.X, 0.0f) * side, set.GetForward() * 3.0f + door->GetPos());
                candidate.Z = PeekWorldMap().GetGroundSizeZAt(candidate);
                if (orientation)
                    *orientation = set;
                pos = candidate;
                if (ParentThing->GetType() == CThingBuilding::StatGetType() && !((CThingBuilding*)ParentThing)->IsPointInside(pos))
                    return true;
            }
        }
    }
    return false;
}