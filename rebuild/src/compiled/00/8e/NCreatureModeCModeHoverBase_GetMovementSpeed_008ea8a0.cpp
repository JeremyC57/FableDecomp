// 008ea8a0 NCreatureMode::CModeHoverBase::GetMovementSpeed -- choose the matching hover movement speed
// Leaf: ?GetMovementSpeed@CModeHoverBase@NCreatureMode@@UBEMABVC3DVector@@0W4EMovementType@@@Z
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
template<class T,class A>class _Vector_base { protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:const T* begin()const { return this->_M_start; }const T* end()const { return this->_M_finish; }
};
template<class T,class R>struct unary_function { typedef T argument_type;typedef R result_type; };
template<class I,class P>I __fastcall __find_if(I,I,P,const random_access_iterator_tag&);
template<class I,class P>inline I __fastcall find_if(I first,I last,P pred) { return __find_if(first,last,pred,random_access_iterator_tag()); }
}
enum EMovementType { };
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
    class CHoverState {
    public:
        // Layout: original animation string, radial area and movement enum are unread; the original speed remains typed.
        unsigned char unobservedHoverState[20];
        float MovementSpeed;
    };
    class CIsValidHoverStateForMovementType:public std::unary_function<CHoverState,bool> {
    public:
        C2DVector MvDir;EMovementType Type;
        CIsValidHoverStateForMovementType(const C2DVector& direction,EMovementType type):MvDir(direction),Type(type) { }
        bool operator()(const CHoverState&)const;
    };
    std::vector<CHoverState> HoverStates;
    // Layout: CurrentName, PCurrentState, CurrentMovementType, AnimQueue and PParticleEffect are unread.
    unsigned char unobservedHoverTail[24];
    virtual float GetMovementSpeed(const C3DVector&,const C3DVector&,EMovementType)const;
};
float CModeHoverBase::GetMovementSpeed(const C3DVector& direction,const C3DVector& facing,EMovementType type)const {
    C3DVector side(facing.Y,-facing.X,0.0f);
    C2DVector movement(side.Dot(direction),facing.Dot(direction));
    const CHoverState* found=std::find_if(HoverStates.begin(),HoverStates.end(),CIsValidHoverStateForMovementType(movement,type));
    return found==HoverStates.end()?0.0f:found->MovementSpeed;
}
}
FABLE_STATIC_ASSERT(sizeof(CTCCreatureModeManager::CModeBase)==28);
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeHoverBase::CHoverState)==24);
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeHoverBase)==64);