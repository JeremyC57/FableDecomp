// 006cade0 _STL::vector<CCompositeBlendDesc>::operator=
// Flags: /Gr
// Leaf: ??4?$vector@UCCompositeBlendDesc@FableAssign_006cade0@@@FableAssign_006cade0@@QAEAAU01@ABU01@@Z
// Layout: vector3 pointer words; oracle element stride16; trivial destruction, separately bound copy helpers.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableAssign_006cade0 {
struct CCompositeBlendDesc {unsigned int TextureIndex,BlendType,DiffuseColour,SpecularColour;};
struct __false_type {__false_type(){}};
__declspec(noinline) CCompositeBlendDesc* __fastcall __uninitialized_copy(const CCompositeBlendDesc* first,const CCompositeBlendDesc* last,CCompositeBlendDesc* result,const __false_type&){for(;first!=last;++first,++result)if(result)new(result)CCompositeBlendDesc(*first);return result;}
__declspec(noinline) CCompositeBlendDesc* __fastcall __copy(const CCompositeBlendDesc* first,const CCompositeBlendDesc* last,CCompositeBlendDesc* result,const __false_type&){for(int n=last-first;n>0;--n,++first,++result)*result=*first;return result;}
template<class T> struct vector {
 T* start;T* finish;T* end;
 unsigned int size()const{return finish-start;}
 unsigned int capacity()const{return end-start;}
 __forceinline T* _M_allocate_and_copy(unsigned int n,const T* first,const T* last){T* p=n?(T*)malloc(n*sizeof(T)):0;__uninitialized_copy(first,last,p,__false_type());return p;}
 __forceinline void _M_clear(){if(start)free(start);}
 vector& operator=(const vector& x);
};
template<class T> vector<T>& vector<T>::operator=(const vector<T>& x){
 if(&x!=this){
  const unsigned int n=x.size();
  if(n>capacity()){
   T* tmp=_M_allocate_and_copy(n,x.start,x.finish);_M_clear();start=tmp;end=start+n;
  }else if(size()>=n){
   __copy(x.start,x.finish,start,__false_type());
  }else{
   __copy(x.start,x.start+size(),start,__false_type());
   __uninitialized_copy(x.start+size(),x.finish,finish,__false_type());
  }
  finish=start+n;
 }
 return *this;
}
template vector<CCompositeBlendDesc>& vector<CCompositeBlendDesc>::operator=(const vector<CCompositeBlendDesc>&);
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
int main(){using namespace FableAssign_006cade0;
vector<CCompositeBlendDesc> a,b;memset(&a,0,sizeof a);memset(&b,0,sizeof b);b.start=(CCompositeBlendDesc*)malloc(5*sizeof(CCompositeBlendDesc));b.finish=b.start+5;b.end=b.finish;
for(int i=0;i<5;++i){unsigned char* p=(unsigned char*)&b.start[i];for(unsigned int j=0;j<sizeof(CCompositeBlendDesc);++j)p[j]=(unsigned char)(i*16+j);}
a=b;assert(a.size()==5 && a.capacity()==5 && memcmp(a.start,b.start,5*sizeof(CCompositeBlendDesc))==0);CCompositeBlendDesc* first=a.start;a=a;assert(a.start==first && a.size()==5);
b.finish=b.start+2;a=b;assert(a.start==first && a.size()==2 && memcmp(a.start,b.start,2*sizeof(CCompositeBlendDesc))==0);
b.finish=b.start+4;a=b;assert(a.start==first && a.size()==4 && memcmp(a.start,b.start,4*sizeof(CCompositeBlendDesc))==0);
b.finish=b.start;a=b;assert(a.size()==0 && a.start==first);free(a.start);free(b.start);puts("006cade0_TEST PASS");return 0;}