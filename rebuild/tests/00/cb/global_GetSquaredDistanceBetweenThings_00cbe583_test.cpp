// Standalone behaviour fixture; the byte grade uses the complete shared polymorphic class.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
#define CHECK(x) do { if(!(x)) { printf("FAIL %d\n",__LINE__); abort(); } } while(0)
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
};
class CScriptThing {
public:
    C3DVector Position;
    mutable int Reads;
    CScriptThing(float x,float y,float z):Position(x,y,z),Reads(0){}
    virtual ~CScriptThing(){}
    virtual const C3DVector& GetPos()const {++Reads;return Position;}
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a,const CScriptThing& b)
{
    return (a.GetPos()-b.GetPos()).GetSquaredMagnitude();
}
int main()
{
    const int points[][3]={{0,0,0},{1,0,0},{0,-2,0},{0,0,3},{3,4,0},{-3,4,-5},{7,-8,9},{16,8,-4},{-16,-8,4}};
    for(int i=0;i<9;++i) for(int j=0;j<9;++j) {
        CScriptThing a((float)points[i][0],(float)points[i][1],(float)points[i][2]);
        CScriptThing b((float)points[j][0],(float)points[j][1],(float)points[j][2]);
        int total=0;
        for(int k=0;k<3;++k){int delta=points[i][k]-points[j][k];total+=delta*delta;}
        CHECK(GetSquaredDistanceBetweenThings(a,b)==(float)total);
        CHECK(a.Reads==1 && b.Reads==1);
        CHECK(GetSquaredDistanceBetweenThings(b,a)==(float)total);
        CHECK(a.Reads==2 && b.Reads==2);
        CHECK(GetSquaredDistanceBetweenThings(a,a)==0.0f && a.Reads==4);
        CHECK(a.Position.X==(float)points[i][0] && a.Position.Y==(float)points[i][1] && a.Position.Z==(float)points[i][2]);
        CHECK(b.Position.X==(float)points[j][0] && b.Position.Y==(float)points[j][1] && b.Position.Z==(float)points[j][2]);
    }
    CScriptThing a(0.5f,1.25f,-2.0f),b(-0.5f,-0.75f,1.0f);
    CHECK(GetSquaredDistanceBetweenThings(a,b)==14.0f);
    puts("00cbe583_TEST PASS");return 0;
}