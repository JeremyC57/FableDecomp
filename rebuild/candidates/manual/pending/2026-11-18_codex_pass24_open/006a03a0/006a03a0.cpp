// CCreatureAction_UseObject::Initialise() -- select the nearest authored action dummy or a bounding approach point.
// Grade: DIFFER(855v825) -- The shared action and graphic classes use the original dummy lookup and bounding query. The animation scan, registry calls, and x87 evaluation order still differ.
// Leaf: ?Initialise@CCreatureAction_UseObject@@UAEXXZ
// Retail: 0x006A03A0, 825 bytes, thiscall.
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PERSONALSPACE_PASS24
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT2
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_USEOBJECT_PASS24
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_UseObject.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern const CCharString ACTION_POINT;
float FABLE_FASTCALL GFGet2DDistance(const C2DCoordI& first, const C2DCoordI& second);

void CCreatureAction_UseObject::Initialise()
{
    CCreatureActionBase::Initialise();
    TotalTime = -9;
    ActionTime = -9;
    std::vector<CCharString> dummy_object_names;
    NGraphicAppearance::CTCGraphicAppearance* ptc_graph = PUsedThing->GetTCGraphicAppearance();
    bool init_success = false;
    if (ptc_graph) {
        ptc_graph->GetDummyObjectNamesStartingWith(dummy_object_names, ACTION_POINT);
        std::sort(dummy_object_names.begin(), dummy_object_names.end());
        if (!dummy_object_names.empty()) {
            long index = 0;
            ptc_graph->GetDummyObject(dummy_object_names[0], index, 0, DummyPos, Dummy, 1.0f);
            for (long c_name = 1; c_name < static_cast<long>(dummy_object_names.size()); ++c_name) {
                if (dummy_object_names[c_name] == dummy_object_names[c_name - 1])
                    ++index;
                else
                    index = 0;
                C3DVector pos;
                CRightHandedSet rhset;
                ptc_graph->GetDummyObject(dummy_object_names[c_name], index, 0, pos, rhset, 1.0f);
                if ((C2DVector(pos) - C2DVector(PCreature->GetPos())).GetSquaredMagnitude() <
                    (C2DVector(DummyPos) - C2DVector(PCreature->GetPos())).GetSquaredMagnitude()) {
                    DummyPos = pos;
                    Dummy = rhset;
                }
            }
            init_success = true;
        }
    }
    if (!init_success) {
        PUsedThing->GetPhysicsTC()->GetBoundingInfo(DummyPos, Proximity);
        Proximity += PCreature->GetPersonalSpaceRadius() * 2.0f;
    }
    UsingDummy = true;
    if (GFGet2DDistance(DummyPos.ToC2DCoordI(), PCreature->GetPos().ToC2DCoordI()) <
        PCreature->GetPersonalSpaceRadius() * 2.0f) {
        if (fabs(DummyPos.Z - PCreature->GetPos().Z) < 2.5f) {
            MovingToDummy = false;
            if (Proximity == 0.0f)
                TurningToDummy = AllowTurn;
        } else {
            MovingToDummy = true;
            TurningToDummy = false;
        }
    } else {
        MovingToDummy = true;
        TurningToDummy = false;
    }
    MovingToDummy = false;
    TurningToDummy = false;
    PreviousPhysicsComponent = 0;
}
