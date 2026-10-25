// 00515320 std::vector<C2DLineF>::operator=
// Flags: /Gr
// Leaf: ??4?$vector@UC2DLineF@FableAssign_00515320@@@FableAssign_00515320@@QAEAAU01@ABU01@@Z
// Layout: vector3 pointer words; oracle element stride16; trivial destruction, separately bound copy helpers.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableAssign_00515320 {
struct C2DLineF {float X1,Y1,X2,Y2;};
struct __false_type {__false_type(){}};
__declspec(noinline) C2DLineF* __fastcall __uninitialized_copy(const C2DLineF* first,const C2DLineF* last,C2DLineF* result,const __false_type&){for(;first!=last;++first,++result)if(result)new(result)C2DLineF(*first);return result;}
__declspec(noinline) C2DLineF* __fastcall __copy(const C2DLineF* first,const C2DLineF* last,C2DLineF* result,const __false_type&){for(int n=last-first;n>0;--n,++first,++result)*result=*first;return result;}
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
template vector<C2DLineF>& vector<C2DLineF>::operator=(const vector<C2DLineF>&);
}