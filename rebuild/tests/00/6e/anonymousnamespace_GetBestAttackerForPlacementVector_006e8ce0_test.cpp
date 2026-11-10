#include <list>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// Standalone boundary fixture: list is the compiler standard library; the creature
// stub provides only the external GetPos result used by the exact target body.
const float DELTA_NOTIONAL_ZERO=0.0001f;
class C3DVector {
public:
 float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
 const C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}
 float GetAccurateMagnitude()const{float magnitude=X*X+Y*Y+Z*Z;return (float)sqrt(magnitude);}
 const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
 const float Dot(const C3DVector& o)const{return X*o.X+Y*o.Y+Z*o.Z;}
};
class CThing {public:C3DVector position;const C3DVector& GetPos()const{return position;}};
class CThingCreatureBase:public CThing {};
namespace {
const CThingCreatureBase* __fastcall GetBestAttackerForPlacementVector(
    const std::list<const CThingCreatureBase*>& closest_attackers,
    const C3DVector& defender_pos, const C3DVector& placement_vector)
{
    const CThingCreatureBase* pbest_creature=0;
    float best_value=-(1.0f+DELTA_NOTIONAL_ZERO);
    for(std::list<const CThingCreatureBase*>::const_iterator it=closest_attackers.begin();it!=closest_attackers.end();++it) {
        const CThingCreatureBase* pcreature_base=*it;
        C3DVector norm_placement_vector(placement_vector);
        norm_placement_vector.Z=0.0f;
        float mag_placement=norm_placement_vector.GetAccurateMagnitude();
        norm_placement_vector/=mag_placement;
        C3DVector defender_to_attacker(pcreature_base->GetPos()-defender_pos);
        defender_to_attacker.Z=0.0f;
        float mag_distance=defender_to_attacker.GetAccurateMagnitude();
        defender_to_attacker/=mag_distance;
        float cos_thet=norm_placement_vector.Dot(defender_to_attacker);
        if(cos_thet < -1.0f) cos_thet=-1.0f;
        else if(cos_thet > 1.0f) cos_thet=1.0f;
        float delta_mag=(float)fabs(mag_placement-mag_distance);
        float this_value=cos_thet;
        if(this_value>best_value) {pbest_creature=pcreature_base;best_value=this_value;}
    }
    return pbest_creature;
}
}
static const CThingCreatureBase* oracle(const std::list<const CThingCreatureBase*>& items,const C3DVector& origin,const C3DVector& direction){
 const CThingCreatureBase* best=0;double score=-1.0001;
 for(std::list<const CThingCreatureBase*>::const_iterator i=items.begin();i!=items.end();++i){
 double x=(*i)->position.X-origin.X,y=(*i)->position.Y-origin.Y;
 double a=sqrt(x*x+y*y),b=sqrt((double)direction.X*direction.X+(double)direction.Y*direction.Y);
 double q=(x*direction.X+y*direction.Y)/(a*b);if(q < -1)q=-1;else if(q>1)q=1;
 if(q>score){score=q;best=*i;}
 }return best;
}
int main(){
 CThingCreatureBase c[4];std::list<const CThingCreatureBase*> items;
 C3DVector origin(0,0,50),direction(1,0,99);
 assert(GetBestAttackerForPlacementVector(items,origin,direction)==0);
 for(int i=0;i<4;++i)items.push_back(&c[i]);
 unsigned int seed=12345;
 for(int trial=0;trial<4096;++trial){
   for(int j=0;j<4;++j){seed=seed*1664525u+1013904223u;float x=(float)((seed>>16)%101)-50.0f;
    seed=seed*1664525u+1013904223u;float y=(float)((seed>>16)%101)-50.0f;if(x==0&&y==0)x=1;
    c[j].position=C3DVector(x,y,(float)j*123.0f);}
   direction=C3DVector(1.0f,0.37f,99.0f);
   assert(GetBestAttackerForPlacementVector(items,origin,direction)==oracle(items,origin,direction));
 }
 items.clear();c[0].position=C3DVector(1,0,200);c[1].position=C3DVector(2,0,-200);
 items.push_back(&c[0]);items.push_back(&c[1]);direction=C3DVector(1,0,3);
 assert(GetBestAttackerForPlacementVector(items,origin,direction)==&c[0]);
 items.clear();c[0].position=origin;items.push_back(&c[0]);
 assert(GetBestAttackerForPlacementVector(items,origin,direction)==0);
 puts("006e8ce0_TEST PASS");return 0;
}