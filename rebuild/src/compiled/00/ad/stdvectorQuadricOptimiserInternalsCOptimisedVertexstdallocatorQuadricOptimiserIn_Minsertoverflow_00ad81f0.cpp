// 00ad81f0 std::vector<QuadricOptimiserInternals::COptimisedVertex>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@VCOptimisedVertex@QuadricOptimiserInternals@@V?$allocator@VCOptimisedVertex@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCOptimisedVertex@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z
// Flags: /Gr
#include <stdlib.h>
#include <new>
#include "rebuild_abi.h"
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
namespace QuadricOptimiserInternals{class CQuadric{private:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;};
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