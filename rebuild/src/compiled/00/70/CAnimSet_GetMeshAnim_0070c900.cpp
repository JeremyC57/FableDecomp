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