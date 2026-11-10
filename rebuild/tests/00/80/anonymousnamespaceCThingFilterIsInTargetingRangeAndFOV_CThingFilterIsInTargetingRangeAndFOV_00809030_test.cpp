// Exact constructor; public field inspection in test only.
// Controlled normalizer and lock-target service; original component lookup is exercised.
#include <stdio.h>
#include <math.h>
#include <bitset>
#include <utility>
static int normalize_calls,lookup_calls,lock_calls;
static float seen_z;
class C3DVector {
public:
    float X,Y,Z;
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    C3DVector& AccurateNormalise(){++normalize_calls;seen_z=Z;float m=(float)sqrt(X*X+Y*Y+Z*Z);if(m){X/=m;Y/=m;Z/=m;}return *this;}
};
class CTCBase {public:virtual ~CTCBase(){}};
class CThing;
enum ETCInterfaceType{TCI_TARGETING=8};
class CTCTargetingBase:public CTCBase {
public:
    CThing* TestLock;
    static ETCInterfaceType StatGetInterfaceType(){return TCI_TARGETING;}
    virtual CThing* GetPTargetLockedThing()const{++lock_calls;return TestLock;}
};
template<class From,class To> inline To GFDebugPDynamicCast(From from,To){return static_cast<To>(from);}
template<class K,class V> class CVectorMap {
public:
    typedef std::pair<K,V> value_type;
    typedef const value_type* const_iterator;
    value_type Entries[5];unsigned Count;
    const_iterator end()const{return Entries+Count;}
    const_iterator LowerBound(const K& key)const{
        ++lookup_calls;const_iterator first=Entries;long count=Count;
        while(count>0){long half=count/2;const_iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}
        return first;
    }
    const_iterator Find(const K& key)const{const_iterator i=LowerBound(key);return i==this->end()||key<i->first?this->end():i;}
};
class CThing {
public:
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*> Components;
    bool HasTC(ETCInterfaceType type)const{return UsingComponents.test(type);}
    template<class T>bool PeekTC(T** result)const{
        if(HasTC(T::StatGetInterfaceType())){
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
};
class CTargetingDef{};
namespace {
class CThingFilter_IsInTargetingRangeAndFOV {
public:

    C3DVector Position;
    C3DVector FacingVector;
    const CThing& Targeter;
    const CThing* PTargetLockThing;
    const CTargetingDef& TargetingDef;
    const CThing* PCarriedThing;
    float TargetingFOV;
    bool AngleTest3D;
public:
    CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&,const C3DVector&,const CThing&,const CTargetingDef&,const CThing*,float,bool);
    bool operator()(const CThing&)const;
};
CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(
    const C3DVector& target_base_pos,const C3DVector& target_dir,const CThing& targeter,
    const CTargetingDef& targeting_def,const CThing* pcarried_thing,float targeting_fov,bool angle_test_3d)
    :Position(target_base_pos),FacingVector(target_dir),Targeter(targeter),PTargetLockThing(0),
     TargetingDef(targeting_def),PCarriedThing(pcarried_thing),TargetingFOV(targeting_fov),AngleTest3D(angle_test_3d)
{
    if(!angle_test_3d)
        FacingVector.Z=0;
    FacingVector.AccurateNormalise();
    const CTCTargetingBase* ptc_targ;
    if(targeter.PeekTC(&ptc_targ))
        PTargetLockThing=ptc_targ->GetPTargetLockedThing();
}
}
int main(){
    unsigned cases=0;const float fovs[]={0.0f,0.5f,1.0f,2.0f};
    for(int axis=0;axis<3;++axis)for(int sign=0;sign<2;++sign)for(int three=0;three<2;++three)
    for(int component=0;component<2;++component)for(int locked=0;locked<2;++locked)
    for(int carried=0;carried<2;++carried)for(int fi=0;fi<4;++fi)for(int location=0;location<3;++location){
        if(axis==2&&!three)continue;
        CThing thing,lock,carry;CTCTargetingBase targeting;CTCBase other;CTargetingDef def;
        targeting.TestLock=locked?&lock:0;
        thing.UsingComponents.reset();thing.UsingComponents.set(7);thing.UsingComponents.set(9);thing.UsingComponents.set(8,component!=0);
        thing.Components.Count=3;
        for(int j=0;j<3;++j){thing.Components.Entries[j].first=(ETCInterfaceType)(j<location?j:j==location?8:8+j);thing.Components.Entries[j].second=j==location?&targeting:&other;}
        float value=sign?-4.0f:4.0f;C3DVector pos(2.0f,-3.0f,8.0f),dir(axis==0?value:0.0f,axis==1?value:0.0f,axis==2?value:0.0f);
        normalize_calls=lookup_calls=lock_calls=0;seen_z=-99.0f;
        CThingFilter_IsInTargetingRangeAndFOV f(pos,dir,thing,def,carried?&carry:0,fovs[fi],three!=0);
        float unit=sign?-1.0f:1.0f;
        if(f.Position.X!=2.0f||f.Position.Y!=-3.0f||f.Position.Z!=8.0f||&f.Targeter!=&thing||&f.TargetingDef!=&def
          ||f.PCarriedThing!=(carried?&carry:0)||f.TargetingFOV!=fovs[fi]||f.AngleTest3D!=(three!=0)
          ||f.PTargetLockThing!=(component&&locked?&lock:0)||normalize_calls!=1||lookup_calls!=component||lock_calls!=component
          ||seen_z!=(three?dir.Z:0.0f)||f.FacingVector.X!=(axis==0?unit:0.0f)||f.FacingVector.Y!=(axis==1?unit:0.0f)
          ||f.FacingVector.Z!=(axis==2?unit:0.0f)||dir.Z!=(axis==2?value:0.0f))return 1;
        ++cases;
    }
    if(cases!=960)return 2;
    puts("00809030_TEST PASS");return 0;
}