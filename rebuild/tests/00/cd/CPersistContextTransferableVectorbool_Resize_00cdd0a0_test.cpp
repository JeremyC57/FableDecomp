#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00cdd0a0 CPersistContext_TransferableVector<bool>::Resize
// Flags:
#ifndef _MSC_VER
#define __declspec(x)
#endif
// Leaf: ?Resize@?$CPersistContext_TransferableVector@_N@@EAEXJ@Z
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
// Layout: actual467686/ CDD040 helpers operate on byte element pointers.
// This models the retail byte-bool instantiation, not donor bit-vector storage.
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
 T*erase(T*,T*);
 void _M_fill_insert(T*,unsigned int,const T&);
 __declspec(noinline) void resize(unsigned int n,const T&value){
  if(n<(unsigned int)(this->_M_finish-this->_M_start))erase(this->_M_start+n,this->_M_finish);
  else _M_fill_insert(this->_M_finish,n-(unsigned int)(this->_M_finish-this->_M_start),value);
 }
};
}
template<class T>class CPersistTraits{public:T DefaultVal;CPersistTraits():DefaultVal(T()) {}};
class CPersistContext_TransferableVectorGenericType{public:virtual void Resize(long)=0;};
// Layout: original generic vector interface at0; TransVector pointer at4.
template<class T>class CPersistContext_TransferableVector:public CPersistContext_TransferableVectorGenericType{
 std::vector<T>*TransVector;
 virtual void Resize(long n){CPersistTraits<T>traits;TransVector->resize(n,traits.DefaultVal);}
};
template class CPersistContext_TransferableVector<bool>;

namespace std{
template<class T,class A>T*vector<T,A>::erase(T*first,T*last){T*out=first;T*end=this->_M_finish;while(last!=end)*out++=*last++;this->_M_finish=out;return first;}
template<class T,class A>void vector<T,A>::_M_fill_insert(T*pos,unsigned int count,const T&value){
 if(pos!=this->_M_finish||count>(unsigned int)(this->_M_end_of_storage._M_data-this->_M_finish))abort();
 while(count--)*this->_M_finish++=value;
}
}
int main(){
 bool storage[67];std::vector<bool>data;CPersistContext_TransferableVector<bool>subject;
 if(sizeof(data)!=12||sizeof(subject)!=8)abort();
 std::vector<bool>*ptr=&data;memcpy(reinterpret_cast<char*>(&subject)+4,&ptr,4);
 unsigned int cases=0;
 for(int pattern=0;pattern<3;++pattern)for(int old=0;old<=64;++old)for(int size=0;size<=64;++size){
  for(int j=0;j<67;++j)storage[j]=(pattern==0?false:pattern==1?true:(j%2)!=0);
  storage[0]=true;storage[65]=true;storage[66]=true;
  bool before[67];memcpy(before,storage,sizeof(storage));
  bool*bounds[3]={storage+1,storage+1+old,storage+65};memcpy(&data,bounds,12);
  CPersistContext_TransferableVectorGenericType&dispatch=subject;dispatch.Resize(size);
  bool*after[3];memcpy(after,&data,12);
  if(after[0]!=storage+1||after[1]!=storage+1+size||after[2]!=storage+65)abort();
  for(int j=0;j<size;++j)if(storage[j+1]!=(j<old?before[j+1]:false))abort();
  for(int j=(size>old?size:old)+1;j<67;++j)if(storage[j]!=before[j])abort();
  if(!storage[0]||!storage[65]||!storage[66])abort();++cases;
 }
 if(cases!=12675)abort();puts("00cdd0a0_TEST PASS");return 0;
}