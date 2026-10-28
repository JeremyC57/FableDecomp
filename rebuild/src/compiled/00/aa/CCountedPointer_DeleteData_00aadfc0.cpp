// CCountedPointer::DeleteData(void* object) -- Delete the typed pointee and its owned plain-element array.
// Leaf: ?DeleteData@?$CCountedPointer@VCWaveFile@@@@SIXPAX@Z
// Identity: Own LoadWaveFile00aad650 allocates24 bytes, Channels/SampleLength/SamplingFrequency and AudioData at12; Reset00aadff0 stores this callback. Exact original CCountedPointer<CWaveFile>::DeleteData PDB name.
// Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:~vector(){if(this->_M_start)free(this->_M_start);}};}
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer{public:static void __fastcall DeleteData(void* object){delete static_cast<T*>(object);}};
class CWaveFile{public:unsigned long Channels,SampleLength,SamplingFrequency;CArray<short> AudioData;};
template void __fastcall CCountedPointer<CWaveFile>::DeleteData(void*);