#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int allocs=0,clears=0,fail_alloc=0;
// 00462e78 std::vector<bool,std::allocator<bool> >::operator=
// Leaf: ??4?$vector@_NV?$allocator@_N@std@@@std@@QAEAAV01@ABV01@@Z
// Flags: /Oy-
#pragma optimize("s",on)
#include <stddef.h>
extern "C" void*__cdecl memmove(void*,const void*,size_t);
namespace std {
struct __true_type{};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base {protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T>struct __type_traits;
template<>struct __type_traits<bool>{typedef __true_type has_trivial_assignment_operator,is_POD_type;};
FABLE_FORCEINLINE void*__copy_trivial(const void*first,const void*last,void*result){return last==first?result:static_cast<char*>(memmove(result,first,static_cast<const char*>(last)-static_cast<const char*>(first)))+(static_cast<const char*>(last)-static_cast<const char*>(first));}
template<class I,class O>FABLE_FORCEINLINE O __copy_ptrs(I first,I last,O result,const __true_type&){return static_cast<O>(__copy_trivial(first,last,result));}
template<class I,class O>O __fastcall __uninitialized_copy(I,I,O,const __true_type&);
template<class T>struct _IsPOD{typedef typename __type_traits<T>::is_POD_type _Type;static FABLE_FORCEINLINE _Type _Ret(){return _Type();}};
template<class T>FABLE_FORCEINLINE _IsPOD<T>_Is_POD(T*){return _IsPOD<T>();}
template<class I,class O>FABLE_FORCEINLINE O uninitialized_copy(I first,I last,O result){return __uninitialized_copy(first,last,result,_Is_POD((bool*)0)._Ret());}
template<class I>FABLE_FORCEINLINE void _Destroy(I,I){}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*pointer;typedef const T*const_pointer;typedef size_t size_type;typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss,_IsPODType;size_type size()const{return size_type(this->_M_finish-this->_M_start);}size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}pointer end(){return this->_M_finish;}const_pointer end()const{return this->_M_finish;}vector&operator=(const vector&);
protected:template<class I>pointer _M_allocate_and_copy(size_type,I,I);void _M_clear();};
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector<T,A>&x){
 if(&x!=this){const size_type xlen=x.size();if(xlen>capacity()){
  pointer tmp=_M_allocate_and_copy(xlen,(const_pointer)x._M_start+0,(const_pointer)x._M_finish+0);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;
 }else if(size()>=xlen){pointer i=__copy_ptrs((const_pointer)x._M_start+0,(const_pointer)x._M_finish+0,(pointer)this->_M_start,_TrivialAss());_Destroy(i,this->_M_finish);
 }else{
  __copy_ptrs((const_pointer)x._M_start,(const_pointer)x._M_start+size(),(pointer)this->_M_start,_TrivialAss());
  uninitialized_copy((const_pointer)x._M_finish-(x.size()-size()),x.end(),end());
 }this->_M_finish=this->_M_start+xlen;}return *this;
}
template<class I,class O>O __fastcall __uninitialized_copy(I first,I last,O out,const __true_type&){while(first!=last)*out++=*first++;return out;}
template<class T,class A>template<class I>typename vector<T,A>::pointer vector<T,A>::_M_allocate_and_copy(size_type count,I first,I last){if(fail_alloc)throw 7;pointer p=static_cast<pointer>(malloc(count*sizeof(T)));if(!p)throw 8;++allocs;pointer q=p;while(first!=last)*q++=*first++;return p;}
template<class T,class A>void vector<T,A>::_M_clear(){++clears;free(this->_M_start);this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;}
template class vector<bool>;
}

class TestVector:public std::vector<bool>{public:
 void init(unsigned int n,unsigned int cap,unsigned int seed){this->_M_start=static_cast<bool*>(malloc((cap?cap:1)*sizeof(bool)));this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_start+cap;for(unsigned int i=0;i<cap;++i)this->_M_start[i]=((i+seed)%3)==0;}
 bool at(unsigned int i)const{return this->_M_start[i];}
 bool*data()const{return this->_M_start;}
 void dispose(){free(this->_M_start);}
};
#define CHECK(x) do{if(!(x)){printf("failure line %d\n",__LINE__);return 1;}}while(0)
int main(){
 for(unsigned int cap=0;cap<=16;++cap)for(unsigned int oldn=0;oldn<=cap;++oldn)for(unsigned int newn=0;newn<=20;++newn){
 TestVector dst,src;dst.init(oldn,cap,1);src.init(newn,newn+2,2);bool*old=dst.data();int na=allocs,nc=clears;
 std::vector<bool>&ret=static_cast<std::vector<bool>&>(dst)=static_cast<const std::vector<bool>&>(src);
 CHECK(&ret==static_cast<std::vector<bool>*>(&dst));CHECK(dst.size()==newn);CHECK(src.size()==newn);CHECK(src.capacity()==newn+2);
 for(unsigned int i=0;i<newn;++i){CHECK(dst.at(i)==src.at(i));CHECK(src.at(i)==((i+2)%3==0));}
 if(newn>cap){CHECK(allocs==na+1);CHECK(clears==nc+1);CHECK(dst.capacity()==newn);}else{CHECK(allocs==na);CHECK(clears==nc);CHECK(dst.data()==old);CHECK(dst.capacity()==cap);}
 bool*now=dst.data();na=allocs;nc=clears;static_cast<std::vector<bool>&>(dst)=static_cast<const std::vector<bool>&>(dst);CHECK(dst.data()==now);CHECK(dst.size()==newn);CHECK(allocs==na&&clears==nc);for(unsigned int i=0;i<newn;++i)CHECK(dst.at(i)==src.at(i));
 dst.dispose();src.dispose();
 }
 TestVector dst,src;dst.init(3,4,1);src.init(9,9,2);bool*saved=dst.data();int nc=clears;fail_alloc=1;bool caught=false;try{static_cast<std::vector<bool>&>(dst)=static_cast<const std::vector<bool>&>(src);}catch(int value){caught=value==7;}CHECK(caught);CHECK(dst.data()==saved);CHECK(dst.size()==3);CHECK(dst.capacity()==4);CHECK(clears==nc);for(unsigned int i=0;i<3;++i)CHECK(dst.at(i)==((i+1)%3==0));dst.dispose();src.dispose();
 puts("00462e78_TEST PASS");return 0;
}