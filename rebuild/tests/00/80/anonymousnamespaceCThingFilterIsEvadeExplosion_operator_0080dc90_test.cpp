// Behaviour test for `anonymous namespace'::CThingFilter_IsEvadeExplosion::operator() (0x0080DC90): the row body (and its same-TU
// IsTravellingAway) over models of its callees. Kills: the IsRolling gate dropped / inverted, the wrong thing queried, the move taken
// from the wrong positions or in the wrong direction, Z not flattened (move or line-out), the magnitude guard skipped / wrong bound
// (DELTA_NOTIONAL_ZERO, strict >), the line-out query with the wrong ends or point, the IsNonZero guard dropped, the line-out not
// normalised before the direction test, the direction test's arguments swapped against the threshold, and a pass returned false.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
const float DELTA_NOTIONAL_ZERO=1e-4f;
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 const C3DVector& operator/=(float v){float r=1.0f/v;X*=r;Y*=r;Z*=r;return *this;}
 float GetAccurateMagnitude()const{float m=X*X+Y*Y+Z*Z;return (float)sqrt(m);}
 bool IsNonZero()const{return Y*Y+X*X+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 const float Dot(const C3DVector& o)const{return X*o.X+Y*o.Y+Z*o.Z;}
 C3DVector& AccurateNormalise();};
static int normalise_calls;
C3DVector& C3DVector::AccurateNormalise(){++normalise_calls;float m=(float)sqrt(X*X+Y*Y+Z*Z);X/=m;Y/=m;Z/=m;return *this;}
class CTCPhysicsBase {public:C3DVector Position,OldPosition;
 const C3DVector& GetPosition()const{return Position;}const C3DVector& GetLastPosition()const{return OldPosition;}};
class CThing {public:CTCPhysicsBase* PhysicsTC;bool Rolling;const CTCPhysicsBase* GetPhysicsTC()const{return PhysicsTC;}};
static int rolling_calls,lineout_calls;static const CThing* rolling_arg;static C3DVector lineout_result;
static const C3DVector *lo_start,*lo_end,*lo_point;
namespace NCreatureMode { bool IsRolling(const CThing& thing){++rolling_calls;rolling_arg=&thing;return thing.Rolling;} }
namespace NPhysicsHelpers {
C3DVector GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& s,const C3DVector& e,const C3DVector& p)
{++lineout_calls;lo_start=&s;lo_end=&e;lo_point=&p;return lineout_result;} }
namespace {
class CThingFilter_IsEvadeExplosion {
public:
    CThingFilter_IsEvadeExplosion(const C3DVector& start_pos, const C3DVector& end_pos) : StartPos(start_pos), EndPos(end_pos) {}
    bool IsTravellingAway(const C3DVector& a, const C3DVector& b) const;
    bool operator()(const CThing& thing) const;
private:
    const C3DVector& StartPos;
    const C3DVector& EndPos;
};
bool CThingFilter_IsEvadeExplosion::IsTravellingAway(const C3DVector& a, const C3DVector& b) const
{
    return b.Dot(a) > cos((double)0.43633232f);
}
bool CThingFilter_IsEvadeExplosion::operator()(const CThing& thing) const
{
    if (NCreatureMode::IsRolling(thing)) {
        const C3DVector& pos = thing.GetPhysicsTC()->GetPosition();
        C3DVector move = pos - thing.GetPhysicsTC()->GetLastPosition();
        move.Z = 0.0f;
        float magnitude = move.GetAccurateMagnitude();
        if (magnitude > DELTA_NOTIONAL_ZERO) {
            move /= magnitude;
            C3DVector out = NPhysicsHelpers::GetLineOutFromClosestPointOnFiniteLineToPoint(StartPos, EndPos, pos);
            out.Z = 0.0f;
            if (out.IsNonZero()) {
                out.AccurateNormalise();
                if (IsTravellingAway(out, move))
                    return true;
            }
        }
    }
    return false;
}
}
// reference model of the filter
static bool expect(const CThing& t,const C3DVector& lo,bool& wants_lineout,bool& wants_norm){
 wants_lineout=wants_norm=false;if(!t.Rolling)return false;
 double mx=(double)t.PhysicsTC->Position.X-t.PhysicsTC->OldPosition.X,my=(double)t.PhysicsTC->Position.Y-t.PhysicsTC->OldPosition.Y;
 double m=sqrt(mx*mx+my*my);if(!(m>1e-4))return false;wants_lineout=true;
 double lx=lo.X,ly=lo.Y;double l2=lx*lx+ly*ly;if(!(l2>1e-8))return false;wants_norm=true;
 double l=sqrt(l2);return (mx/m)*(lx/l)+(my/m)*(ly/l)>cos(0.43633232);}
int main(){
 C3DVector start(1.0f,2.0f,3.0f),end(-4.0f,5.0f,6.0f);CThingFilter_IsEvadeExplosion f(start,end);
 CTCPhysicsBase phys;CThing thing;thing.PhysicsTC=&phys;
 const float d[]={0.0f,0.00005f,0.3f,-1.0f,2.5f};
 const float ang[]={0.0f,0.3f,0.5f,1.6f,3.1f,-0.2f};
 long cases=0,passes=0;
 for(int r=0;r!=2;++r)for(int a=0;a!=5;++a)for(int b=0;b!=5;++b)for(int k=0;k!=6;++k)for(int z=0;z!=3;++z){
  thing.Rolling=r!=0;phys.OldPosition=C3DVector(10.0f,-3.0f,7.0f);
  phys.Position=C3DVector(10.0f+d[a],-3.0f+d[b],7.0f+(z==1?50.0f:0.0f));
  float s=(z==2)?0.0f:3.0f;lineout_result=C3DVector(s*(float)cos(ang[k]),s*(float)sin(ang[k]),9.0f);
  rolling_calls=lineout_calls=normalise_calls=0;
  bool got=f(thing);bool wl,wn;bool want=expect(thing,lineout_result,wl,wn);
  check(rolling_calls==1&&rolling_arg==&thing);
  check(lineout_calls==(wl?1:0)&&normalise_calls==(wn?1:0));
  if(wl)check(lo_start==&start&&lo_end==&end&&lo_point==&phys.Position);
  check(got==want);if(got)++passes;++cases;
 }
 check(cases==900&&passes>0&&passes<cases);
 // direction test: travelling directly away passes, sideways fails, threshold at ~25 degrees
 C3DVector x(1.0f,0.0f,0.0f);check(f.IsTravellingAway(x,x));
 C3DVector yv(0.0f,1.0f,0.0f);check(!f.IsTravellingAway(x,yv));
 C3DVector in(cosf(0.42f),sinf(0.42f),0.0f),outv(cosf(0.45f),sinf(0.45f),0.0f);check(f.IsTravellingAway(x,in)&&!f.IsTravellingAway(x,outv));
 printf("%ld %ld\n",cases,passes);puts("0080dc90_TEST PASS");return 0;
}