// 00433530 vector<CCharString>::_M_insert_overflow (catalog: std::vector::Reallocate)
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
#include <new>

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct __false_type { __false_type() {} };
struct CCharStringData;struct CCharString {CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
__forceinline void CopyConstruct(CCharString* p,const CCharString& x) { new(p) CCharString(x); }
void* __cdecl Place(unsigned int,void*);
__forceinline void Construct(CCharString* p,const CCharString& x) { if(p) CopyConstruct(p,x); }
__forceinline void Destroy(CCharString* p) {p->~CCharString();}

__forceinline CCharString* __fastcall UninitCopy(CCharString* first,CCharString* last,CCharString* out,const __false_type&) {for(;first!=last;++first,++out) Construct(out,*first);return out;}
__forceinline CCharString* __fastcall UninitFill(CCharString* out,unsigned int n,const CCharString& x,const __false_type&) {for(;n>0;--n,++out) Construct(out,x);return out;}

template<class T> __forceinline void DestroyStorage(T* first,T* last){for(;first!=last;++first)Destroy(first);}
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool); };
template<class T> __forceinline const T& Max(const T& a,const T& b) {return a<b?b:a;}
template<class T> void vector<T>::_M_insert_overflow(T* pos,const T& x,const __false_type&,unsigned int n,bool atend) {
 const unsigned int old_size=_M_finish-_M_start;
 const unsigned int len=old_size+Max(old_size,n);
 T* new_start=len ? (T*)malloc(len*sizeof(T)) : 0;
 T* new_finish=UninitCopy(_M_start,pos,new_start,__false_type());
 if(n==1) { Construct(new_finish,x); ++new_finish; }
 else new_finish=UninitFill(new_finish,n,x,__false_type());
 if(!atend) new_finish=UninitCopy(pos,_M_finish,new_finish,__false_type());
 DestroyStorage(_M_start,_M_finish);
 if(_M_start) free(_M_start);
 _M_finish=new_finish; _M_start=new_start; _M_end_of_storage=new_start+len;
}
template void vector<CCharString>::_M_insert_overflow(CCharString*,const CCharString&,const __false_type&,unsigned int,bool);
