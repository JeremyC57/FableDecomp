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
__forceinline void*__copy_trivial(const void*first,const void*last,void*result){return last==first?result:static_cast<char*>(memmove(result,first,static_cast<const char*>(last)-static_cast<const char*>(first)))+(static_cast<const char*>(last)-static_cast<const char*>(first));}
template<class I,class O>__forceinline O __copy_ptrs(I first,I last,O result,const __true_type&){return static_cast<O>(__copy_trivial(first,last,result));}
template<class I,class O>O __fastcall __uninitialized_copy(I,I,O,const __true_type&);
template<class T>struct _IsPOD{typedef typename __type_traits<T>::is_POD_type _Type;static __forceinline _Type _Ret(){return _Type();}};
template<class T>__forceinline _IsPOD<T>_Is_POD(T*){return _IsPOD<T>();}
template<class I,class O>__forceinline O uninitialized_copy(I first,I last,O result){return __uninitialized_copy(first,last,result,_Is_POD((bool*)0)._Ret());}
template<class I>__forceinline void _Destroy(I,I){}
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
template class vector<bool>;
}