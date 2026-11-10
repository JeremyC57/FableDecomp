#include <stdio.h>
#include <math.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
// These math bodies are copied from the supplied shared headers.
class C3DVector { public:
 float X,Y,Z;
 C3DVector() {}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
 const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
};
class CRightHandedSet { protected: C3DVector Up,Forward; public:
 const C3DVector& GetForward() const { return Forward; }
 const C3DVector& GetUp() const { return Up; }
 C3DVector GetRight() const { return Forward ^ Up; }
 void SetForward(const C3DVector& forward) { Forward = forward; }
 void SetUp(const C3DVector& up) { Up = up; }
};
class CMatrix3x4 { public:
 float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
 CMatrix3x4() {}
};
class C3DMeshObject {
public:
    static CMatrix3x4 FABLE_FASTCALL GetPositionMatrixFromRHSet(const CRightHandedSet& set);
};
CMatrix3x4 FABLE_FASTCALL C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set)
{
    C3DVector up(set.GetUp());
    C3DVector forward(set.GetForward());
    const C3DVector& right=set.GetRight();
    CMatrix3x4 m;
    m.E11=-right.X; m.E12=-right.Y; m.E13=-right.Z;
    m.E21=-forward.X; m.E22=-forward.Y; m.E23=-forward.Z;
    m.E31=up.X; m.E32=up.Y; m.E33=up.Z;
    m.E41=m.E42=m.E43=0.0f;
    return m;
}

static int Check(const C3DVector& up, const C3DVector& forward) {
 CRightHandedSet set;
 set.SetUp(up); set.SetForward(forward);
 const CMatrix3x4 m=C3DMeshObject::GetPositionMatrixFromRHSet(set);
 const float actual[]={m.E11,m.E12,m.E13,m.E21,m.E22,m.E23,m.E31,m.E32,m.E33,m.E41,m.E42,m.E43};
 const double expected[]={
   (double)forward.Z*up.Y-(double)forward.Y*up.Z,
   (double)forward.X*up.Z-(double)forward.Z*up.X,
   (double)forward.Y*up.X-(double)forward.X*up.Y,
   -forward.X,-forward.Y,-forward.Z,up.X,up.Y,up.Z,0.0,0.0,0.0};
 for(int i=0;i<12;++i) if(actual[i]!=expected[i]) return 1;
 if(set.GetForward().X!=forward.X || set.GetForward().Y!=forward.Y || set.GetForward().Z!=forward.Z) return 2;
 if(set.GetUp().X!=up.X || set.GetUp().Y!=up.Y || set.GetUp().Z!=up.Z) return 3;
 return 0;
}
int main() {
 const float v[][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1},{0.5f,-2.0f,4.0f},{-0.25f,1.5f,2.0f},{8.0f,-4.0f,0.125f}};
 for(int i=0;i<10;++i) for(int j=0;j<10;++j)
   if(Check(C3DVector(v[i][0],v[i][1],v[i][2]),C3DVector(v[j][0],v[j][1],v[j][2]))) return 1;
 puts("00a358f0_TEST PASS");
 return 0;
}