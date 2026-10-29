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
// Fixture-only GNU ABI adaptation: MSVC double aggregate alignment is8, GNU32 default is4.
#ifdef __GNUC__
#define PASS9_QUADRIC_ALIGN __attribute__((aligned(8)))
#else
#define PASS9_QUADRIC_ALIGN
#endif
extern "C" void*__cdecl TestMalloc(unsigned int);extern "C" void __cdecl TestFree(void*);
// Fixture-only tracking leaves the selected candidate's allocator calls and signatures unchanged.
#define malloc TestMalloc
#define free TestFree
// 00ad81f0 std::vector<QuadricOptimiserInternals::COptimisedVertex>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@VCOptimisedVertex@QuadricOptimiserInternals@@V?$allocator@VCOptimisedVertex@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCOptimisedVertex@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z
// Flags: /Gr
#include <stdlib.h>
#include <new>
#include "rebuild_abi.h"
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
namespace QuadricOptimiserInternals{class PASS9_QUADRIC_ALIGN CQuadric{private:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;};
// Layout: native original fields through Cost restored. The unread FaceList/Exists/indices/Groups/PairContractions tail is an opaque implementation projection; copy/destructor remain the exact external original members.
class COptimisedVertex{public:C3DVector Position,Normal,TextureCoord;CQuadric PositionError;float Cost;private:unsigned char unread[52];public:COptimisedVertex();COptimisedVertex(const COptimisedVertex&);~COptimisedVertex();};}
namespace std{struct __false_type{};template<class T>inline const T&max(const T&a,const T&b){return a<b?b:a;}template<class T>class allocator{public:allocator(){}allocator(const allocator&){}T*allocate(unsigned int count){return count?(T*)malloc(count*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};template<class T1,class T2>inline void _Construct(T1*p,const T2&value){new(p)T1(value);}template<class T>inline void _Destroy(T*p){p->~T();}template<class Forward>inline void _Destroy(Forward first,Forward last){for(;first!=last;++first)_Destroy(&*first);}
template<class Input,class Forward>inline Forward __fastcall __uninitialized_copy(Input first,Input last,Forward result,const __false_type&){Forward current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class Forward,class Size,class T>inline Forward __fastcall __uninitialized_fill_n(Forward first,Size count,const T&value,const __false_type&){Forward current=first;for(;count>0;--count,++current)_Construct(&*current,value);return current;}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)this->_M_end_of_storage.deallocate(_M_start,this->_M_end_of_storage._M_data-_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T*pointer;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}size_type size()const{return this->_M_finish-this->_M_start;}protected:void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}void _M_set(pointer start,pointer finish,pointer end){this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;}void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer position,const T&value,const __false_type&,size_type fill,bool atEnd){const size_type oldSize=size();const size_type length=oldSize+(max)(oldSize,fill);pointer start=this->_M_end_of_storage.allocate(length);pointer finish=start;finish=__uninitialized_copy(this->_M_start,position,start,__false_type());if(fill==1){_Construct(finish,value);++finish;}else finish=__uninitialized_fill_n(finish,fill,value,__false_type());if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());_M_clear();_M_set(start,finish,start+length);}
template void vector<QuadricOptimiserInternals::COptimisedVertex>::_M_insert_overflow(QuadricOptimiserInternals::COptimisedVertex*,const QuadricOptimiserInternals::COptimisedVertex&,const __false_type&,unsigned int,bool);
}
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedVertex)==184);

#undef malloc
#undef free

