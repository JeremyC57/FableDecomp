#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#include "engine/C3DVector.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

const float DELTA_NOTIONAL_ZERO=0.0001f;
unsigned long GFastOneOverSqrtTable[128];

static void InitialiseFixtureSqrtTable()
{
    for (unsigned i=0; i<128; ++i) {
        unsigned bits=(126U<<23)+(i<<17)+0x10000U;
        float input;
        std::memcpy(&input,&bits,4);
        float estimate=1.0f/std::sqrt(input);
        std::memcpy(&bits,&estimate,4);
        GFastOneOverSqrtTable[i]=(bits&0x7fffffU)>>15;
    }
}
static unsigned rotates=0;
void C3DVector::Rotate(float first,float second)
{
    assert(first==0.0f && second==0.25f);
    ++rotates;
    *this=C3DVector(-Y,X,Z);
}
class CRightHandedSet
{
public:
    C3DVector up, forward;
    CRightHandedSet(const C3DVector& u,const C3DVector& f) : up(u), forward(f) { }
};
struct PhysicsFixture
{
    C3DVector up, forward;
    unsigned updates;
    PhysicsFixture() : updates(0) { }
    void SetRHSet(const CRightHandedSet& set)
    { up=set.up; forward=set.forward; ++updates; }
};
struct CThingTrackNode
{
    const CThingTrackNode* NextNode;
    const CThingTrackNode* PreviousNode;
    C3DVector pos;
    PhysicsFixture physics;

    CThingTrackNode() : NextNode(0), PreviousNode(0), pos(0,0,0) { }
    const CThingTrackNode* GetNextNode() const { return NextNode; }
    const CThingTrackNode* GetPreviousNode() const { return PreviousNode; }
    const C3DVector& PeekPos() const { return pos; }
    PhysicsFixture* GetPhysicsTC() { return &physics; }
    void UpdateFacingDirection();
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CThingTrackNode::UpdateFacingDirection()
{
    C3DVector forward(0.0f, 0.0f, 0.0f);
    if (GetNextNode())
        forward = GetNextNode()->PeekPos() - PeekPos();
    else if (GetPreviousNode())
        forward = PeekPos() - GetPreviousNode()->PeekPos();
    if (forward.IsNonZero())
    {
        forward.Rotate(0.0f, 0.25f);
        forward.Normalise();
        C3DVector right(forward.Y, -forward.X, 0.0f);
        right.Normalise();
        C3DVector up = right ^ forward;
        up.Normalise();
        GetPhysicsTC()->SetRHSet(CRightHandedSet(up, forward));
    }
}

int main()
{
    InitialiseFixtureSqrtTable();
    CThingTrackNode node, next, previous;
    next.pos = C3DVector(2, 0, 0);
    previous.pos = C3DVector(0, -5, 0);
    node.UpdateFacingDirection();
    assert(rotates == 0 && node.physics.updates == 0);

    node.PreviousNode = &previous;
    node.UpdateFacingDirection();
    assert(rotates == 1 && node.physics.updates == 1);
    assert(std::fabs(node.physics.forward.X+1)<0.01f && node.physics.forward.Y==0);
    assert(std::fabs(node.physics.up.Z-1)<0.01f);

    node.NextNode = &next;
    node.UpdateFacingDirection();
    assert(rotates == 2 && node.physics.updates == 2);
    assert(node.physics.forward.X==0 && std::fabs(node.physics.forward.Y-1)<0.01f);
    assert(std::fabs(node.physics.up.Z-1)<0.01f);

    next.pos = node.pos;
    node.UpdateFacingDirection();
    assert(rotates == 2 && node.physics.updates == 2);
    std::puts("00837220_TEST PASS");
    return 0;
}
