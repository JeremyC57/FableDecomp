// 008f45f0 CCreatureAction_DragonSwoop::FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_DragonSwoop@@UAEXXZ
// Inline-budget: Retail expands all three AppendControlPoint calls. S79 leaves calls; S80 expands them and all four mapped copies match.
// Flags: /d2inlS80
#define FABLE_RETAIL_DRAGON_SWOOP_PASS29
#define FABLE_RETAIL_CCREATUREACTION_DRAGONSWOOP_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_DragonSwoop.h"
#include "engine/CCardinalSpline.h"

void CCreatureAction_DragonSwoop::FrameUpdate()
{
    CCardinalSpline spline;
    spline.AppendControlPoint(SwoopPoints[0]);
    spline.AppendControlPoint(SwoopPoints[1]);
    spline.AppendControlPoint(SwoopPoints[2]);
    spline.SetTension(0.5f);
    C3DVector pos, delta;
    spline.GetSplinePosition(pos, GetPercentageComplete());
    delta = pos - PCreature->GetPos();
    delta.AccurateNormalise();
    PCreature->GetPhysicsTC()->ResetPosition(pos);
    PCreature->GetPhysicsTC()->SetRHSet(CRightHandedSet(delta.CrossWithUp().Cross(delta), delta));
    CCombatActionBase::FrameUpdate();
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

template<class T> union EngineStorage {
    double aligner;
    unsigned char bytes[sizeof(T)];
    T* pointer() { return reinterpret_cast<T*>(bytes); }
};
static EngineStorage<CCreatureAction_DragonSwoop> actionData;
static EngineStorage<CThingCreatureBase> creatureData;
static void* physicsTable[76];
static void** physicsObject = physicsTable;
static C3DVector initialPos, sampledPos, expectedForward, expectedUp;
static C3DVector expectedPoints[3];
static float progress;
static int sequence, pointerReads;

float CCreatureActionBase::GetPercentageComplete() const
{
    assert(sequence++ == 0);
    return progress;
}
void CCardinalSpline::GetSplinePosition(C3DVector& position, float t) const
{
    assert(sequence++ == 1 && t == progress && Tension == 0.5f);
    assert(ControlPoints.size() == 3);
    for (int i = 0; i < 3; ++i) {
        assert(ControlPoints[i].X == expectedPoints[i].X);
        assert(ControlPoints[i].Y == expectedPoints[i].Y);
        assert(ControlPoints[i].Z == expectedPoints[i].Z);
    }
    position = sampledPos;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    ++pointerReads;
    return creatureData.pointer();
}
const C3DVector& CThing::GetPos() const
{
    assert(sequence++ == 2);
    return initialPos;
}
C3DVector& C3DVector::AccurateNormalise()
{
    assert(sequence++ == 3);
    float length = static_cast<float>(sqrt(X * X + Y * Y + Z * Z));
    if (length != 0) { X /= length; Y /= length; Z /= length; }
    return *this;
}
void CCombatActionBase::FrameUpdate() { assert(sequence++ == 6); }

// Native x86 virtual-call fixture. Instrument only the two slots used by retail.
class PhysicsTrace {
public:
    virtual void Reset(const C3DVector& pos) {
        assert(sequence++ == 4 && pos.X == sampledPos.X && pos.Y == sampledPos.Y && pos.Z == sampledPos.Z);
    }
    virtual void Orientation(const CRightHandedSet& set) {
        assert(sequence++ == 5);
        const C3DVector& f = set.GetForward();
        const C3DVector& u = set.GetUp();
        assert(f.X == expectedForward.X && f.Y == expectedForward.Y && f.Z == expectedForward.Z);
        assert(u.X == expectedUp.X && u.Y == expectedUp.Y && u.Z == expectedUp.Z);
    }
};
int main()
{
    PhysicsTrace trace;
    void** callbacks;
    memcpy(&callbacks, &trace, sizeof(callbacks));
    physicsTable[31] = callbacks[0];
    physicsTable[75] = callbacks[1];
    void* physics = &physicsObject;
    memcpy(creatureData.bytes + 0x60, &physics, sizeof(physics));
    CCreatureAction_DragonSwoop* action = actionData.pointer();
    for (int j = 0; j < 3; ++j) {
        expectedPoints[j] = C3DVector(2.0f*j, 4.0f*j, -8.0f*j);
        action->SwoopPoints[j] = expectedPoints[j];
    }
    initialPos = C3DVector(1.0f, 2.0f, 3.0f);
    for (int i = 0; i < 3; ++i) {
        progress = 0.5f*i;
        expectedForward = i == 0 ? C3DVector(1,0,0) : i == 1 ? C3DVector(0,-1,0) : C3DVector(0,0,1);
        expectedUp = i == 2 ? C3DVector(0,0,0) : C3DVector(0,0,1);
        sampledPos = initialPos + expectedForward * 2.0f;
        sequence = pointerReads = 0;
        action->CCreatureAction_DragonSwoop::FrameUpdate();
        assert(sequence == 7 && pointerReads == 3);
    }
    puts("008f45f0_TEST PASS");
    return 0;
}