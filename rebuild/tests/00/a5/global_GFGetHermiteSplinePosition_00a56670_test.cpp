// Self-contained behavioural checks of the submitted Hermite function.
#include <stdio.h>
#include <math.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#define FABLE_VECTOR_INLINE inline   // test copy: plain inline (no bare __forceinline in tests)
#else
#define FABLE_FASTCALL
#define FABLE_VECTOR_INLINE inline
#endif
// C3DVector arithmetic copied from the supplied shared header; unused members omitted.
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
};
void FABLE_FASTCALL GFGetHermiteSplinePosition(C3DVector& dest, const C3DVector& from_pos, const C3DVector& from_tan,
                                           const C3DVector& to_pos, const C3DVector& to_tan, float s)
{
    float s2 = s * s;
    float s3 = s2 * s;
    float wpos_from = 2.0f * s3 - 3.0f * s2 + 1.0f;
    float wpos_to = 3.0f * s2 - 2.0f * s3;
    float wtan_from = s3 - 2.0f * s2 + s;
    float wtan_to = s3 - s2;
    dest = from_pos * wpos_from + to_pos * wpos_to + from_tan * wtan_from + to_tan * wtan_to;
}

static int failures=0;
static void CheckVector(const C3DVector& actual,const C3DVector& expected) {
    if(actual.X!=expected.X || actual.Y!=expected.Y || actual.Z!=expected.Z) ++failures;
}
int main() {
    C3DVector first(2.0f,-4.0f,8.0f), last(10.0f,12.0f,-8.0f);
    C3DVector tangentFirst(4.0f,8.0f,-12.0f), tangentLast(-4.0f,0.0f,4.0f), actual;
    GFGetHermiteSplinePosition(actual,first,tangentFirst,last,tangentLast,0.0f); CheckVector(actual,first);
    GFGetHermiteSplinePosition(actual,first,tangentFirst,last,tangentLast,1.0f); CheckVector(actual,last);
    GFGetHermiteSplinePosition(actual,first,tangentFirst,last,tangentLast,0.5f);
    CheckVector(actual,C3DVector(7.0f,5.0f,-2.0f));
    // Independent cubic basis checks use binary fractions, exact at these small magnitudes.
    for(int step=-4;step<=8;++step) {
        float parameter=step*0.25f;
        double t=parameter, hFirst=2*t*t*t-3*t*t+1, hLast=-2*t*t*t+3*t*t;
        double hTangentFirst=t*t*t-2*t*t+t, hTangentLast=t*t*t-t*t;
        C3DVector expected((float)(hFirst*first.X+hLast*last.X+hTangentFirst*tangentFirst.X+hTangentLast*tangentLast.X),
                           (float)(hFirst*first.Y+hLast*last.Y+hTangentFirst*tangentFirst.Y+hTangentLast*tangentLast.Y),
                           (float)(hFirst*first.Z+hLast*last.Z+hTangentFirst*tangentFirst.Z+hTangentLast*tangentLast.Z));
        GFGetHermiteSplinePosition(actual,first,tangentFirst,last,tangentLast,parameter);CheckVector(actual,expected);
        C3DVector aliasFirst(first),aliasLast(last),aliasTangentFirst(tangentFirst),aliasTangentLast(tangentLast);
        GFGetHermiteSplinePosition(aliasFirst,aliasFirst,tangentFirst,last,tangentLast,parameter);CheckVector(aliasFirst,expected);
        GFGetHermiteSplinePosition(aliasLast,first,tangentFirst,aliasLast,tangentLast,parameter);CheckVector(aliasLast,expected);
        GFGetHermiteSplinePosition(aliasTangentFirst,first,aliasTangentFirst,last,tangentLast,parameter);CheckVector(aliasTangentFirst,expected);
        GFGetHermiteSplinePosition(aliasTangentLast,first,tangentFirst,last,aliasTangentLast,parameter);CheckVector(aliasTangentLast,expected);
    }
    if(failures) {printf("FAIL %d\n",failures);return 1;}
    puts("00a56670_TEST PASS");return 0;
}