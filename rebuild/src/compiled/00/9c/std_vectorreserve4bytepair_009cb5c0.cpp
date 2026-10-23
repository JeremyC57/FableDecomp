// 009cb5c0 _STL::vector<CIVCountedPointer<CIVCountedPointeeBase>>::_M_insert_overflow
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@U?$CIVCountedPointer@UCIVCountedPointeeBase@@@FableIVVector_009cb5c0@@@FableIVVector_009cb5c0@@QAEXPAU?$CIVCountedPointer@UCIVCountedPointeeBase@@@2@ABU32@ABU__false_type@2@I_N@Z
// Layout: vector3 pointers; CIVCountedPointer is one pointer, PDB struct_layouts_egor.tsv12177-12178; IVRefCount+4 and DeleteThis slot1.
#include "engine/CIVCountedPointeeBase.h"
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableIVVector_009cb5c0 {
struct __false_type {__false_type(){}};
typedef void (__fastcall *CIVDeleteThis)(CIVCountedPointeeBase*);
template<class T> struct CIVCountedPointer {
 CIVCountedPointeeBase* Object;
 CIVCountedPointer():Object(0){}
 __forceinline CIVCountedPointer(const CIVCountedPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CIVCountedPointer(){if(Object && --Object->IVRefCount==0)((CIVDeleteThis*)Object->__vftable)[1](Object);}
};
typedef CIVCountedPointer<CIVCountedPointeeBase> CSharedPointee;
__forceinline CSharedPointee* __uninitialized_copy_aux(CSharedPointee* first,CSharedPointee* last,CSharedPointee* out){for(;first!=last;++first,++out)if(out)new(out)CSharedPointee(*first);return out;}
__forceinline CSharedPointee* __uninitialized_fill_n_aux(CSharedPointee* out,unsigned int count,const CSharedPointee& value){for(;count>0;--count,++out)if(out)new(out)CSharedPointee(value);return out;}
__declspec(noinline) CSharedPointee* __fastcall __uninitialized_copy(const CSharedPointee* first,const CSharedPointee* last,CSharedPointee* out,const __false_type&) {
 for(;first!=last;++first,++out)if(out)new(out)CSharedPointee(*first);return out;
}
__declspec(noinline) void __fastcall _Destroy(CSharedPointee* first,CSharedPointee* last,const __false_type&){for(;first!=last;++first)first->~CSharedPointee();}
template<class T> __forceinline const T& max(const T&a,const T&b){return a<b?b:a;}
template<class T> struct vector {
 T* start;T* finish;T* end;
 void _M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd);
};
template<class T> void vector<T>::_M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd){
 const unsigned int oldSize=finish-start;const unsigned int length=oldSize+max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=__uninitialized_copy_aux(start,position,newStart);
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=__uninitialized_fill_n_aux(newFinish,count,value);
 if(!atEnd)newFinish=__uninitialized_copy(position,finish,newFinish,__false_type());
 _Destroy(start,finish,__false_type());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void vector<CSharedPointee>::_M_insert_overflow(CSharedPointee*,const CSharedPointee&,const __false_type&,unsigned int,bool);
}