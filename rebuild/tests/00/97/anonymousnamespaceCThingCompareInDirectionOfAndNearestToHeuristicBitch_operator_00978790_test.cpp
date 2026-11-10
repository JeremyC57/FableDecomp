#include <cstdio>
#include <cmath>
#include <math.h>
class C3DVector { public: float X,Y,Z; C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} };
class C2DVector { public: float X,Y; C2DVector(float x=0,float y=0):X(x),Y(y){} C2DVector(const C3DVector& v):X(v.X),Y(v.Y){} C2DVector operator-(const C2DVector& v)const{return C2DVector(X-v.X,Y-v.Y);} float GetMagnitude()const{return (float)std::sqrt(X*X+Y*Y);} const C2DVector& operator*=(float v){X*=v;Y*=v;return *this;} float Dot(const C2DVector& v)const{return X*v.X+Y*v.Y;} };
class CThing { public: C3DVector Position; const C3DVector& GetPos()const{return Position;} };
namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                              // +0x00
    C2DVector Dir;                              // +0x08
    float GetWeightFromParams(float dot, float distance) const
    {
        return (float)pow(dot, (float)((long)distance / 4 * 2 + 1));
    }
    bool operator()(const CThing* a, const CThing* b) const
    {
        C2DVector offsetA = C2DVector(a->GetPos()) - Pos;
        float distanceA = offsetA.GetMagnitude();
        offsetA *= 1.0f / distanceA;
        float dotA = offsetA.Dot(Dir);
        float weightA = GetWeightFromParams(dotA, distanceA);
        C2DVector offsetB = C2DVector(b->GetPos()) - Pos;
        float distanceB = offsetB.GetMagnitude();
        offsetB *= 1.0f / distanceB;
        float dotB = offsetB.Dot(Dir);
        if (GetWeightFromParams(dotB, distanceB) < weightA)
            return true;
        return false;
    }
};
}

static double ReferenceWeight(const CThing& t,const C2DVector& pos,const C2DVector& dir){double x=t.Position.X-pos.X,y=t.Position.Y-pos.Y;double distance=std::sqrt(x*x+y*y);double alignment=(x*dir.X+y*dir.Y)/distance;int exponent=((int)distance/4)*2+1;return std::pow(alignment,exponent);}
int main(){
 CThing points[8]; points[0].Position=C3DVector(2,0,8);points[1].Position=C3DVector(0,2,-8);points[2].Position=C3DVector(3,4,0);points[3].Position=C3DVector(-3,4,0);points[4].Position=C3DVector(9,0,0);points[5].Position=C3DVector(0,-9,0);points[6].Position=C3DVector(-2,0,0);points[7].Position=C3DVector(5,12,0);
 CThingCompare_InDirectionOfAndNearestToHeuristicBitch compare;compare.Pos=C2DVector(0,0);compare.Dir=C2DVector(1,0);int ok=1,cases=0;
 for(int i=0;i<8;++i)for(int j=0;j<8;++j){double wa=ReferenceWeight(points[i],compare.Pos,compare.Dir),wb=ReferenceWeight(points[j],compare.Pos,compare.Dir);if(std::fabs(wa-wb)>1e-5){ok &= compare(&points[i],&points[j])==(wb<wa);++cases;}}
 ok &= std::fabs(compare.GetWeightFromParams(0.5f,3.5f)-0.5f)<1e-6f;
 ok &= std::fabs(compare.GetWeightFromParams(0.5f,4.0f)-0.125f)<1e-6f;
 ok &= std::fabs(compare.GetWeightFromParams(-0.5f,8.0f)+0.03125f)<1e-6f;
 compare.Pos=C2DVector(1,1);compare.Dir=C2DVector(0,1);ok &= compare(&points[2],&points[1])==(ReferenceWeight(points[1],compare.Pos,compare.Dir)<ReferenceWeight(points[2],compare.Pos,compare.Dir));
 compare.Pos=C2DVector(0,0);compare.Dir=C2DVector(1,0);
 ok &= !compare(&points[0],&points[4]) && !compare(&points[4],&points[0]);
 ok &= !compare(&points[1],&points[5]) && !compare(&points[5],&points[1]);
 ok &= cases>=48;
 std::printf(ok ? "00978790_TEST PASS\n" : "00978790_TEST FAIL\n");return ok?0:1;}