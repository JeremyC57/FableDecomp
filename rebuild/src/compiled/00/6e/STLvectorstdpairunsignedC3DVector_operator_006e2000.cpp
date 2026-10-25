// 006e2000 std::vector<std::pair<unsigned_long,C3DVector>>::operator=
// Flags: /Gr
// Leaf: ??4?$vector@U?$pair@IUC3DVector@FableAssign_006e2000@@@FableAssign_006e2000@@@FableAssign_006e2000@@QAEAAU01@ABU01@@Z
// Layout: vector3 pointer words; oracle element stride16; trivial destruction, separately bound copy helpers.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableAssign_006e2000 {
struct C3DVector {float X,Y,Z;};
template<class K,class V> struct pair {K first;V second;};
typedef pair<unsigned int,C3DVector> CVectorParam;
struct __false_type {__false_type(){}};
__declspec(noinline) CVectorParam* __fastcall __uninitialized_copy(const CVectorParam* first,const CVectorParam* last,CVectorParam* result,const __false_type&){for(;first!=last;++first,++result)if(result)new(result)CVectorParam(*first);return result;}
__declspec(noinline) CVectorParam* __fastcall __copy(const CVectorParam* first,const CVectorParam* last,CVectorParam* result,const __false_type&){for(int n=last-first;n>0;--n,++first,++result)*result=*first;return result;}
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
template vector<CVectorParam>& vector<CVectorParam>::operator=(const vector<CVectorParam>&);
}