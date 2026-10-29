#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
extern "C" void*__cdecl TestMalloc(unsigned int);extern "C" void __cdecl TestFree(void*);
// Fixture-only CRT tracking preserves actual allocation sizes, guards and ownership.
#define malloc TestMalloc
#define free TestFree
// 00ad4100 std::vector<QuadricOptimiserInternals::COptimisedFace>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@VCOptimisedFace@QuadricOptimiserInternals@@V?$allocator@VCOptimisedFace@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCOptimisedFace@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z
// Flags: /Gr
#include <stdlib.h>
#include <new>
#include "rebuild_abi.h"
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
namespace QuadricOptimiserInternals{class COptimisedFace{public:unsigned long VertexIndices[3],UniqueVertexIndices[3];long Edges[3];C3DVector UTangent,VTangent;bool Exists;unsigned long OutputIndex;long Segment;COptimisedFace();COptimisedFace(const COptimisedFace&);};}
namespace std{struct __false_type{};template<class T>inline const T&max(const T&a,const T&b){return a<b?b:a;}template<class T>class allocator{public:allocator(){}allocator(const allocator&){}T*allocate(unsigned int count){return count?(T*)malloc(count*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};template<class T1,class T2>inline void _Construct(T1*p,const T2&value){new(p)T1(value);}template<class T>inline void _Destroy(T*p){p->~T();}template<class Forward>inline void _Destroy(Forward first,Forward last){for(;first!=last;++first)_Destroy(&*first);}
template<class Input,class Forward>inline Forward __fastcall __uninitialized_copy(Input first,Input last,Forward result,const __false_type&){Forward current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class Forward,class Size,class T>inline Forward __fastcall __uninitialized_fill_n(Forward first,Size count,const T&value,const __false_type&){Forward current=first;for(;count>0;--count,++current)_Construct(&*current,value);return current;}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)this->_M_end_of_storage.deallocate(_M_start,this->_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T*pointer;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}size_type size()const{return this->_M_finish-this->_M_start;}protected:void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}void _M_set(pointer start,pointer finish,pointer end){this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;}void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer position,const T&value,const __false_type&,size_type fill,bool atEnd){const size_type oldSize=size();const size_type length=oldSize+(max)(oldSize,fill);pointer start=this->_M_end_of_storage.allocate(length);pointer finish=start;finish=__uninitialized_copy(this->_M_start,position,start,__false_type());if(fill==1){_Construct(finish,value);++finish;}else finish=__uninitialized_fill_n(finish,fill,value,__false_type());if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());_M_clear();_M_set(start,finish,start+length);}
template void vector<QuadricOptimiserInternals::COptimisedFace>::_M_insert_overflow(QuadricOptimiserInternals::COptimisedFace*,const QuadricOptimiserInternals::COptimisedFace&,const __false_type&,unsigned int,bool);
}
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedFace)==72);

#undef malloc
#undef free

