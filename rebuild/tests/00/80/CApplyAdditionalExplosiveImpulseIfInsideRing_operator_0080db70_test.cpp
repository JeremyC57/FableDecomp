#include <math.h>
#include <assert.h>
#include <stdio.h>
const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
float GetAccurateMagnitude()const {float magnitude=X*X+Y*Y+Z*Z;return (float)sqrt(magnitude);}};
// Boundary stub records the external physics operations in their call order.
class CTCPhysicsBase {public:C3DVector Position,impulse;int phase;
const C3DVector& GetPosition()const{return Position;}
void ApplyControlledImpulse(const C3DVector& v){assert(phase==0);impulse=v;phase=1;}
void SetAsMoving(bool moving){assert(moving&&phase==1);phase=2;}};
class CThing {public:CTCPhysicsBase* PhysicsTC;CTCPhysicsBase* GetPhysicsTC(){return PhysicsTC;}};
class CApplyAdditionalExplosiveImpulseIfInsideRing {private:C3DVector BasePosition;float InnerRadius,OuterRadius;public:CApplyAdditionalExplosiveImpulseIfInsideRing(const C3DVector& p,float i,float o):BasePosition(p),InnerRadius(i),OuterRadius(o){} void operator()(CThing* pnearby_thing)const;};
void CApplyAdditionalExplosiveImpulseIfInsideRing::operator()(CThing* pnearby_thing) const
{
 CTCPhysicsBase& their_phys=*pnearby_thing->GetPhysicsTC();
 const C3DVector& their_pos=their_phys.GetPosition();
 C3DVector explosion_to_thing=their_pos-BasePosition;
 explosion_to_thing.Z=0.0f;
 float mag=explosion_to_thing.GetAccurateMagnitude();
 if(mag<DELTA_NOTIONAL_ZERO) explosion_to_thing=C3DVector(1.0f,0.0f,0.0f);
 else explosion_to_thing/=mag;
 if(mag>InnerRadius&&mag<OuterRadius){
  float dist_to_move=OuterRadius-mag;
  const float FRACTION_OF_DISTANCE_OUT_TO_MOVE_EACH_FRAME=0.25f;
  their_phys.ApplyControlledImpulse(explosion_to_thing*dist_to_move*FRACTION_OF_DISTANCE_OUT_TO_MOVE_EACH_FRAME);
  their_phys.SetAsMoving(true);
 }
}

int main(){
 CTCPhysicsBase physics;CThing thing;thing.PhysicsTC=&physics;
 C3DVector base(2,-3,100);
 for(int inner=0;inner<4;++inner)for(int outer=inner+1;outer<7;++outer)for(int x=-8;x<=8;++x)for(int y=-8;y<=8;++y){
  CApplyAdditionalExplosiveImpulseIfInsideRing ring(base,(float)inner,(float)outer);physics.Position=C3DVector(2+(float)x,-3+(float)y,-900);physics.phase=0;
  ring(&thing);double mag=sqrt((double)x*x+(double)y*y);bool active=mag>inner&&mag<outer;assert(physics.phase==(active?2:0));
  if(active){double scale=(outer-mag)*0.25/mag;assert(fabs(physics.impulse.X-x*scale)<0.000002);assert(fabs(physics.impulse.Y-y*scale)<0.000002);assert(physics.impulse.Z==0.0f);}
 }
 CApplyAdditionalExplosiveImpulseIfInsideRing ring(base,-1,1);physics.Position=base;physics.phase=0;ring(&thing);
 assert(physics.phase==2&&physics.impulse.X==0.25f&&physics.impulse.Y==0.0f&&physics.impulse.Z==0.0f);
 puts("0080db70_TEST PASS");return 0;
}