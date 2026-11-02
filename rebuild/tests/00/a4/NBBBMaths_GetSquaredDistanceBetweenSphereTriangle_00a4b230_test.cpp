#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H
// PDB C3DVector TPI 0x273e: public X/Y/Z at 0/4/8, size 12.
// Embedded alignment is under review; this header introduces no packing directive.
#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

// NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(...) -- Squared distance to a triangle.
// Leaf: ?GetSquaredDistanceBetweenSphereTriangle@NBBBMaths@@YIMABVC3DVector@@M000@Z

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

static double component(const C3DVector& v,int i) { return i==0?v.X:i==1?v.Y:v.Z; }
static double dot(const double* a,const double* b) { return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]; }
static double reference(const C3DVector& p,const C3DVector& origin,const C3DVector& a,const C3DVector& b)
{
    double vertices[3][3],q[3],edgeA[3],edgeB[3]; int i,j;
    for(i=0;i<3;++i) {
        vertices[0][i]=component(origin,i);
        vertices[1][i]=component(origin,i)+component(a,i);
        vertices[2][i]=component(origin,i)+component(b,i);
        q[i]=component(p,i)-component(origin,i);
        edgeA[i]=component(a,i); edgeB[i]=component(b,i);
    }
    double best=1.0e30;
    for(i=0;i<3;++i) {
        double d[3],r[3];
        for(j=0;j<3;++j) { d[j]=vertices[(i+1)%3][j]-vertices[i][j]; r[j]=component(p,j)-vertices[i][j]; }
        double t=dot(r,d)/dot(d,d); if(t<0.0)t=0.0; if(t>1.0)t=1.0;
        double distance=0.0;
        for(j=0;j<3;++j) { double e=r[j]-t*d[j]; distance+=e*e; }
        if(distance<best)best=distance;
    }
    double aa=dot(edgeA,edgeA),ab=dot(edgeA,edgeB),bb=dot(edgeB,edgeB);
    double qa=dot(q,edgeA),qb=dot(q,edgeB),det=aa*bb-ab*ab;
    double u=(qa*bb-qb*ab)/det,v=(qb*aa-qa*ab)/det;
    if(u>=0.0&&v>=0.0&&u+v<=1.0) {
        double distance=0.0;
        for(j=0;j<3;++j) { double e=q[j]-u*edgeA[j]-v*edgeB[j]; distance+=e*e; }
        if(distance<best)best=distance;
    }
    return best;
}
static void check(const C3DVector& p,const C3DVector& origin,const C3DVector& a,const C3DVector& b,bool exact)
{
    float actual=NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(p,0.0f,origin,a,b);
    double expected=reference(p,origin,a,b);
    if((exact && actual!=(float)expected)||fabs(actual-expected)>0.0001*(1.0+expected)) {
        fprintf(stderr,"distance mismatch %g expected %g\n",actual,expected); exit(1);
    }
    if(actual!=NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(p,8.0f,origin,a,b))exit(2);
}
int main()
{
    C3DVector origin(4.0f,-8.0f,2.0f);
    int a,b,sgn,i,j,k;
    for(a=0;a<3;++a) for(b=0;b<3;++b) if(a!=b) for(sgn=-1;sgn<=1;sgn+=2) {
        float av[3]={0,0,0},bv[3]={0,0,0}; av[a]=4.0f; bv[b]=4.0f*(float)sgn;
        C3DVector edgeA(av[0],av[1],av[2]),edgeB(bv[0],bv[1],bv[2]);
        for(i=-4;i<=12;++i)for(j=-4;j<=12;++j)for(k=-2;k<=2;++k) {
            float q[3]={origin.X,origin.Y,origin.Z}; q[a]+=(float)i*0.5f; q[b]+=(float)j*0.5f*(float)sgn; q[3-a-b]+=(float)k*0.5f;
            C3DVector p(q[0],q[1],q[2]); check(p,origin,edgeA,edgeB,true);
        }
    }
    C3DVector edgeA(4.0f,2.0f,0.0f),edgeB(1.0f,4.0f,2.0f);
    for(i=-4;i<=12;++i)for(j=-4;j<=12;++j)for(k=-2;k<=2;++k) {
        C3DVector p(origin.X+(float)i*0.5f,origin.Y+(float)j*0.5f,origin.Z+(float)k*0.5f);
        check(p,origin,edgeA,edgeB,false);
    }
    puts("00a4b230_TEST PASS"); return 0;
}