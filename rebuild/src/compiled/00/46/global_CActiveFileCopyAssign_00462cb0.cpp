// 00462cb0 CActiveFile_CopyAssign
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@Uvalue_type@FileVectorAssignment@@@_STL@FileVectorAssignment@@QAEAAU012@ABU012@@Z
#include <stdlib.h>
#include <string.h>
#include <new>
#pragma intrinsic(memcpy)
namespace FileVectorAssignment {
// Layout: receiver is a12-byte STLport vector, three-dword POD elements per pointer arithmetic and movsd copies. Semantic element name unresolved.
struct value_type{unsigned Data[3];};
namespace _STL {
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