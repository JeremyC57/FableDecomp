#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
// 006e7f00 GetAttackerProngSelectionCompareValue
// Leaf: ?GetAttackerProngSelectionCompareValue@?A0x
// Flags:
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#pragma pack(push,1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(pop)
namespace {
float __fastcall GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.X * flatPlacement.X + flatPlacement.Y * flatPlacement.Y + flatPlacement.Z * flatPlacement.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    float inverse = 1.0f / (float)sqrt(lengthSquared);
    flatPlacement.X = inverse * flatPlacement.X;
    flatPlacement.Y = inverse * flatPlacement.Y;
    flatPlacement.Z = inverse * flatPlacement.Z;
    lengthSquared = flatProng.X * flatProng.X + flatProng.Y * flatProng.Y + flatProng.Z * flatProng.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    inverse = 1.0f / (float)sqrt(lengthSquared);
    flatProng.X = inverse * flatProng.X;
    flatProng.Y = inverse * flatProng.Y;
    flatProng.Z = inverse * flatProng.Z;
    return flatProng.Z * flatPlacement.Z + (flatProng.X * flatPlacement.X + flatProng.Y * flatPlacement.Y) + 1.0f;
}
}
// Translation-unit retention pointer; an implementation-only test/build artifact.
float (__fastcall *implementation_entry_006e7f00)(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&) = GetAttackerProngSelectionCompareValue;

static bool check_value(const C3DVector& p,const C3DVector& q)
{
    C3DVector arbitraryA(7.0f,-2.0f,31.0f), arbitraryB(-4.0f,8.0f,-16.0f);
    double ps=(double)p.X*p.X+(double)p.Y*p.Y;
    double qs=(double)q.X*q.X+(double)q.Y*q.Y;
    double expected;
    if(ps < 0.0001f || qs < 0.0001f) expected=0.0001f;
    else expected=1.0+((double)p.X*q.X+(double)p.Y*q.Y)/sqrt(ps*qs);
    double actual=GetAttackerProngSelectionCompareValue(arbitraryA,arbitraryB,p,q);
    double delta=actual-expected;
    if(delta<0) delta=-delta;
    return delta<0.000002;
}
int main()
{
    unsigned long cases=0;
    int x,y,u,v;
    for(x=-4;x<=4;x+=2) for(y=-4;y<=4;y+=2)
    for(u=-4;u<=4;u+=2) for(v=-4;v<=4;v+=2) {
        C3DVector a((float)x,(float)y,16.0f),b((float)u,(float)v,-32.0f);
        if(!check_value(a,b)) return 1;
        ++cases;
    }
    C3DVector tiny(0.00390625f,0.0f,8.0f),large(2.0f,0.0f,0.0f);
    if(!check_value(tiny,large)||!check_value(large,tiny)) return 2;
    C3DVector small(0.015625f,0.0f,4.0f);
    if(!check_value(small,large)||!check_value(large,small)) return 3;
    if(cases!=625 || sizeof(C3DVector)!=12) return 4;
    puts("006e7f00_TEST PASS");
    return 0;
}