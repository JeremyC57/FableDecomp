// Standalone test: the submitted body and identical shared-header math methods.
#include <math.h>
#include <stdio.h>
#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE inline   // test copy: plain inline (no bare __forceinline in tests)
#pragma intrinsic(atan2, fmod, sin, cos, acos)
#else
#define FABLE_VECTOR_INLINE inline
#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    C3DVector& AccurateNormalise();
    FABLE_VECTOR_INLINE float GetAngleXY() const {
        float angle=(float)atan2(X,Y)*0.15915494f;
        if(angle<0.0f || angle>=1.0f) {
            angle=(float)fmod((double)angle,1.0);
            if(angle<0.0f) angle+=1.0f;
        }
        return angle;
    }
    FABLE_VECTOR_INLINE C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
    FABLE_VECTOR_INLINE const C3DVector& InitialiseFromAngles(float angleXY,float angleYZ,float length) {
        float cosYZ=(float)cos(angleYZ*(double)6.2831855f);
        X=(float)(sin(angleXY*(double)6.2831855f)*(cosYZ*length));
        Y=(float)(cos(angleXY*(double)6.2831855f)*(cosYZ*length));
        Z=(float)(sin(angleYZ*(double)6.2831855f)*length);
        return *this;
    }
};
class CRightHandedSet {
protected:
    C3DVector Up,Forward;
public:
    const C3DVector& GetForward() const{return Forward;}
    const C3DVector& GetUp() const{return Up;}
    C3DVector GetRight()const{return Forward ^ Up;}
    void SetForward(const C3DVector& v){Forward=v;}
    void SetUp(const C3DVector& v){Up=v;}
    float GetAngleXZ() const;
};
float CRightHandedSet::GetAngleXZ() const
{
    C3DVector forward(GetForward().X, GetForward().Y, GetForward().Z);
    C3DVector right = GetRight();
    right.AccurateNormalise();
    C3DVector heading;
    heading.InitialiseFromAngles(forward.GetAngleXY(), 0.0f, 1.0f);
    C3DVector flatRight = heading.CrossWithUp();
    float sign = (float)(right.Z < 0.0f ? -1 : 1);
    flatRight.AccurateNormalise();
    float dot = flatRight.Dot(right);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915494f;
    return angle * sign;
}

// Stub for the separately compiled callee, with its accurate Euclidean normalisation.
static int calls=0;
C3DVector& C3DVector::AccurateNormalise() {
    ++calls;
    float inverse=1.0f/(float)sqrt(X*X+Y*Y+Z*Z);
    X*=inverse;Y*=inverse;Z*=inverse;return *this;
}
static int failures=0;
static void Check(const C3DVector& forward,const C3DVector& up,float expected) {
    CRightHandedSet frame;frame.SetForward(forward);frame.SetUp(up);
    calls=0;float actual=frame.GetAngleXZ();
    if(calls!=2 || fabs(actual-expected)>0.0001f || actual!=actual) {++failures;printf("case failed: %g expected %g\n",actual,expected);}
    if(frame.GetForward().X!=forward.X || frame.GetForward().Y!=forward.Y || frame.GetForward().Z!=forward.Z ||
       frame.GetUp().X!=up.X || frame.GetUp().Y!=up.Y || frame.GetUp().Z!=up.Z) ++failures;
}
int main() {
    Check(C3DVector(0,1,0),C3DVector(0,0,1),0.0f);
    Check(C3DVector(0,1,0),C3DVector(1,0,0),-0.25f);
    Check(C3DVector(0,1,0),C3DVector(-1,0,0),0.25f);
    Check(C3DVector(0,1,0),C3DVector(0,0,-1),0.5f);
    Check(C3DVector(0,1,0),C3DVector(1,0,1),-0.125f);
    Check(C3DVector(0,1,0),C3DVector(-1,0,1),0.125f);
    Check(C3DVector(0,2,0),C3DVector(4,0,4),-0.125f);
    Check(C3DVector(1,0,0),C3DVector(0,0,1),0.0f);
    Check(C3DVector(-1,0,0),C3DVector(0,0,1),0.0f);
    Check(C3DVector(0,-1,0),C3DVector(0,0,1),0.0f);
    if(failures) return 1;
    puts("00a22ca0_TEST PASS");return 0;
}