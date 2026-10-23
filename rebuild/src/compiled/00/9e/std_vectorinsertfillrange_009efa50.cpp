// 009efa50 N009efa50::vector<Element>::_M_insert_overflow (STLport; catalog: vector_insert_fill_range)
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>
namespace N009efa50 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct __false_type { __false_type() {} };
struct CCharStringData;struct CCharString {CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element {Element(){}virtual void dtor(unsigned int);unsigned int W0;CCharString S;unsigned int A,B;unsigned char Flag;unsigned int C;__forceinline Element(const Element& o):W0(o.W0),S(o.S),A(o.A),B(o.B),Flag(o.Flag),C(o.C){}};
void* __cdecl Place(unsigned int,void*);
__forceinline void _Construct(Element* p,const Element& x) {if(p) new(p) Element(x);}
__forceinline void _Destroy(Element* p) {p->dtor(0);}
#pragma auto_inline(off)

__declspec(noinline) Element* __fastcall __uninitialized_copy(Element* first,Element* last,Element* out,const __false_type&) {for(;first!=last;++first,++out) _Construct(out,*first);return out;}
__declspec(noinline) Element* __fastcall __uninitialized_fill_n(Element* out,unsigned int n,const Element& x,const __false_type&) {for(;n>0;--n,++out) _Construct(out,x);return out;}

#pragma auto_inline(on)
template<class T> __forceinline void DestroyStorage(T* first,T* last){for(;first!=last;++first)_Destroy(first);}
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool); };
template<class T> __forceinline const T& Max(const T& a,const T& b) {return a<b?b:a;}
template<class T> void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend) {
 const unsigned int old_size=_M_finish-_M_start;
 const unsigned int len=old_size+Max(old_size,n);
 T* new_start=len ? (T*)malloc(len*sizeof(T)) : 0;
 T* new_finish=__uninitialized_copy(_M_start,pos,new_start,__false_type());
 if(n==1) { _Construct(new_finish,x); ++new_finish; }
 else new_finish=__uninitialized_fill_n(new_finish,n,x,__false_type());
 if(!atend) new_finish=__uninitialized_copy(pos,_M_finish,new_finish,__false_type());
 DestroyStorage(_M_start,_M_finish);
 if(_M_start) free(_M_start);
 _M_finish=new_finish; _M_start=new_start; _M_end_of_storage=new_start+len;
}
template void vector<Element>::_M_insert_overflow(Element*,const Element&,const __false_type&,unsigned int,bool);

}