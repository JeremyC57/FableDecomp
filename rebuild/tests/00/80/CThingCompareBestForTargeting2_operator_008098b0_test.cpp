#include <stdio.h>
#include <string.h>
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}};
class CThing {public:float Factor;CThing(float f):Factor(f){}};
static int events[6],eventCount;
static void log(int n){if(eventCount<6)events[eventCount++]=n;}
class CBaseIntelligentPointer {public:CThing* PData;CBaseIntelligentPointer(CThing* p=0):PData(p){} CThing* GetPItem()const{log(1);return PData;}};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:CIntelligentPointer(T* p=0):CBaseIntelligentPointer(p){} T* Get()const{return static_cast<T*>(GetPItem());}};
namespace NPlayerTargeting {class CTargetPoint {public:CIntelligentPointer<CThing> PThing;long Index;C3DVector TargetingPosition;long FrameRejected;CTargetPoint(CThing* t,float v):PThing(t),Index(0),TargetingPosition(v),FrameRejected(0){}};}
class CVectorCompare_BestForTargeting {public:float GetCompareValueForPoint(const C3DVector& p)const{log(3);return p.X;}};
const float DELTA_NOTIONAL_ZERO=0.0001f;
class CThingCompare_BestForTargeting2 {public:bool operator()(const NPlayerTargeting::CTargetPoint&,const NPlayerTargeting::CTargetPoint&)const;private:float GetThingCompareMultiplier(const CThing* t)const{log(2);return t->Factor;} CThing* Hero;CVectorCompare_BestForTargeting VectorCompare;float EnemyPreferenceFactor;};
bool CThingCompare_BestForTargeting2::operator()(const NPlayerTargeting::CTargetPoint& a, const NPlayerTargeting::CTargetPoint& b) const
{
    const CThing* thingA = a.PThing.Get();
    const CThing* thingB = b.PThing.Get();
    float multiplierA = GetThingCompareMultiplier(thingA);
    float multiplierB = GetThingCompareMultiplier(thingB);
    C3DVector posA = a.TargetingPosition;
    C3DVector posB = b.TargetingPosition;
    float valueA = VectorCompare.GetCompareValueForPoint(posA) * multiplierA;
    if (valueA - VectorCompare.GetCompareValueForPoint(posB) * multiplierB > DELTA_NOTIONAL_ZERO)
        return true;
    return false;
}

static float fromBits(unsigned int bits){float f;memcpy(&f,&bits,4);return f;}
int main(){
 CThingCompare_BestForTargeting2 compare;
 float values[]={-4,-2,-0.5f,0,0.5f,2,4};
 for(int a=0;a<7;++a)for(int b=0;b<7;++b)for(int c=0;c<7;++c)for(int e=0;e<7;++e){
  CThing ta(values[c]),tb(values[e]);NPlayerTargeting::CTargetPoint pa(&ta,values[a]),pb(&tb,values[b]);
  eventCount=0;bool got=compare(pa,pb);float first=values[a]*values[c];float second=values[b]*values[e];bool expected=first-second>DELTA_NOTIONAL_ZERO;
  if(got!=expected||eventCount!=6||events[0]!=1||events[1]!=1||events[2]!=2||events[3]!=2||events[4]!=3||events[5]!=3)return 1;
 }
 CThing one(1);NPlayerTargeting::CTargetPoint a(&one,1),b(&one,1);if(compare(a,b))return 2;
 a.TargetingPosition.X=fromBits(0x7fc00000);if(compare(a,b)||compare(b,a))return 3;
 a.TargetingPosition.X=fromBits(0x7f800000);if(!compare(a,b)||compare(b,a))return 4;
 puts("008098b0_TEST PASS");return 0;
}