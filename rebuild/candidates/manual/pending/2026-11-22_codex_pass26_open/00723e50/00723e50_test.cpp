#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __fastcall
#endif
// The selected method is compiled with the shared vectors and deterministic physics adapters.
// CTCPhysicsStandard::SetRHSetAsFlushOnNormal; original xy/right_2d/forward locals and source-line sequence.
// Leaf: ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z
// Flags: (none)
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26
// Inferred: retail subtracts the exact single-precision quarter-turn at 0x00723E6D.
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
#define FABLE_RETAIL_C3DVECTOR_ANGLE_FORMALS_MAGNITUDE_PASS26
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#pragma intrinsic(sin,cos)
#endif
#include <cstdio>
#include <cmath>
#include <cstring>
static int failures=0;
#define CHECK(x) do {if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
unsigned long GFastOneOverSqrtTable[128];
static void InitReciprocalFixture(){
 for(unsigned i=0;i<128;++i){float n=(i<64?2.0f:1.0f)*(1.0f+((i&63)+0.5f)/64.0f);float r=1.0f/(float)std::sqrt(n);unsigned bits;std::memcpy(&bits,&r,4);GFastOneOverSqrtTable[i]=(bits&0x00ffffffu)>>15;}
}
static bool Near(float a,float b,float eps=0.002f){return std::fabs(a-b)<eps;}

static void CheckReciprocalSeed()
{
    const unsigned exponents[] = {37, 38, 125, 126, 127, 128, 216, 217};
    for (unsigned e = 0; e < sizeof(exponents) / sizeof(exponents[0]); ++e) {
        for (unsigned mantissa = 0; mantissa < 64; ++mantissa) {
            const unsigned bits = (exponents[e] << 23) | (mantissa << 17) | 0x7fffu;
            float value;
            std::memcpy(&value, &bits, sizeof(value));
            const float expected = 1.0f / (float)std::sqrt(value);
            CHECK(Near(GFFastOneOverSqrt(value) / expected, 1.0f, 0.001f));
        }
    }
}

struct FixturePhysics {
 CRightHandedSet RHSet,OldRHSet;float angle;int updates;mutable int angle_calls;
 explicit FixturePhysics(float a):angle(a),updates(0),angle_calls(0) {RHSet.SetUp(C3DVector(0,0,1));RHSet.SetForward(C3DVector(0,1,0));}
 void UpdateOldRHSet(){CHECK(updates==0&&angle_calls==0);++updates;OldRHSet=RHSet;}
 float GetFacingAngleXY()const{CHECK(updates==1&&angle_calls==0);++angle_calls;return angle;}
 void SetRHSetAsFlushOnNormal(const C3DVector& normal);
};
#define CTCPhysicsStandard FixturePhysics
void CTCPhysicsStandard::SetRHSetAsFlushOnNormal(const C3DVector& normal)
{
    UpdateOldRHSet();
    float xy = GetFacingAngleXY();
    C3DVector right_2d;
    right_2d.InitialiseFromAngles((xy - 0.25f), 0.0f, 1.0f);
    C3DVector forward = right_2d ^ normal;
    forward.Normalise();
    RHSet.SetForward(forward);
    RHSet.SetUp(normal);
}

#undef CTCPhysicsStandard
static bool SameVector(const C3DVector& a, const C3DVector& b)
{
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

static void CheckNormalAliases(float angle)
{
    for (unsigned which = 0; which != 4; ++which) {
        FixturePhysics reference(angle), actual(angle);
        actual.RHSet.SetForward(C3DVector(0.25f, 0.75f, 1.0f));
        actual.RHSet.SetUp(C3DVector(0.5f, 1.0f, 0.25f));
        reference.RHSet = actual.RHSet;
        actual.OldRHSet.SetForward(C3DVector(2.0f, 1.0f, 0.0f));
        actual.OldRHSet.SetUp(C3DVector(1.0f, 0.0f, 2.0f));
        reference.OldRHSet = actual.OldRHSet;
        const C3DVector initial = which % 2 == 0
            ? actual.RHSet.GetForward() : actual.RHSet.GetUp();
        const CRightHandedSet& owner = which < 2 ? actual.RHSet : actual.OldRHSet;
        const C3DVector& live = which % 2 == 0 ? owner.GetForward() : owner.GetUp();
        reference.SetRHSetAsFlushOnNormal(initial);
        actual.SetRHSetAsFlushOnNormal(live);
        CHECK(actual.updates == 1 && actual.angle_calls == 1);
        CHECK(SameVector(actual.RHSet.GetForward(), reference.RHSet.GetForward()));
        const C3DVector& expectedUp = which == 0 ? reference.RHSet.GetForward() : initial;
        CHECK(SameVector(actual.RHSet.GetUp(), expectedUp));
        CHECK(SameVector(actual.OldRHSet.GetForward(), reference.OldRHSet.GetForward()));
        CHECK(SameVector(actual.OldRHSet.GetUp(), reference.OldRHSet.GetUp()));
    }
}

int main(){InitReciprocalFixture();CheckReciprocalSeed();const float angles[]={0,.125f,.25f,.5f,.75f,-.125f};const C3DVector normals[]={C3DVector(0,0,1),C3DVector(.6f,0,.8f),C3DVector(0,.8f,.6f),C3DVector(.3f,.4f,1.5f)};
 for(unsigned a=0;a<6;++a) CheckNormalAliases(angles[a]);
 for(unsigned a=0;a<6;++a)for(unsigned n=0;n<4;++n){FixturePhysics p(angles[a]);p.SetRHSetAsFlushOnNormal(normals[n]);const C3DVector& f=p.RHSet.GetForward();const C3DVector& up=p.RHSet.GetUp();
 CHECK(p.updates==1&&p.angle_calls==1);CHECK(up.X==normals[n].X&&up.Y==normals[n].Y&&up.Z==normals[n].Z);CHECK(Near(f.X*f.X+f.Y*f.Y+f.Z*f.Z,1));CHECK(Near(f.X*up.X+f.Y*up.Y+f.Z*up.Z,0,.00001f));
 CHECK(p.OldRHSet.GetForward().X==0&&p.OldRHSet.GetForward().Y==1&&p.OldRHSet.GetUp().Z==1);
 if(n==0){CHECK(Near(f.X,(float)std::sin(angles[a]*6.2831855f)));CHECK(Near(f.Y,(float)std::cos(angles[a]*6.2831855f)));}}
 {FixturePhysics p(0);p.SetRHSetAsFlushOnNormal(C3DVector(0,0,0));CHECK(p.RHSet.GetForward().X==0&&p.RHSet.GetForward().Y==0&&p.RHSet.GetForward().Z==0);}
 std::puts(failures?"00723e50_TEST FAIL":"00723e50_TEST PASS");return failures?1:0;}
