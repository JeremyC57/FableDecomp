#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
// 00add690 C3DBoundingSphere::FastBuildFromPoints
// Leaf: ?FastBuildFromPoints@C3DBoundingSphere@@QAEXPBVC3DVector@@J@Z
// Flags:
// Layout: original C3DVector12 XYZ and sphere16 Center0 Radius12; no shared headers supplied.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float Radius;       // +0x0c
    void FastBuildFromPoints(const C3DVector* points, long count);
};

void C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count)
{
    C3DVector minimum(0.0f, 0.0f, 0.0f);
    C3DVector maximum(0.0f, 0.0f, 0.0f);
    long i;
    if (count > 0)
    for (i = 0; i != count; ++i) {
        if (!(minimum.X < points[i].X)) minimum.X = points[i].X;
        if (!(minimum.Y < points[i].Y)) minimum.Y = points[i].Y;
        if (!(minimum.Z < points[i].Z)) minimum.Z = points[i].Z;
        if (!(maximum.X > points[i].X)) maximum.X = points[i].X;
        if (!(maximum.Y > points[i].Y)) maximum.Y = points[i].Y;
        if (!(maximum.Z > points[i].Z)) maximum.Z = points[i].Z;
    }
    Center = C3DVector((maximum.X + minimum.X) * 0.5f, (maximum.Y + minimum.Y) * 0.5f, (maximum.Z + minimum.Z) * 0.5f);
    float best = 0.0f;
    for (unsigned long j = 0; j < (unsigned long)count; ++j) {
        float d = (points[j] - Center).GetSquaredMagnitude();
        if (best < d) best = d;
    }
    Radius = (float)sqrt(best);
}

static double Abs(double x){return x<0?-x:x;}
int main(){int cases=0;for(int count=0;count<=17;++count)for(int pattern=0;pattern<32;++pattern){
 C3DVector points[17];double lo[3]={0,0,0},hi[3]={0,0,0};
 for(int k=0;k<count;++k){points[k].X=float(((k*3+pattern)%17)-8)*0.25f;points[k].Y=float(((k*5+pattern*2)%19)-9)*0.25f;points[k].Z=float(((k*7+pattern*3)%23)-11)*0.25f;double p[3]={points[k].X,points[k].Y,points[k].Z};for(int c=0;c<3;++c){if(p[c]<lo[c])lo[c]=p[c];if(p[c]>hi[c])hi[c]=p[c];}}
 double center[3]={(lo[0]+hi[0])*0.5,(lo[1]+hi[1])*0.5,(lo[2]+hi[2])*0.5},best=0;
 for(int k=0;k<count;++k){double x=points[k].X-center[0],y=points[k].Y-center[1],z=points[k].Z-center[2];double d=x*x+y*y+z*z;if(d>best)best=d;}
 C3DBoundingSphere result;result.Center=C3DVector(123,456,789);result.Radius=999;result.FastBuildFromPoints(points,count);
 CHECK(result.Center.X==center[0]&&result.Center.Y==center[1]&&result.Center.Z==center[2]);CHECK(Abs(result.Radius-sqrt(best))<0.00001);++cases;
 }CHECK(cases==576);puts("00add690_TEST PASS");return 0;}
