#include <stdio.h>
// 0070c900 CAnimSet::GetMeshAnim
// Leaf: ?GetMeshAnim@CAnimSet@@QBE?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@M@Z
// Flags:
// Layout: CAnimSet has no engine header. PDB struct_layouts_egor.tsv:23385-23408
// plus native offsets prove Anims+4, InternalAlphas+0x10, percentages+0x1c/+0x20;
// two 16-byte donor vectors become two 12-byte retail STLport vectors.
#define CIVCountedPointeeBase CIVCountedPointeeBaseLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define C3DAnimationState C3DAnimationStateLayout
#include "engine/C3DAnimationState.h"
#undef C3DAnimationState
#define C3DAnimationTransitionState C3DAnimationTransitionStateLayout
#include "engine/C3DAnimationTransitionState.h"
#undef C3DAnimationTransitionState
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;
    mutable unsigned long IVRefCount;
    CIVCountedPointeeBase():IVRefCount(1) { }
    void AddRef() const { ++IVRefCount; }
    long Release() const { long count=--IVRefCount;if(!count) Delete();return count; }
};
template<class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(T* p):Object(p) { }
    CIVCountedPointer(const CIVCountedPointer& p):Object(p.Object) { if(Object) Object->AddRef(); }
    template<class U> CIVCountedPointer(const CIVCountedPointer<U>& p):Object(p.Object) { if(Object) Object->AddRef(); }
    ~CIVCountedPointer() { if(Object) { Object->Release();Object=0; } }
};
class C3DAnimationStateBase : public CIVCountedPointeeBase {
public:
    enum EType { };
    virtual EType GetType() const=0;
};
class C3DAnimationState : public C3DAnimationStateBase {
public:
    long AnimationIndex;
    float AnimationTime;
    C3DAnimationState(unsigned long index,float time):AnimationIndex(index),AnimationTime(time) { }
    virtual EType GetType() const;
};
class C3DAnimationTransitionState : public C3DAnimationStateBase {
public:
    CIVCountedPointer<C3DAnimationStateBase> StartState,EndState;
    float Progress;
    C3DAnimationTransitionState(const CIVCountedPointer<C3DAnimationStateBase>&,const CIVCountedPointer<C3DAnimationStateBase>&,float);
    virtual EType GetType() const;
};
namespace std {
template<class T> class allocator { };
template<class V,class T,class A> class _STLP_alloc_proxy : public A { public:V _M_data; };
template<class T,class A> class _Vector_base {
public:
    T* _M_start;T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    unsigned int size() const { return this->_M_finish-this->_M_start; }
    const T* begin() const { return this->_M_start; }
    const T& operator[](unsigned int n) const { return *(begin()+n); }
};
}

class CTransitionBlendAlpha { public:float CurrentAlpha,LastAlpha; };
enum EAnimUpdateMode { };
class CAnimSet {
public:
    EAnimUpdateMode UpdateMode;
    std::vector<short> Anims;
    std::vector<CTransitionBlendAlpha> InternalAlphas;
    float CurrentPercentageComplete,LastPercentageComplete;
    CIVCountedPointer<C3DAnimationStateBase> GetMeshAnim(float) const;
private:
    float GetInterpolatedTime(float,long) const;
    float GetInterpolatedInternalAlpha(float fraction,long index) const {
        float from=InternalAlphas[index].LastAlpha;
        float to=InternalAlphas[index].CurrentAlpha;
        return (to-from)*fraction+from;
    }
};
CIVCountedPointer<C3DAnimationStateBase> CAnimSet::GetMeshAnim(float fraction) const
{
    CIVCountedPointer<C3DAnimationState> start(new C3DAnimationState(Anims[0],GetInterpolatedTime(fraction,0)));
    if(Anims.size()==1) return start;
    CIVCountedPointer<C3DAnimationState> end(new C3DAnimationState(Anims[1],GetInterpolatedTime(fraction,1)));
    CIVCountedPointer<C3DAnimationTransitionState> transition(new C3DAnimationTransitionState(start,end,GetInterpolatedInternalAlpha(fraction,1)));
    return transition;
}

