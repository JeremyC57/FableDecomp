// 00a18b50 CPhysicsMesh::HasCollidedWithTriangleAt
// Leaf: ?HasCollidedWithTriangleAt@CPhysicsMesh@@ABE_NABVC3DTriangle2@@MABVC3DVector@@1MAAV3@2@Z
// Flags:
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_CPHYSICSMESH_SWEEP_TRIANGLE_PASS30
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF

#define FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CPhysicsMesh.h"
#include "engine/CSphere.h"

bool CPhysicsMesh::HasCollidedWithTriangleAt(
    const C3DTriangle2& tri, float sphere_radius, const C3DVector& from,
    const C3DVector& normalised_velocity, float speed, C3DVector& intersection_pos,
    C3DVector& intersection_normal) const
{
    C3DVector sphere_intersection;
    const C3DVector& a = TotalPoints[tri.GetA()];
    const C3DVector& b = TotalPoints[tri.GetB()];
    const C3DVector& c = TotalPoints[tri.GetC()];
    C3DVector b_a = b - a;
    C3DVector c_a = c - a;
    C3DVector p_normal = b_a.Cross(c_a);
    float cos_angle = GFDotProduct(p_normal, normalised_velocity);
    if (cos_angle > 0.0f) {
        C3DVector to = from + normalised_velocity * speed;
        long no_divides = static_cast<long>(speed / sphere_radius) + 1;
        C3DVector next_pos = no_divides == 1 ? to : from;
        C3DVector add_pos = (normalised_velocity * speed) / static_cast<float>(no_divides);
        for (long c1 = 0; c1 < no_divides; ++c1) {
            float distance_sqr = NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(next_pos, sphere_radius, a, b_a, c_a);
            if (distance_sqr < GFSqr(sphere_radius)) {
                intersection_normal = p_normal;
                intersection_normal.Normalise();
                intersection_pos = from + normalised_velocity * GFRoot(distance_sqr);
                return true;
            }
            if (c1 == no_divides - 1)
                next_pos = to;
            else
                next_pos += add_pos;
        }
    }
    return false;
}

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned long GFastOneOverSqrtTable[128];
static int exercised;
static void check(bool condition) { if (!condition) abort(); }
void CIVCountedPointeeBase::Delete() const { abort(); }
typedef bool (CPhysicsMesh::*SweepMember)(const C3DTriangle2&,float,const C3DVector&,const C3DVector&,float,C3DVector&,C3DVector&) const;

static void InitialPoints(CArray<C3DVector>& points,const C3DVector& b,const C3DVector& c)
{
    points.resize(5);
    points[0]=C3DVector(19,17,13);
    points[1]=C3DVector(0,0,0);
    points[2]=b;
    points[3]=C3DVector(23,29,31);
    points[4]=c;
}

static void InitialiseTable() {
 check(sizeof(unsigned int)==4 && sizeof(float)==4);
 // Generate the seed table mathematically, following original initializer00a0db60.
 for(unsigned int i=0;i<128;++i){
  unsigned int bits=(i|0x1f80u)<<17;float value;
  memcpy(&value,&bits,4);float inverse=(float)(1.0/sqrt((double)value));
  memcpy(&bits,&inverse,4);GFastOneOverSqrtTable[i]=((bits+0x2000u)>>15)&0xffu;
 }
 // Original initializer00a0dbb0 patches the power-of-two seed.
 GFastOneOverSqrtTable[64]=255;
}
static bool Near(float actual,float expected)
{
    return fabs(actual-expected)<0.0002;
}

static bool SameVector(const C3DVector& a,const C3DVector& b)
{
    return Near(a.X,b.X) && Near(a.Y,b.Y) && Near(a.Z,b.Z);
}

namespace {
    enum { Capacity=5 };
    C3DVector Seen[Capacity];
    C3DVector ExpectedA(0,0,0);
    C3DVector ExpectedB;
    C3DVector ExpectedC;
    float Distances[Capacity];
    float Radius;
    int SampleCount;
    int SampleLimit;
}

