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