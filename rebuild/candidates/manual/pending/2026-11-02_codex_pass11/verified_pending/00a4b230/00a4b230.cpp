// NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(...) -- Squared distance to a triangle.
// Leaf: ?GetSquaredDistanceBetweenSphereTriangle@NBBBMaths@@YIMABVC3DVector@@M000@Z
#include "engine/C3DVector.h"
#include <math.h>
namespace NBBBMaths {
float __fastcall GetSquaredDistanceBetweenSphereTriangle(const C3DVector& SpherePos,float Radius,const C3DVector& Origin,const C3DVector& Edge0,const C3DVector& Edge1)
{
    const C3DVector& E0 = Edge0;
    const C3DVector& E1 = Edge1;
    C3DVector Diff = Origin-SpherePos;
    float A00=Edge0.Dot(Edge0);
    float A01=E0.Dot(E1);
    float A11=Edge1.Dot(Edge1);
    float B0=Diff.Z*Edge0.Z;
    B0+=Diff.Y*Edge0.Y;
    B0+=Diff.X*Edge0.X;
    float B1=Diff.Z*E1.Z;
    B1+=Diff.Y*E1.Y;
    B1+=Diff.X*E1.X;
    float C=Diff.X*Diff.X;
    C+=Diff.Z*Diff.Z;
    C+=Diff.Y*Diff.Y;
    float Det=(float)fabs(A00*A11-A01*A01);
    float S=A01*B1-A11*B0;
    float T=A01*B0-A00*B1;
    float SqrDistance;
    if(S+T<=Det) {
        if(S<0.0f) {
            if(T<0.0f) {
                if(B0<0.0f) {
                    T=0.0f;
                    if(-B0>=A00) { S=1.0f; SqrDistance=A00+2.0f*B0+C; }
                    else { S=-B0/A00; SqrDistance=B0*S+C; }
                } else {
                    S=0.0f;
                    if(B1>=0.0f) { T=0.0f; SqrDistance=C; }
                    else if(-B1>=A11) { T=1.0f; SqrDistance=A11+2.0f*B1+C; }
                    else { T=-B1/A11; SqrDistance=B1*T+C; }
                }
            } else {
                S=0.0f;
                if(B1>=0.0f) { T=0.0f; SqrDistance=C; }
                else if(-B1>=A11) { T=1.0f; SqrDistance=A11+2.0f*B1+C; }
                else { T=-B1/A11; SqrDistance=B1*T+C; }
            }
        } else if(T<0.0f) {
            T=0.0f;
            if(B0>=0.0f) { S=0.0f; SqrDistance=C; }
            else if(-B0>=A00) { S=1.0f; SqrDistance=A00+2.0f*B0+C; }
            else { S=-B0/A00; SqrDistance=B0*S+C; }
        } else {
            float InvDet=1.0f/Det;
            S*=InvDet; T*=InvDet;
            SqrDistance=S*(A00*S+A01*T+2.0f*B0)+T*(A01*S+A11*T+2.0f*B1)+C;
        }
    } else {
        float Tmp0,Tmp1,Numer,Denom;
        if(S<0.0f) {
            Tmp0=A01+B0; Tmp1=A11+B1;
            if(Tmp1>Tmp0) {
                Numer=Tmp1-Tmp0; Denom=A00-2.0f*A01+A11;
                if(Numer>=Denom) { S=1.0f; T=0.0f; SqrDistance=A00+2.0f*B0+C; }
                else {
                    S=Numer/Denom; T=1.0f-S;
                    SqrDistance=S*(A00*S+A01*T+2.0f*B0)+T*(A01*S+A11*T+2.0f*B1)+C;
                }
            } else {
                S=0.0f;
                if(Tmp1<=0.0f) { T=1.0f; SqrDistance=A11+2.0f*B1+C; }
                else if(B1>=0.0f) { T=0.0f; SqrDistance=C; }
                else { T=-B1/A11; SqrDistance=B1*T+C; }
            }
        } else if(T<0.0f) {
            Tmp0=A01+B1; Tmp1=A00+B0;
            if(Tmp1>Tmp0) {
                Numer=Tmp1-Tmp0; Denom=A00-2.0f*A01+A11;
                if(Numer>=Denom) { T=1.0f; S=0.0f; SqrDistance=A11+2.0f*B1+C; }
                else {
                    T=Numer/Denom; S=1.0f-T;
                    SqrDistance=S*(A00*S+A01*T+2.0f*B0)+T*(A01*S+A11*T+2.0f*B1)+C;
                }
            } else {
                T=0.0f;
                if(Tmp1<=0.0f) { S=1.0f; SqrDistance=A00+2.0f*B0+C; }
                else if(B0>=0.0f) { S=0.0f; SqrDistance=C; }
                else { S=-B0/A00; SqrDistance=B0*S+C; }
            }
        } else {
            Numer=A11+B1-A01-B0;
            if(Numer<=0.0f) { S=0.0f; T=1.0f; SqrDistance=A11+2.0f*B1+C; }
            else {
                Denom=A00-2.0f*A01+A11;
                if(Numer>=Denom) { S=1.0f; T=0.0f; SqrDistance=A00+2.0f*B0+C; }
                else {
                    S=Numer/Denom; T=1.0f-S;
                    SqrDistance=S*(A00*S+A01*T+2.0f*B0)+T*(A01*S+A11*T+2.0f*B1)+C;
                }
            }
        }
    }
    return (float)fabs(SqrDistance);
}
}
