// 00af7b60 NParticleEngine::CParticle::~CParticle
// Leaf: ??1CParticle@NParticleEngine@@UAE@XZ
// Flags:
#include <stdlib.h>
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{CBaseClassNonCopyable(const CBaseClassNonCopyable&);protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
// Layout: handle private counted-pointer storage is unobserved; destruction is external.
class CEnginePrimitiveHandle:public CBaseClass{unsigned char unreadData[4];public:virtual ~CEnginePrimitiveHandle();};
namespace std{
template<class T>class allocator{public:allocator(){}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
template<class T>inline void _Destroy(T*p){p->~T();}template<class Forward>inline void _Destroy(Forward first,Forward last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,(unsigned int)(_M_end_of_storage._M_data-_M_start));}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}};
}
// Layout: native CArray storage/lifetime is its twelve-byte STLport vector base.
template<class T>class CArray:public std::vector<T>{};
namespace boost{template<class T>class scoped_ptr{T*ptr;scoped_ptr(const scoped_ptr&);public:explicit scoped_ptr(T*p=0):ptr(p){}~scoped_ptr(){delete ptr;}};}
namespace NParticleEngine{
// Layout: random-acceleration state is unread; raw original base and virtual slots retained.
class CRandomAcceleration:public CBaseClassNonCopyable{unsigned char unread[20];public:virtual ~CRandomAcceleration();virtual void Delete();};
class CParticle:public CBaseClassNonCopyable{unsigned char unreadPrefix[124];boost::scoped_ptr<CRandomAcceleration>RandomAcceleration;CArray<CArray<long> >PrimIDList;CArray<CEnginePrimitiveHandle>ParentHandleList;
#ifdef PASS9_TEST
friend int ::main();
#endif
public:virtual ~CParticle();virtual void Delete();};
CParticle::~CParticle(){}
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticle)==156);FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);