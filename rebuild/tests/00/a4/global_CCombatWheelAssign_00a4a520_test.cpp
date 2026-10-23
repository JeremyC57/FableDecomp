// 00a4a520 CCombatWheel_Assign
// Flags: /O2 /Oy /W3 
// Leaf: ??4?$vector@$$BY02I@_STL@VectorAssignment_00a4a520@@QAEAAU012@ABU012@@Z
#include <stdlib.h>
namespace VectorAssignment_00a4a520 {
// Layout: STLport vector receiver stores start,finish,end at+0,+4,+8. Retail stride is 12 bytes; element semantics remain unresolved, so represent only its dwords.
typedef unsigned value_type[3];
struct __false_type{__false_type(){}};
template<class Tag> __declspec(noinline) value_type* __fastcall uninitialized_copy(value_type*a,value_type*b,value_type*c,const Tag&){while(a!=b){for(unsigned i=0;i<sizeof(value_type)/sizeof(unsigned);++i)(*c)[i]=(*a)[i];++a;++c;}return c;}
template<class Tag> __declspec(noinline) value_type* __fastcall _Copy(value_type*a,value_type*b,value_type*c,const Tag&,unsigned){while(a!=b){for(unsigned i=0;i<sizeof(value_type)/sizeof(unsigned);++i)(*c)[i]=(*a)[i];++a;++c;}return c;}
void __fastcall _Destroy(value_type*,value_type*);
namespace _STL {
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

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace VectorAssignment_00a4a520 {
void __fastcall _Destroy(value_type*,value_type*){}
namespace _STL{template<>void vector<value_type>::_M_clear(){free(_M_start);}}
}
int main(){using namespace VectorAssignment_00a4a520;
 _STL::vector<value_type>a,b;memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));
 value_type data[3];for(unsigned i=0;i<3;++i)for(unsigned j=0;j<sizeof(value_type)/sizeof(unsigned);++j)data[i][j]=101*i+j+1;
 b._M_start=data;b._M_finish=data+3;b._M_end_of_storage=data+3;
 assert(&(a=b)==&a);assert(a._M_finish-a._M_start==3&&a._M_end_of_storage-a._M_start==3&&memcmp(a._M_start,data,sizeof(data))==0);
 value_type*storage=a._M_start;assert(&(a=a)==&a&&a._M_start==storage&&memcmp(a._M_start,data,sizeof(data))==0);
 b._M_finish=data+1;data[0][0]=701;a=b;assert(a._M_start==storage&&a._M_finish==storage+1&&memcmp(storage,data,sizeof(value_type))==0);
 b._M_finish=data+3;data[1][0]=801;data[2][0]=901;a=b;assert(a._M_start==storage&&a._M_finish==storage+3&&memcmp(storage,data,sizeof(data))==0);
 b._M_finish=data;a=b;assert(a._M_start==storage&&a._M_finish==storage&&a._M_end_of_storage==storage+3);
 free(a._M_start);puts("00a4a520_TEST PASS");return 0;}