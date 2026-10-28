// MicroThread::MicroThreadDetails::~MicroThreadDetails() -- Release owned sequence storage and destroy the base.
// Leaf: ??1MicroThreadDetails@MicroThread@@UAE@XZ
// Identity: Own deleting-dtor009d88b0 and full RTTI129b938 prove MicroThreadDetails; base RTTI129b928 MicroThreadInfo; original Stack+8 vector<unsigned char>.
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std {
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(){_M_start=0;_M_finish=0;_M_end_of_storage._M_data=0;}__forceinline ~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:__forceinline ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();}};
}
template<class T>class CArray {public:std::vector<T> Data;};
// Layout: no active micro-thread header; bool Dead+4 and saved-stack vector+8, confirmed by original stack-size method.
namespace MicroThread {class MicroThreadInfo {public:virtual void MarkAsDead();virtual unsigned int MicroThreadStackSize()const;virtual __forceinline ~MicroThreadInfo(){}bool Dead;};class MicroThreadDetails:public MicroThreadInfo {public:std::vector<unsigned char> Stack;}; }

void DestroyCandidate(MicroThread::MicroThreadDetails* p){p->MicroThreadDetails::~MicroThreadDetails();}