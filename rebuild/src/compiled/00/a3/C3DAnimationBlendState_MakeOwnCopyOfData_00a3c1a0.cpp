// 00a3c1a0 C3DAnimationBlendState::MakeOwnCopyOfData
// Leaf: ?MakeOwnCopyOfData@C3DAnimationBlendState@@AAEXXZ
// Flags:
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#define CIVCountedPointeeBase CIVCountedPointeeBaseLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
// Layout: import the flattened base and restore its original constructor and virtual interface.
// Header debt: the supplied opt-in omits the proven default constructor that sets IVRefCount to one.
class CIVCountedPointeeBase {
public:CIVCountedPointeeBase():IVRefCount(1){}virtual~CIVCountedPointeeBase(){}virtual void Delete()const;
 void AddRef()const{++IVRefCount;}long Release()const{return --IVRefCount;}long GetRefCount()const{return IVRefCount;}
 mutable unsigned long IVRefCount;
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBaseLayout));
#include "engine/CCountedPointer.h"
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base();};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:vector(const A&a=A()):_Vector_base<T,A>(a){}~vector();};
}
template<class T>class CArray:public std::vector<T>{};
template<class T>class CIVCountedPointer {
private:CIVCountedPointeeBase*Object;
public:
 CIVCountedPointer():Object(0){}
 CIVCountedPointer(T*p):Object(static_cast<CIVCountedPointeeBase*>(p)){}
 CIVCountedPointer(const CIVCountedPointer&p):Object(p.Object){if(Object)Object->AddRef();}
 ~CIVCountedPointer(){Release();}
 void Release(){CIVCountedPointeeBase*p=Object;if(p){if(p->Release()==0)p->Delete();Object=0;}}
 void Reset(T*p){Release();Object=static_cast<CIVCountedPointeeBase*>(p);}
 CIVCountedPointer&operator=(const CIVCountedPointer&p){if(Object!=p.Object){Release();Object=p.Object;if(Object)Object->AddRef();}return *this;}
 bool IsNull()const{return Object==0;}
 unsigned long GetRefCount()const{return Object?Object->GetRefCount():0;}
 T*Get()const{return static_cast<T*>(Object);}
 T&operator*()const{return *Get();}
};
class C3DAnimationStateBase;class C3DAnimationStateAlpha;class C3DSkeletalMorph;
// Layout: original CBaseClass parent and Data at4; private nested class owns the count/vector/morph.
class C3DAnimationBlendState:public CBaseClass {
private:
 class CSharedData:public CIVCountedPointeeBase{
 public:CIVCountedPointer<C3DAnimationStateBase>BaseAnimation;CArray<C3DAnimationStateAlpha>CombinationBlends;CCountedPointer<const C3DSkeletalMorph>SkeletalMorph;
 CSharedData(){}
 CSharedData(const CSharedData&);virtual~CSharedData();
 };
 CIVCountedPointer<CSharedData>Data;
 void MakeOwnCopyOfData();
};
void C3DAnimationBlendState::MakeOwnCopyOfData(){
 if(Data.IsNull())Data.Reset(new CSharedData);
 else if(Data.GetRefCount()>1){CIVCountedPointer<CSharedData>copy(new CSharedData(*Data));Data=copy;}
}