using QuadricOptimiserInternals::COptimisedFace;static unsigned copyCalls,expectedCopies,errors,phase;struct Allocation{unsigned char*raw,*data;unsigned size;bool released,old;};static Allocation allocations[3];static unsigned allocationCount,oldFrees,newFrees;
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount>=3){++errors;return 0;}Allocation&r=allocations[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;r.old=phase==0;memset(r.raw,0xa5,size+32);return r.data;}
extern "C" void __cdecl TestFree(void*p){Allocation*record=0;for(unsigned i=0;i<allocationCount;++i)if(allocations[i].data==p)record=allocations+i;if(!record||record->released){++errors;return;}for(unsigned i=0;i<16;++i)if(record->raw[i]!=0xa5||record->data[record->size+i]!=0xa5)++errors;if(record->old){if(phase!=1||copyCalls!=expectedCopies)++errors;++oldFrees;}else{if(phase!=2)++errors;++newFrees;}record->released=true;free(record->raw);}
COptimisedFace::COptimisedFace(){for(unsigned i=0;i<3;++i){VertexIndices[i]=UniqueVertexIndices[i]=0;Edges[i]=0;}UTangent.X=UTangent.Y=UTangent.Z=VTangent.X=VTangent.Y=VTangent.Z=0;Exists=false;OutputIndex=0;Segment=0;}
COptimisedFace::COptimisedFace(const COptimisedFace&other){if(phase!=1)++errors;++copyCalls;for(unsigned i=0;i<3;++i){VertexIndices[i]=other.VertexIndices[i];UniqueVertexIndices[i]=other.UniqueVertexIndices[i];Edges[i]=other.Edges[i];}memcpy(&UTangent,&other.UTangent,12);memcpy(&VTangent,&other.VTangent,12);Exists=other.Exists;OutputIndex=other.OutputIndex;Segment=other.Segment;}
static void setFace(COptimisedFace&face,unsigned key){for(unsigned i=0;i<3;++i){face.VertexIndices[i]=key*100+i;face.UniqueVertexIndices[i]=0xffffffffUL-key*7-i;face.Edges[i]=(long)key-(long)i-5;}face.UTangent.X=key*0.25f;face.UTangent.Y=-(float)key;face.UTangent.Z=key+0.5f;face.VTangent.X=-key*0.25f;face.VTangent.Y=key+1.0f;face.VTangent.Z=0;face.Exists=(key&1)!=0;face.OutputIndex=0x80000000UL+key;face.Segment=-(long)key;}
static bool equals(const COptimisedFace&face,unsigned key){COptimisedFace expected;setFace(expected,key);return memcmp(face.VertexIndices,expected.VertexIndices,12)==0&&memcmp(face.UniqueVertexIndices,expected.UniqueVertexIndices,12)==0&&memcmp(face.Edges,expected.Edges,12)==0&&memcmp(&face.UTangent,&expected.UTangent,12)==0&&memcmp(&face.VTangent,&expected.VTangent,12)==0&&face.Exists==expected.Exists&&face.OutputIndex==expected.OutputIndex&&face.Segment==expected.Segment;}
class TestVector:public std::vector<COptimisedFace>{public:using std::vector<COptimisedFace>::_M_insert_overflow;void Initialise(COptimisedFace*data,unsigned size,unsigned capacity){this->_M_start=data;this->_M_finish=size?data+size:data;this->_M_end_of_storage._M_data=capacity?data+capacity:data;}COptimisedFace*Data(){return this->_M_start;}unsigned Size()const{return this->_M_finish-this->_M_start;}unsigned Capacity()const{return this->_M_end_of_storage._M_data-this->_M_start;}};
int main(){unsigned cases=0;for(unsigned oldSize=0;oldSize<=8;++oldSize)for(unsigned fill=1;fill<=8;++fill)for(unsigned position=0;position<=oldSize;++position)for(unsigned atEnd=0;atEnd<2;++atEnd)for(unsigned alias=0;alias<2;++alias)for(unsigned spare=0;spare<2;++spare){if((atEnd&&position!=oldSize)||(alias&&!oldSize))continue;allocationCount=copyCalls=errors=oldFrees=newFrees=0;phase=0;unsigned capacity=oldSize+spare;COptimisedFace*old=static_cast<COptimisedFace*>(TestMalloc((capacity?capacity:1)*sizeof(COptimisedFace)));for(unsigned i=0;i<oldSize;++i){new(old+i)COptimisedFace;setFace(old[i],i+1);}COptimisedFace value;setFace(value,99);unsigned valueKey=alias?(oldSize/2+1):99;const COptimisedFace&insertValue=alias?old[oldSize/2]:value;expectedCopies=oldSize+fill;{TestVector vector;vector.Initialise(old,oldSize,capacity);std::__false_type tag;phase=1;COptimisedFace*where=position?old+position:old;vector._M_insert_overflow(where,insertValue,tag,fill,atEnd!=0);if(errors||copyCalls!=expectedCopies||oldFrees!=1U||allocationCount!=2U||vector.Size()!=oldSize+fill||vector.Capacity()!=oldSize+(oldSize<fill?fill:oldSize))return 1;for(unsigned i=0;i<vector.Size();++i){unsigned key=i<position?i+1:i<position+fill?valueKey:i-fill+1;if(!equals(vector.Data()[i],key))return 2;}phase=2;}if(errors||newFrees!=1)return 3;for(unsigned i=0;i<allocationCount;++i)if(!allocations[i].released)return 4;++cases;}if(cases!=1696)return 5;puts("00ad4100_TEST PASS");return 0;}