namespace NBBBMaths {
float FABLE_FASTCALL GetSquaredDistanceBetweenSphereTriangle(const C3DVector& sample,float radius,
    const C3DVector& a,const C3DVector& b_a,const C3DVector& c_a)
{
    check(SampleCount<SampleLimit);
    check(radius==Radius);
    check(SameVector(a,ExpectedA));
    check(SameVector(b_a,ExpectedB));
    check(SameVector(c_a,ExpectedC));
    Seen[SampleCount]=sample;
    return Distances[SampleCount++];
}
}

static void CheckSweep(SweepMember sweep,const CPhysicsMesh& mesh,const C3DTriangle2& tri,const C3DVector& velocity,
                       float speed,float radius,const float* distances,const float* expected_steps,
                       int samples,bool hit,float intersection_distance,const C3DVector& expected_normal)
{
    SampleCount=0;
    SampleLimit=samples;
    Radius=radius;
    for (int index=0;index<samples;++index)
        Distances[index]=distances[index];
    const C3DVector from(1,1,-2);
    const C3DVector untouched(6,7,8);
    C3DVector position=untouched;
    C3DVector normal=untouched;
    check((mesh.*sweep)(tri,radius,from,velocity,speed,position,normal)==hit);
    check(SampleCount==samples);
    for (int sample=0;sample<samples;++sample)
        check(SameVector(Seen[sample],from+velocity*expected_steps[sample]));
    if (hit) {
        check(SameVector(position,from+velocity*intersection_distance));
        check(SameVector(normal,expected_normal));
    } else {
        check(SameVector(position,untouched));
        check(SameVector(normal,untouched));
    }

}

// This test-only destructor can call the private overload on a fully constructed mesh.
CPhysicsMesh::~CPhysicsMesh()
{
    InitialiseTable();
    check(GFastOneOverSqrtTable[64]==255);
    check(GFastOneOverSqrtTable[1]!=GFastOneOverSqrtTable[100]);
    check(sizeof(CPhysicsMesh)==0xc0);
    CPhysicsMesh& mesh = *this;
    check((reinterpret_cast<const char*>(&TotalPoints)-reinterpret_cast<const char*>(this))==0x3c);
    C3DTriangle2 tri;
    tri.SetVertexIndex(0,1);
    tri.SetVertexIndex(1,2);
    tri.SetVertexIndex(2,4);

    ExpectedB=C3DVector(4,0,0);
    ExpectedC=C3DVector(0,3,0);
    InitialPoints(TotalPoints,ExpectedB,ExpectedC);
    const C3DVector forward(0,0,1);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,C3DVector(0,0,-1),2,1,0,0,0,false,0,forward);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,C3DVector(1,0,0),2,1,0,0,0,false,0,forward);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,-2,1,0,0,0,false,0,forward);

    const float single_step[]={0.5f};
    const float single_hit[]={0.25f};
    const float single_boundary[]={1.0f};
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,0.5f,1,single_hit,single_step,1,true,0.5f,forward);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,0.5f,1,single_boundary,single_step,1,false,0,forward);

    const float steps[]={0,2.0f/3.0f,4.0f/3.0f};
    const float first_hit[]={0.0f};
    const float later_hit[]={1.25f,0.25f};
    const float no_hit[]={2.0f,3.0f,1.0f};
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,2,1,first_hit,steps,1,true,0,forward);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,2,1,later_hit,steps,2,true,0.5f,forward);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,forward,2,1,no_hit,steps,3,false,0,forward);

    ExpectedB=C3DVector(1,-1,0);
    ExpectedC=C3DVector(1,1,-2);
    InitialPoints(TotalPoints,ExpectedB,ExpectedC);
    const float component=static_cast<float>(1.0/sqrt(3.0));
    const C3DVector diagonal(component,component,component);
    CheckSweep(&CPhysicsMesh::HasCollidedWithTriangleAt,mesh,tri,diagonal,0.5f,1,single_hit,single_step,1,true,0.5f,diagonal);
    ++exercised;
}

int main()
{
    { CPhysicsMesh mesh; }
    check(exercised==1);
    puts("00a18b50_TEST PASS");
    return 0;
}