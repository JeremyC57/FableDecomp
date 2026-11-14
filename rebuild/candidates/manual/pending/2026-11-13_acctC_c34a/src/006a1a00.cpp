// CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation(const CThing&, C3DVector&, CRightHandedSet&, bool, float) -- position and orientation of a carried thing: its carry slot's dummy on the carrier's graphic (else the carrier's physics position and facing), turned for a creature's primary slot, and pointed at the secondary dummy for a two-handed hold
// Grade: DIFFER(864v864, 3 hunks; q masks a wrong call: 2nd conversion calls ??B 0x00415D70, retail GetString 0x00415DD0) -- see NOTES
// Leaf: ?GetCarriedThingInterpolatedPosAndOrientation@CTCCarrying@@QBE_NABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A1A00, 864 bytes, thiscall `ret 14h`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_INTERPOLATEDPOS
#define FABLE_RETAIL_CVECTORMAP_VALFIND_CONST
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INIT
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCarrySlotDef.h"

// Descriptive name (no PDB data symbol): a never-written global bool next to the NGlobalConsole flags (0x01375748..); read only here
// (0x006A1BB9).
extern bool g_bTurnPrimarySlotCarriedThing;

// Spelling notes (agentC34). EVIDENCED: the named physics pointer + the temporary up vector inside the Init call (retail stores the
// three up floats after the facing call's pushes, 0x006A1B73..0x006A1B83); the secondary name fetched with GetString 0x00415DD0
// (called at 0x006A1C5A before any GetDummyObject push, destroyed at 0x006A1C87 before the result test). FITTED: the inner block
// with `bool found` that gives that lifetime (an implicit conversion gives the same bytes but calls ??B 0x00415D70).

bool CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation(const CThing& thing, C3DVector& pos, CRightHandedSet& orientation,
                                                               bool interpolate, float time) const
{
    CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >::const_iterator it =
        CarrySlots.ValFind(&thing);
    if (it == CarrySlots.end())
        return false;
    CDefPointer<const CCarrySlotDef> def;
    if (!CGameDefinitionManager::Get().CDefinitionManager::GetDef(it->first, &def))
        return false;
    CCharString dummy_name = def->DummyPosName;
    if (!GetParentThing().GetTCGraphicAppearance())
        return false;
    unsigned long flags = 0x10;
    if (!interpolate)
        flags = 0x31;
    if (!GetParentThing().GetTCGraphicAppearance()->GetDummyObject(dummy_name, 0, flags, pos, orientation, time)) {
        if (!GetParentThing().GetPhysicsTC())
            return false;
        pos = GetParentThing().GetPhysicsTC()->GetPosition(time);
        const CTCPhysicsBase* physics = GetParentThing().GetPhysicsTC();
        orientation.Init(C3DVector(0.0f, 0.0f, 1.0f), physics->GetFacingVector(time));
    }
    if (GetParentThing().GetType() == 2 && IsSlotPrimarySlot(it->first) && g_bTurnPrimarySlotCarriedThing)
        orientation.Rotate(orientation.GetUp(), 0.5f);
    if (def->SecondaryDummyPosName != "") {
        const CTCCreatureModeManager* modes;
        if (GetParentThing().GetType() == 2 && GetParentThing().PeekTC(&modes) && modes->GetCurrentMode().IsHoldWeaponWithBothHands() &&
            !static_cast<const CThingCreatureBase&>(GetParentThing()).IsPerformingAnyAction()) {
            C3DVector secondary_pos;
            CRightHandedSet secondary_orientation;
            bool found;
            {
                CCharString secondary_name = def->SecondaryDummyPosName.GetString();
                found = GetParentThing().GetTCGraphicAppearance()->GetDummyObject(secondary_name, 0, flags, secondary_pos,
                                                                                  secondary_orientation, time);
            }
            if (found) {
                C3DVector direction = pos - secondary_pos;
                if (direction.IsNonZero()) {
                    direction.AccurateNormalise();
                    orientation.Rotate(direction ^ orientation.GetUp(), 0.2f);
                }
            }
        }
    }
    return true;
}
