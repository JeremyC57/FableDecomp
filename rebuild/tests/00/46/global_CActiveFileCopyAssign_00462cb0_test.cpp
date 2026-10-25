// 00462cb0 CActiveFile_CopyAssign
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@Uvalue_type@FileVectorAssignment@@@std@FileVectorAssignment@@QAEAAU012@ABU012@@Z
#include <stdlib.h>
#include <string.h>
#include <new>
#pragma intrinsic(memcpy)
namespace FileVectorAssignment {
// Layout: receiver is a12-byte STLport vector, three-dword POD elements per pointer arithmetic and movsd copies. Semantic element name unresolved.
struct value_type{unsigned Data[3];};
namespace std {
template<class T>__forceinline T*_Copy(T*first,T*last,T*out){for(int n=last-first;n>0;--n,++first,++out)*out=*first;return out;}
template<class T>__forceinline void _UninitializedCopy(T*out,T*first,T*last){while(first!=last){if(out)new(out)T(*first);++first;++out;}}
template<class T>struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;T*_M_allocate_and_copy(unsigned,T*,T*);void _M_clear();vector&operator=(const vector&);};
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
 if(&rhs!=this){unsigned count=rhs._M_finish-rhs._M_start;
  if(count>unsigned(_M_end_of_storage-_M_start)){T*fresh=_M_allocate_and_copy(count,rhs._M_start,rhs._M_finish);_M_clear();_M_start=fresh;_M_end_of_storage=fresh+count;}
  else if(unsigned(_M_finish-_M_start)>=count){_Copy(rhs._M_start,rhs._M_finish,_M_start);}
  else{_Copy(rhs._M_start,rhs._M_start+(_M_finish-_M_start),_M_start);_UninitializedCopy(_M_finish,rhs._M_start+(_M_finish-_M_start),rhs._M_finish);}
  _M_finish=_M_start+count;
 }return *this;
}

template struct vector<value_type>;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace FileVectorAssignment {namespace std {
template<>value_type*vector<value_type>::_M_allocate_and_copy(unsigned n,value_type*a,value_type*b){value_type*p=(value_type*)malloc(n*sizeof(value_type));value_type*q=p;for(;a!=b;++a,++q)new(q)value_type(*a);return p;}
template<>void vector<value_type>::_M_clear(){free(_M_start);}
}}
int main(){using namespace FileVectorAssignment;FileVectorAssignment::std::vector<value_type>a,b;memset(&a,0,sizeof(a));value_type data[5];for(unsigned i=0;i<5;++i)for(unsigned j=0;j<3;++j)data[i].Data[j]=100*i+j;b._M_start=data;b._M_finish=data+3;b._M_end_of_storage=data+5;
a=b;assert(a._M_finish-a._M_start==3&&a._M_end_of_storage-a._M_start==3&&memcmp(a._M_start,data,3*sizeof(value_type))==0);value_type*storage=a._M_start;a=a;assert(a._M_start==storage&&memcmp(a._M_start,data,3*sizeof(value_type))==0);
b._M_finish=data+1;a=b;assert(a._M_start==storage&&a._M_finish==storage+1&&memcmp(a._M_start,data,sizeof(value_type))==0);data[1].Data[1]=999;data[2].Data[2]=888;b._M_finish=data+3;a=b;assert(a._M_start==storage&&a._M_finish==storage+3&&memcmp(a._M_start,data,3*sizeof(value_type))==0);
b._M_finish=data+5;a=b;assert(a._M_finish-a._M_start==5&&a._M_end_of_storage-a._M_start==5&&memcmp(a._M_start,data,5*sizeof(value_type))==0);b._M_finish=data;a=b;assert(a._M_finish==a._M_start);free(a._M_start);puts("00462cb0_TEST PASS");}