using QuadricOptimiserInternals::COptimisedVertex;static unsigned copyCalls,expectedCopies,destroyedOld,errors,phase;
struct Allocation{unsigned char*raw,*data;unsigned size;bool released,old;};static Allocation allocations[3];static unsigned allocationCount,oldFrees,newFrees;
// External original ctor/dtor fixtures model independently owned payloads in an address ledger. Target calls, order, and lifetimes are exercised; the separate deep-tree copy implementation is outside this row.
struct ObjectRecord{const COptimisedVertex*object;unsigned char*resource;unsigned key,role;bool live;};static ObjectRecord objects[64];static unsigned objectCount,resourceCreates,resourceReleases;
static ObjectRecord*findObject(const COptimisedVertex*object){for(unsigned i=0;i<objectCount;++i)if(objects[i].object==object&&objects[i].live)return objects+i;return 0;}
static ObjectRecord*createObject(const COptimisedVertex*object,unsigned key,unsigned role){if(objectCount==64){++errors;return 0;}ObjectRecord*r=objects+objectCount++;r->object=object;r->key=key;r->role=role;r->live=true;r->resource=static_cast<unsigned char*>(malloc(32));for(unsigned i=0;i<32;++i)r->resource[i]=(unsigned char)(key+i*7);++resourceCreates;return r;}
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount>=3){++errors;return 0;}Allocation&r=allocations[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;r.old=phase==0;memset(r.raw,0xa5,size+32);return r.data;}
extern "C" void __cdecl TestFree(void*p){Allocation*record=0;for(unsigned i=0;i<allocationCount;++i)if(allocations[i].data==p)record=allocations+i;if(!record||record->released){++errors;return;}for(unsigned i=0;i<16;++i)if(record->raw[i]!=0xa5||record->data[record->size+i]!=0xa5)++errors;if(record->old){if(phase!=1||copyCalls!=expectedCopies)++errors;for(unsigned i=0;i<objectCount;++i)if(objects[i].role==0&&objects[i].live)++errors;++oldFrees;}else{if(phase!=2)++errors;for(unsigned i=0;i<objectCount;++i)if(objects[i].role==1&&objects[i].live)++errors;++newFrees;}record->released=true;free(record->raw);}
COptimisedVertex::COptimisedVertex(){memset(this,0,sizeof(*this));createObject(this,0,2);}
COptimisedVertex::COptimisedVertex(const COptimisedVertex&other){ObjectRecord*r=findObject(&other);if(!r||phase!=1||destroyedOld){++errors;return;}for(unsigned i=0;i<32;++i)if(r->resource[i]!=(unsigned char)(r->key+i*7))++errors;memcpy(this,&other,sizeof(*this));createObject(this,r->key,1);++copyCalls;}
COptimisedVertex::~COptimisedVertex(){ObjectRecord*r=findObject(this);if(!r){++errors;return;}if(r->role==0){if(phase!=1||copyCalls!=expectedCopies)++errors;++destroyedOld;}else if(r->role==1&&phase!=2)++errors;for(unsigned i=0;i<32;++i)if(r->resource[i]!=(unsigned char)(r->key+i*7))++errors;free(r->resource);r->live=false;++resourceReleases;}
static void setVertex(COptimisedVertex&vertex,unsigned key,unsigned role){unsigned char*bytes=reinterpret_cast<unsigned char*>(&vertex);for(unsigned i=0;i<sizeof(vertex);++i)bytes[i]=(unsigned char)(key*11+i*13);ObjectRecord*r=findObject(&vertex);r->key=key;r->role=role;for(unsigned i=0;i<32;++i)r->resource[i]=(unsigned char)(key+i*7);}
static bool equals(const COptimisedVertex&vertex,unsigned key){const unsigned char*bytes=reinterpret_cast<const unsigned char*>(&vertex);for(unsigned i=0;i<sizeof(vertex);++i)if(bytes[i]!=(unsigned char)(key*11+i*13))return false;ObjectRecord*r=findObject(&vertex);return r&&r->key==key&&r->role==1;}
class TestVector:public std::vector<COptimisedVertex>{public:using std::vector<COptimisedVertex>::_M_insert_overflow;void Initialise(COptimisedVertex*data,unsigned size,unsigned capacity){this->_M_start=data;this->_M_finish=data+size;this->_M_end_of_storage._M_data=data+capacity;}COptimisedVertex*Data(){return this->_M_start;}unsigned Size()const{return this->_M_finish-this->_M_start;}unsigned Capacity()const{return this->_M_end_of_storage._M_data-this->_M_start;}};
int main(){unsigned cases=0;for(unsigned oldSize=0;oldSize<=8;++oldSize)for(unsigned fill=1;fill<=8;++fill)for(unsigned position=0;position<=oldSize;++position)for(unsigned atEnd=0;atEnd<2;++atEnd)for(unsigned alias=0;alias<2;++alias)for(unsigned spare=0;spare<2;++spare){if((atEnd&&position!=oldSize)||(alias&&!oldSize))continue;allocationCount=copyCalls=destroyedOld=errors=oldFrees=newFrees=objectCount=resourceCreates=resourceReleases=0;phase=0;unsigned capacity=oldSize+spare;COptimisedVertex*old=static_cast<COptimisedVertex*>(TestMalloc((capacity?capacity:1)*sizeof(COptimisedVertex)));for(unsigned i=0;i<oldSize;++i){new(old+i)COptimisedVertex;setVertex(old[i],i+1,0);} {COptimisedVertex value;setVertex(value,99,2);unsigned valueKey=alias?(oldSize/2+1):99;const COptimisedVertex&insertValue=alias?old[oldSize/2]:value;expectedCopies=oldSize+fill;{TestVector vector;vector.Initialise(old,oldSize,capacity);std::__false_type tag;phase=1;vector._M_insert_overflow(old+position,insertValue,tag,fill,atEnd!=0);if(errors||copyCalls!=expectedCopies||destroyedOld!=oldSize||oldFrees!=1||allocationCount!=2||vector.Size()!=oldSize+fill||vector.Capacity()!=oldSize+(oldSize<fill?fill:oldSize))return 1;for(unsigned i=0;i<vector.Size();++i){unsigned key=i<position?i+1:i<position+fill?valueKey:i-fill+1;if(!equals(vector.Data()[i],key))return 2;}for(unsigned i=0;i<objectCount;++i)for(unsigned j=i+1;j<objectCount;++j)if(objects[i].live&&objects[j].live&&objects[i].resource==objects[j].resource)return 3;phase=2;}if(errors||newFrees!=1)return 4;}if(errors||resourceCreates!=resourceReleases)return 5;for(unsigned i=0;i<allocationCount;++i)if(!allocations[i].released)return 6;++cases;}if(cases!=1696)return 7;puts("00ad81f0_TEST PASS");return 0;}