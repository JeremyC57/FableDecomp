// 00618420 N00618420::vector<Element>::_M_insert_overflow (STLport; catalog: vector_insert_40byte)
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
namespace N00618420 {

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* __cdecl operator_new_place(unsigned int,void* p) {return p;}
struct __false_type { __false_type() {} };
struct Element { unsigned int W0; unsigned int Data[9]; };
void* __cdecl Place(unsigned int,void*);
__forceinline void _Construct(Element* p,const Element& x) { if(p) *p=x; }
__forceinline void _Destroy(Element*) {}

__forceinline Element* __fastcall __uninitialized_copy(Element* first,Element* last,Element* out,const __false_type&) {for(;first!=last;++first,++out) _Construct(out,*first);return out;}
__forceinline Element* __fastcall __uninitialized_fill_n(Element* out,unsigned int n,const Element& x,const __false_type&) {for(;n>0;--n,++out) _Construct(out,x);return out;}

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
 for(T* p=_M_start;p!=_M_finish;++p) _Destroy(p);
 if(_M_start) free(_M_start);
 _M_finish=new_finish; _M_start=new_start; _M_end_of_storage=new_start+len;
}
template void vector<Element>::_M_insert_overflow(Element*,const Element&,const __false_type&,unsigned int,bool);

}

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace N00618420 {  }
int main() {using namespace N00618420;
 Element* a=(Element*)std::malloc(5*sizeof(Element)); std::memset(a,0,5*sizeof(Element)); for(int i=0;i<4;++i) a[i].W0=i+1;
 Element* x=(Element*)std::malloc(sizeof(Element)); std::memset(x,0,sizeof(Element)); x->W0=99;
 vector<Element> v={a,a+4,a+5}; v._M_insert_overflow(a+2,*x,__false_type(),2,false);
 unsigned int want[]={1,2,99,99,3,4}; if(v._M_finish-v._M_start!=6 || v._M_end_of_storage-v._M_start!=8) return 1;
 for(int j=0;j<6;++j) if(v._M_start[j].W0!=want[j]) return 2;
 v._M_insert_overflow(v._M_finish,*x,__false_type(),1,true); if(v._M_finish-v._M_start!=7||v._M_start[6].W0!=99)return 3;
 std::free(v._M_start);std::free(x); std::printf("00618420_TEST PASS\n");return 0;
}