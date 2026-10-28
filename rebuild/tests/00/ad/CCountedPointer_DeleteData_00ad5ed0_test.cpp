#include <stdio.h>
#include <stdlib.h>
#include <new>
static int buffers,objects,errors;static void* expectedBuffer;static void* expectedObject;
static void tracked_free(void* p){if(p!=expectedBuffer)++errors;++buffers;free(p);}
void operator delete(void* p)throw(){if(p!=expectedObject)++errors;++objects;free(p);}
#define free tracked_free
// CCountedPointer::DeleteData(void* object) -- Delete the typed pointee and its owned plain-element array.
// Leaf: ?DeleteData@?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@SIXPAX@Z
// Identity: Own allocation00acbfc7 size40 and field stores Exists0/Unsplit1/SubContractions4/ListIterator36 bind callback00acc015; original nested type/PDB DeleteData. ListIterator unaccessed single-node-pointer projection.
// Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:~vector(){if(this->_M_start)free(this->_M_start);}};}
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer{public:static void __fastcall DeleteData(void* object){delete static_cast<T*>(object);}};
namespace QuadricOptimiserInternals{class CSubContraction{public:unsigned long Index1,Index2;};class CVertexPairContraction{public:bool Exists,Unsplit;CArray<CSubContraction> SubContractions;float Weight2,Cost;float ResultingPosition[3];void* ListIterator;};}
template void __fastcall CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>::DeleteData(void*);

#undef free
int main(){typedef QuadricOptimiserInternals::CVertexPairContraction Object;CCountedPointer<Object>::DeleteData(0);bool ok=buffers==0&&objects==0;Object* one=new Object;expectedObject=one;CCountedPointer<Object>::DeleteData(one);ok=ok&&buffers==0&&objects==1;Object* two=new Object;two->SubContractions._M_start=(QuadricOptimiserInternals::CSubContraction*)malloc(3*sizeof(QuadricOptimiserInternals::CSubContraction));two->SubContractions._M_finish=two->SubContractions._M_start+2;two->SubContractions._M_end_of_storage._M_data=two->SubContractions._M_start+3;expectedBuffer=two->SubContractions._M_start;expectedObject=two;CCountedPointer<Object>::DeleteData(two);ok=ok&&buffers==1&&objects==2&&!errors;puts(ok?"00ad5ed0_TEST PASS":"00ad5ed0_TEST FAIL");return ok?0:1;}