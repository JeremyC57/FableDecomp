#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
// 00bebaa0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix
// Leaf: ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z
// Flags:
// Layout: original matrix48 E11..E43; protected set Up0 Forward12; vector XYZ12. No shared headers supplied.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    const C3DVector Cross(const C3DVector&b)const{const C3DVector&c=b;float x=Y*c.Z;x-=Z*c.Y;float y=Z*c.X;y-=X*c.Z;float z=X*b.Y;z-=Y*b.X;return C3DVector(x,y,z);}
    const C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
public:
    CRightHandedSet(const C3DVector&u,const C3DVector&f):Up(u),Forward(f){}
    const C3DVector&GetForward()const{return Forward;}const C3DVector&GetUp()const{return Up;}
    C3DVector GetRight() const { return Forward.Cross(Up); }   // as landed 0x0049CE00
};
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void SetTranslateOnly(const C3DVector& t) { E41 = t.X; E42 = t.Y; E43 = t.Z; }
};
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale);
};

void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.GetForward();
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.GetUp();
    right *= scale;
    forward *= scale;
    up *= scale;
    matrix.E11 = -right.X;
    matrix.E12 = -right.Y;
    matrix.E13 = -right.Z;
    matrix.E21 = -forward.X;
    matrix.E22 = -forward.Y;
    matrix.E23 = -forward.Z;
    matrix.E31 = up.X;
    matrix.E32 = up.Y;
    matrix.E33 = up.Z;
    matrix.SetTranslateOnly(pos);
}

int main(){int cases=0;for(int n=0;n<512;++n){
 float ux=float((n%7)-3)*0.5f,uy=float(((n/7)%7)-3)*0.5f,uz=float(((n/49)%7)-3)*0.5f;
 float fx=float(((n*3)%11)-5)*0.25f,fy=float(((n*5)%11)-5)*0.25f,fz=float(((n*7)%11)-5)*0.25f;
 float scale=float((n%9)-4)*0.5f;C3DVector up(ux,uy,uz),forward(fx,fy,fz),pos(float(n%17)*0.25f,float(-(n%13))*0.5f,float(n%19)*0.25f);CRightHandedSet axes(up,forward);CMatrix3x4 out;
 out.E11=out.E12=out.E13=out.E21=out.E22=out.E23=out.E31=out.E32=out.E33=out.E41=out.E42=out.E43=12345.0f;
 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(out,pos,axes,scale);
 CHECK(out.E11==-(fy*uz-fz*uy)*scale&&out.E12==-(fz*ux-fx*uz)*scale&&out.E13==-(fx*uy-fy*ux)*scale);
 CHECK(out.E21==-fx*scale&&out.E22==-fy*scale&&out.E23==-fz*scale);
 CHECK(out.E31==ux*scale&&out.E32==uy*scale&&out.E33==uz*scale);
 CHECK(out.E41==pos.X&&out.E42==pos.Y&&out.E43==pos.Z);++cases;
 }CHECK(cases==512);puts("00bebaa0_TEST PASS");return 0;}
