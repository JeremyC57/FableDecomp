int main();
#ifndef _MSC_VER
#define __fastcall
#endif
#include <string.h>
extern "C" int puts(const char*);
// 008ea930 NCreatureMode::CModeHoverBase::GetBestHoverState -- select a hover record for the local velocity
// Leaf: ?GetBestHoverState@CModeHoverBase@NCreatureMode@@ABEPBVCHoverState@12@ABVC3DVector@@0@Z
// Flags:
#include "rebuild_abi.h"
#pragma pack(push,1)
class C3DVector {
public:
    float X,Y,Z;
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z) { }
    C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z) { }
    const float Dot(const C3DVector& v)const { return X*v.X+Y*v.Y+Z*v.Z; }
};
#pragma pack(pop)
class C2DVector {
public:
    float X,Y;
    C2DVector(float x,float y):X(x),Y(y) { }
    C2DVector(const C2DVector& v):X(v.X),Y(v.Y) { }
};
namespace std {
struct input_iterator_tag { };struct forward_iterator_tag:public input_iterator_tag { };struct bidirectional_iterator_tag:public forward_iterator_tag { };struct random_access_iterator_tag:public bidirectional_iterator_tag { };
template<class T>class allocator { };
template<class V,class T,class A>class _STLP_alloc_proxy:public A { public:V _M_data; };
template<class T,class A>class _Vector_base { friend int ::main(); protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:const T* begin()const { return this->_M_start; }const T* end()const { return this->_M_finish; }
};
template<class T,class R>struct unary_function { typedef T argument_type;typedef R result_type; };
template<class I,class P>I __fastcall __find_if(I,I,P,const random_access_iterator_tag&);
template<class I,class P>inline I __fastcall find_if(I first,I last,P pred) { return __find_if(first,last,pred,random_access_iterator_tag()); }
}
class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();
        // Layout: original parent, weapon filter, removal countdown and active state are unread.
        unsigned char unobservedModeState[24];
    };
};
namespace NCreatureMode {
class CModeHoverBase:public CTCCreatureModeManager::CModeBase {
public:
    friend int ::main();
    class CHoverState {
    public:
        // Layout: original animation string, radial area, movement enum and speed are unread by this leaf.
        unsigned char unobservedHoverState[24];
    };
    class CIsValidHoverStateForVelocity:public std::unary_function<CHoverState,bool> {
    public:
        C2DVector Vel;
        CIsValidHoverStateForVelocity(const C2DVector& v):Vel(v) { }
        bool operator()(const CHoverState&)const;
    };
    std::vector<CHoverState> HoverStates;
    // Layout: CurrentName, PCurrentState, CurrentMovementType, AnimQueue and PParticleEffect are unread.
    unsigned char unobservedHoverTail[24];
private:
    const CHoverState* GetBestHoverState(const C3DVector&,const C3DVector&)const;
};
const CModeHoverBase::CHoverState* CModeHoverBase::GetBestHoverState(const C3DVector& direction,const C3DVector& facing)const {
    C3DVector side(facing.Y,-facing.X,0.0f);
    C2DVector velocity(side.Dot(direction),facing.Dot(direction));
    const CHoverState* found=std::find_if(HoverStates.begin(),HoverStates.end(),CIsValidHoverStateForVelocity(velocity));
    return found==HoverStates.end()?0:found;
}
}
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager::CModeBase)==28);
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeHoverBase::CHoverState)==24);
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeHoverBase)==64);

CTCCreatureModeManager::CModeBase::~CModeBase() { }
typedef NCreatureMode::CModeHoverBase Hover;
static const Hover::CHoverState* fixture_first=0;
static const Hover::CHoverState* fixture_last=0;
static int fixture_selected=-1,fixture_calls=0,fixture_tests=0,fixture_errors=0;
static float fixture_x=0,fixture_y=0;
bool Hover::CIsValidHoverStateForVelocity::operator()(const CHoverState& state)const { ++fixture_tests;if(Vel.X!=fixture_x || Vel.Y!=fixture_y || &state<fixture_first || &state>=fixture_last)++fixture_errors;return fixture_selected>=0 && &state==fixture_first+fixture_selected; }
namespace std {
template<class I,class P>I __fastcall __find_if(I first,I last,P pred,const random_access_iterator_tag&) { ++fixture_calls;if(first!=fixture_first || last!=fixture_last || pred.Vel.X!=fixture_x || pred.Vel.Y!=fixture_y)++fixture_errors;for(;first!=last;++first)if(pred(*first))break;return first; }
}
struct FixtureGuardedHover { unsigned long before;Hover hover;unsigned long after; };
int main() {
    unsigned int cases=0;
    for(int d=0;d<27;++d)for(int f=0;f<27;++f)for(int n=0;n<=6;++n)for(int selected=-1;selected<n;++selected) {
        C3DVector direction((float)(d%3-1),(float)((d/3)%3-1),(float)(d/9-1));C3DVector facing((float)(f%3-1),(float)((f/3)%3-1),(float)(f/9-1));
        FixtureGuardedHover guarded;Hover::CHoverState states[8];memset(states,0x5a,sizeof(states));guarded.before=0x12345678UL;guarded.after=0xfedcba98UL;guarded.hover.HoverStates._M_start=states+1;guarded.hover.HoverStates._M_finish=states+1+n;guarded.hover.HoverStates._M_end_of_storage._M_data=states+7;
        fixture_first=states+1;fixture_last=states+1+n;fixture_selected=selected;fixture_calls=fixture_tests=fixture_errors=0;
        fixture_x=facing.Y*direction.X-facing.X*direction.Y;fixture_y=facing.X*direction.X+facing.Y*direction.Y+facing.Z*direction.Z;
        unsigned char beforeHover[sizeof(Hover)],beforeStates[sizeof(states)];memcpy(beforeHover,&guarded.hover,sizeof(Hover));memcpy(beforeStates,states,sizeof(states));
        const Hover& immutable=guarded.hover;const Hover::CHoverState* actual=immutable.GetBestHoverState(direction,facing);
        if(actual!=(selected<0?0:states+1+selected) || fixture_errors || fixture_calls!=1 || fixture_tests!=(selected<0?n:selected+1))return 1;
        if(memcmp(beforeHover,&guarded.hover,sizeof(Hover)) || memcmp(beforeStates,states,sizeof(states)) || guarded.before!=0x12345678UL || guarded.after!=0xfedcba98UL)return 2;
        ++cases;
    }
    if(cases!=20412)return 3;puts("008ea930_TEST PASS");return 0;
}