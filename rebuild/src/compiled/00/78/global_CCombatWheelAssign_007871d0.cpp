// 007871d0 CCombatWheel_Assign
// Flags: /O2 /Oy /W3 
// Leaf: ??4?$vector@$$BY02I@std@VectorAssignment_007871d0@@QAEAAU012@ABU012@@Z
#include <stdlib.h>
namespace VectorAssignment_007871d0 {
// Layout: STLport vector receiver stores start,finish,end at+0,+4,+8. Retail stride is 12 bytes; element semantics remain unresolved, so represent only its dwords.
typedef unsigned value_type[3];
struct __false_type{__false_type(){}};
template<class Tag> __declspec(noinline) value_type* __fastcall uninitialized_copy(value_type*a,value_type*b,value_type*c,const Tag&){while(a!=b){for(unsigned i=0;i<sizeof(value_type)/sizeof(unsigned);++i)(*c)[i]=(*a)[i];++a;++c;}return c;}
template<class Tag> __declspec(noinline) value_type* __fastcall _Copy(value_type*a,value_type*b,value_type*c,const Tag&,unsigned){while(a!=b){for(unsigned i=0;i<sizeof(value_type)/sizeof(unsigned);++i)(*c)[i]=(*a)[i];++a;++c;}return c;}
void __fastcall _Destroy(value_type*,value_type*);
namespace std {
template<class T>struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;void _M_clear();vector&operator=(const vector&);};
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
 if(&rhs!=this){T*last=rhs._M_finish;T*first=rhs._M_start;unsigned count=last-first;
  if(count>unsigned(_M_end_of_storage-_M_start)){
   T*fresh=count?(T*)malloc(count*sizeof(T)):0;__false_type tag;uninitialized_copy(first,last,fresh,tag);_M_clear();_M_start=fresh;_M_end_of_storage=fresh+count;
  }else if(unsigned(_M_finish-_M_start)>=count){__false_type tag;T*out=_Copy(first,last,_M_start,tag,0);_Destroy(out,_M_finish);
  }else{__false_type tag;_Copy(first,first+(_M_finish-_M_start),_M_start,tag,0);uninitialized_copy(rhs._M_start+(_M_finish-_M_start),rhs._M_finish,_M_finish,tag);}
  _M_finish=_M_start+count;
 }return *this;
}
template struct vector<value_type>;
}
}