static unsigned long fixtureDestroyed=0;
CIVCountedPointeeBase::~CIVCountedPointeeBase() { ++fixtureDestroyed; }
void CIVCountedPointeeBase::Delete() const { delete this; }
C3DAnimationStateBase::EType C3DAnimationState::GetType() const { return (EType)0; }
C3DAnimationStateBase::EType C3DAnimationTransitionState::GetType() const { return (EType)2; }
C3DAnimationTransitionState::C3DAnimationTransitionState(const CIVCountedPointer<C3DAnimationStateBase>& a,const CIVCountedPointer<C3DAnimationStateBase>& b,float progress)
:StartState(a),EndState(b),Progress(progress) { }
// Dependency contract: distinguish animation-index calls and preserve exact binary fractions.
float CAnimSet::GetInterpolatedTime(float f,long i) const
{ return LastPercentageComplete+(CurrentPercentageComplete-LastPercentageComplete)*f+(float)(4*i); }
static bool closeValue(float a,float b) { float difference=a-b;if(difference<0) difference=-difference;return difference<0.000001f; }
int main()
{
    short indices[3]={7,19,31};
    CTransitionBlendAlpha alpha[3];
    alpha[0].LastAlpha=0;alpha[0].CurrentAlpha=1;
    alpha[1].LastAlpha=0.25f;alpha[1].CurrentAlpha=0.75f;
    alpha[2].LastAlpha=0.5f;alpha[2].CurrentAlpha=1;
    const float fractions[6]={-1.0f,0.0f,0.25f,0.5f,1.0f,2.0f};
    unsigned long cases=0,expectedDestroyed=0;
    for(int round=0;round<4;++round) for(int n=1;n<=3;++n) for(int fi=0;fi<6;++fi) {
        CAnimSet set;
        set.Anims._M_start=indices;set.Anims._M_finish=indices+n;set.Anims._M_end_of_storage._M_data=indices+3;
        set.InternalAlphas._M_start=alpha;set.InternalAlphas._M_finish=alpha+3;set.InternalAlphas._M_end_of_storage._M_data=alpha+3;
        set.LastPercentageComplete=(float)round;
        set.CurrentPercentageComplete=(float)(round+2);
        float f=fractions[fi];
        float expectedFirst=(float)round+2.0f*f;
        {
            CIVCountedPointer<C3DAnimationStateBase> result=set.GetMeshAnim(f);
            if(!result.Object || result.Object->IVRefCount!=1) return 1;
            if(n==1) {
                if(static_cast<C3DAnimationStateBase*>(result.Object)->GetType()!=(C3DAnimationStateBase::EType)0) return 2;
                const C3DAnimationState* state=static_cast<const C3DAnimationState*>(result.Object);
                if(state->AnimationIndex!=7 || !closeValue(state->AnimationTime,expectedFirst)) return 3;
            } else {
                if(static_cast<C3DAnimationStateBase*>(result.Object)->GetType()!=(C3DAnimationStateBase::EType)2) return 4;
                const C3DAnimationTransitionState* state=static_cast<const C3DAnimationTransitionState*>(result.Object);
                const C3DAnimationState* first=static_cast<const C3DAnimationState*>(state->StartState.Object);
                const C3DAnimationState* second=static_cast<const C3DAnimationState*>(state->EndState.Object);
                if(!first || !second || first==second || first->IVRefCount!=1 || second->IVRefCount!=1) return 5;
                if(first->AnimationIndex!=7 || second->AnimationIndex!=19) return 6;
                if(!closeValue(first->AnimationTime,expectedFirst) || !closeValue(second->AnimationTime,expectedFirst+4.0f)) return 7;
                if(!closeValue(state->Progress,0.25f+0.5f*f)) return 8;
            }
        }
        expectedDestroyed+=n==1?1:3;
        if(fixtureDestroyed!=expectedDestroyed) return 9;
        ++cases;
    }
    if(cases!=72 || fixtureDestroyed!=168) return 10;
    puts("0070c900_TEST PASS");return 0;
}