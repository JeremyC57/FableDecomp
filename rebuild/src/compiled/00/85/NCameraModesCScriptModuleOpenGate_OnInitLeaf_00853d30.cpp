// 00853d30 NCameraModes::CScriptModuleOpenGate::OnInitLeaf
// Leaf: ?OnInitLeaf@CScriptModuleOpenGate@NCameraModes@@MAEXXZ
// Flags: none
#include "fable_compiler.h"
#include <math.h>
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEOPENGATE_PASS26
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/C3DVector.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CScriptModuleOpenGate.h"


namespace NCameraModes {
void CScriptModuleOpenGate::OnInitLeaf()
{
    if (IS_THING_ALIVE(PGate)) {
        C3DVector gate_to_creature = GetOwner().GetObservedThing()->GetPos() - PGate->GetPos();
        gate_to_creature.Z = 0.0f;
        C3DVector axis_y = PGate->GetPhysicsTC()->GetFacingVector();
        axis_y.Z = 0.0f;
        axis_y.AccurateNormalise();
        if (axis_y.Dot(gate_to_creature) < 0.0f) axis_y *= -1.0f;
        C3DVector axis_x = axis_y.CrossWithUp();
        if (axis_x.Dot(gate_to_creature) > 0.0f) axis_x *= -1.0f;
        const float AWAY_FROM_GATE = 1.5f;
        const float OFF_GATE_AXIS = 1.5f;
        const float START_HEIGHT = 0.75f;
        C3DVector offset = axis_y * AWAY_FROM_GATE + axis_x * OFF_GATE_AXIS + C3DVector(0, 0, START_HEIGHT);
        Pos = PGate->GetPos() + offset;
        C3DVector end_pos = PGate->GetPos() + offset * 2.0f;
        DeltaPos = (end_pos - Pos) / (float)GetDuration();
    }
}
}