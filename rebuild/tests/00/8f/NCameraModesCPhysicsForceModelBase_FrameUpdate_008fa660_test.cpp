// Use the selected translation unit and shared vector arithmetic. The fixture
// supplies six distinct virtual force vectors and records their call order.
#include <assert.h>
#include <stdio.h>
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"

namespace NCameraModes {
class CPhysicsForceModelBase {
public:
    virtual void FrameUpdate();
    virtual C3DVector DoThingEvasion() = 0;
    virtual C3DVector DoThemeEvasion() = 0;
    virtual C3DVector DoGroundEvasion() = 0;
    virtual C3DVector DoCreatureEvasion() = 0;
    virtual C3DVector DoAttractToPoint() = 0;
    virtual C3DVector DoVelocityDamping() = 0;
    C3DVector Published;
    int Updates;
protected:
    void SetAcceleration(const C3DVector& acceleration) {
        Published = acceleration;
        ++Updates;
    }
};

class CPhysicsForceFixture : public CPhysicsForceModelBase {
public:
    CPhysicsForceFixture() : NextForce(0) { Updates = 0; }
    C3DVector DoThingEvasion() { return Consume(0); }
    C3DVector DoGroundEvasion() { return Consume(1); }
    C3DVector DoThemeEvasion() { return Consume(2); }
    C3DVector DoCreatureEvasion() { return Consume(3); }
    C3DVector DoAttractToPoint() { return Consume(4); }
    C3DVector DoVelocityDamping() { return Consume(5); }

    C3DVector Forces[6];
    int NextForce;

private:
    C3DVector Consume(int index) {
        assert(index == NextForce++);
        return Forces[index];
    }
};
}

#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSFORCEMODELBASE_H
// Sum the evasion, point attraction and damping forces.
// Leaf: ?FrameUpdate@CPhysicsForceModelBase@NCameraModes@@UAEXXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPHYSICSFORCEMODELBASE_SET_ACCELERATION_PASS27
#include "engine/NCameraModes_CPhysicsForceModelBase.h"
namespace NCameraModes {

void CPhysicsForceModelBase::FrameUpdate()
{
    C3DVector acceleration(0.0f, 0.0f, 0.0f);
    acceleration += -DoThingEvasion();
    acceleration += -DoGroundEvasion();
    acceleration += -DoThemeEvasion();
    acceleration += -DoCreatureEvasion();
    acceleration += DoAttractToPoint();
    acceleration += -DoVelocityDamping() * 1.0f;
    SetAcceleration(acceleration);
}

}


int main()
{
    NCameraModes::CPhysicsForceFixture fixture;
    fixture.Forces[0] = C3DVector(1.0f, -2.0f, 0.5f);
    fixture.Forces[1] = C3DVector(3.0f, 4.0f, -5.0f);
    fixture.Forces[2] = C3DVector(7.0f, 8.0f, 9.0f);
    fixture.Forces[3] = C3DVector(1.0f, -5.0f, 2.0f);
    fixture.Forces[4] = C3DVector(20.0f, 30.0f, 40.0f);
    fixture.Forces[5] = C3DVector(0.25f, 0.5f, 0.75f);
    fixture.FrameUpdate();
    assert(fixture.NextForce == 6 && fixture.Updates == 1);
    assert(fixture.Published.X == 7.75f);
    assert(fixture.Published.Y == 24.5f);
    assert(fixture.Published.Z == 32.75f);

    for (int i = 0; i != 6; ++i)
        fixture.Forces[i] = C3DVector(0.0f, 0.0f, 0.0f);
    fixture.Forces[4] = C3DVector(-3.0f, 4.0f, 0.0f);
    fixture.NextForce = 0;
    fixture.FrameUpdate();
    assert(fixture.NextForce == 6 && fixture.Updates == 2);
    assert(fixture.Published.X == -3.0f);
    assert(fixture.Published.Y == 4.0f);
    assert(fixture.Published.Z == 0.0f);
    puts("008fa660_TEST PASS");
    return 0;
}