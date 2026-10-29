#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
// std::vector<float>::_M_fill_insert -- insert copies in spare storage or grow.
// Leaf: ?_M_fill_insert@?$vector@MV?$allocator@M@std@@@std@@QAEXPAMIABM@Z
// Flags: /Gr
// Grade: pending independent owner-name audit/native pass9 grade2.
// Actual STLport4.6.2 source, with the retail malloc allocator route and out-of-line real helpers.
// Copyright (c)1994 Hewlett-Packard Company;1996,1997 Silicon Graphics Computer Systems,Inc.;
// 1997 Moscow Center for SPARC Technology;1999 Boris Fomitchev.
// Permission to use/copy/modify/distribute this software for any purpose is granted without fee,
// provided notices are retained. Supplied as is without warranty. Adapted to the observed retail configuration.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
extern "C" void* __cdecl memmove(void*,const void*,unsigned int);
namespace std {
struct __true_type{};struct __false_type{};
template<int I>struct __bool2type{typedef __false_type _Ret;};
template<>struct __bool2type<1>{typedef __true_type _Ret;};
template<class A,class B,class C>struct _Land3{typedef __false_type _Ret;};
template<>struct _Land3<__true_type,__true_type,__true_type>{typedef __true_type _Ret;};
template<class A,class B>struct _IsSame{enum{_Ret=0};};
template<class A>struct _IsSame<A,A>{enum{_Ret=1};};
template<int I>struct __type_traits_aux{
 typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;typedef __false_type is_POD_type;
};
template<>struct __type_traits_aux<1>{
 typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;typedef __true_type is_POD_type;
};
template<class T>struct __type_traits:__type_traits_aux<0>{};
template<>struct __type_traits<float>:__type_traits_aux<1>{};
template<class A,class B>struct _BothPtrType{static __false_type _Ret(){return __false_type();}};
template<class A,class B>struct _BothPtrType<A*,B*>{static __true_type _Ret(){return __true_type();}};
template<class A,class B>struct _OKToMemCpy{
 enum{_Same=_IsSame<A,B>::_Ret};
 typedef typename __type_traits<A>::has_trivial_assignment_operator _Tr1;
 typedef typename __type_traits<B>::has_trivial_assignment_operator _Tr2;
 typedef typename __bool2type<_Same>::_Ret _Tr3;
 typedef typename _Land3<_Tr1,_Tr2,_Tr3>::_Ret _Type;
 static _Type _Ret(){return _Type();}
};
template<class A,class B>inline _OKToMemCpy<A,B> _IsOKToMemCpy(A*,B*){return _OKToMemCpy<A,B>();}
template<class I>struct iterator_traits;
template<class T>struct iterator_traits<T*>{typedef T value_type;};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
inline void*__copy_trivial(const void*first,const void*last,void*result){
 return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
inline void*__copy_trivial_backward(const void*first,const void*last,void*result){
 const int n=(const char*)last-(const char*)first;return n>0?memmove((char*)result-n,first,n):result;
}
template<class I,class O>inline O __copy_ptrs(I first,I last,O result,const __true_type&){return(O)__copy_trivial(first,last,result);}
template<class I,class O>inline O __copy_aux(I first,I last,O result,const __true_type&){
 return __copy_ptrs(first,last,result,_IsOKToMemCpy((typename iterator_traits<I>::value_type*)0,(typename iterator_traits<O>::value_type*)0)._Ret());
}
template<class I,class O>inline O __uninitialized_copy(I first,I last,O result,const __true_type&){return __copy_aux(first,last,result,_BothPtrType<I,O>::_Ret());}
template<class I,class O>inline O __copy_backward_ptrs(I first,I last,O result,const __true_type&){return(O)__copy_trivial_backward(first,last,result);}
template<class O,class N,class T>inline O fill_n(O first,N n,const T&value){for(;n>0;--n,++first)*first=value;return first;}
template<class O,class N,class T>inline O __uninitialized_fill_n(O first,N n,const T&value,const __true_type&){return fill_n(first,n,value);}
template<class O,class N,class T>inline O uninitialized_fill_n(O first,N n,const T&value){return __uninitialized_fill_n(first,n,value,typename __type_traits<typename iterator_traits<O>::value_type>::is_POD_type());}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class I,class T>inline void __destroy(I first,I last,T*){__destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor());}
template<class I>inline void _Destroy(I first,I last){__destroy(first,last,(typename iterator_traits<I>::value_type*)0);}
template<class I,class T>__declspec(noinline)void __fastcall fill(I first,I last,const T&value){for(;first!=last;++first)*first=value;}
template<class T>class allocator{public:
 allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}
 T*allocate(unsigned int n,const void* =0);
 void deallocate(T*p,unsigned int){if(p)free(p);}
};
template<class T>T*allocator<T>::allocate(unsigned int n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:
 V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}
};
template<class T,class A>class _Vector_base{public:
 typedef A allocator_type;
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:
 T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
 typedef _Vector_base<T,A> _Base;
public:
 typedef T*pointer;typedef T*iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const allocator_type&a=allocator_type()):_Base(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 size_type size()const{return this->_M_finish-this->_M_start;}
 iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}
 void _M_fill_insert(iterator,size_type,const T&);
protected:
 typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 __declspec(noinline)void _M_insert_overflow(pointer,const T&,const __true_type&,size_type,bool=false);
};
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer position,const T&x,const __true_type&,size_type n,bool atEnd){
 const size_type old=size(),len=old+(max)(old,n);
 pointer start=this->_M_end_of_storage.allocate(len);
 pointer finish=(pointer)__copy_trivial(this->_M_start,position,start);
 finish=fill_n(finish,n,x);
 if(!atEnd)finish=(pointer)__copy_trivial(position,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
}
template<class T,class A>void vector<T,A>::_M_fill_insert(iterator position,size_type n,const T&x){
 if(n!=0){
  if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n){
   T copy=x;const size_type after=this->_M_finish-position;pointer finish=this->_M_finish;
   if(after>n){
    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
    this->_M_finish+=n;__copy_backward_ptrs(position,finish-n,finish,_TrivialAss());fill(position,position+n,copy);
   }else{
    uninitialized_fill_n(this->_M_finish,n-after,copy);this->_M_finish+=n-after;
    __uninitialized_copy(position,finish,this->_M_finish,_IsPODType());this->_M_finish+=after;fill(position,finish,copy);
   }
  }else _M_insert_overflow(position,x,_IsPODType(),n);
 }
}
template void vector<float>::_M_fill_insert(float*,unsigned int,const float&);
}

// Test-only storage setup; the actual candidate insert and all its original helpers are above.
class FixtureVector:public std::vector<float>{public:
 void initialise(unsigned n,unsigned capacity){this->_M_start=this->_M_end_of_storage.allocate(capacity);this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_start+capacity;for(unsigned i=0;i<capacity;++i)this->_M_start[i]=-99.0f;for(unsigned i=0;i<n;++i)this->_M_start[i]=(float)(i+1);}
 float*data(){return this->_M_start;}unsigned capacity()const{return this->_M_end_of_storage._M_data-this->_M_start;}
};
static bool equals(FixtureVector&v,const float*expected,unsigned n){if(v.size()!=n)return false;for(unsigned i=0;i<n;++i)if(v.data()[i]!=expected[i])return false;return true;}
typedef char VectorLayout[sizeof(std::vector<float>)==12?1:-1];
int main(){
 FixtureVector first;first.initialise(4,8);float nine=9;float*original=first.data();
 first._M_fill_insert(first.begin()+1,0,nine);float untouched[]={1,2,3,4};if(!equals(first,untouched,4)||first.data()!=original)return 1;
 first._M_fill_insert(first.begin()+1,1,nine);float expected1[]={1,9,2,3,4};if(!equals(first,expected1,5)||first.data()!=original||first.data()[7]!=-99)return 2;
 FixtureVector second;second.initialise(4,8);float seven=7;second._M_fill_insert(second.begin()+3,3,seven);float expected2[]={1,2,3,7,7,7,4};if(!equals(second,expected2,7)||second.data()[7]!=-99)return 3;
 FixtureVector grow;grow.initialise(3,3);const float&aliased=grow.data()[2];grow._M_fill_insert(grow.begin()+1,2,aliased);float expected3[]={1,3,3,2,3};if(!equals(grow,expected3,5)||grow.capacity()!=6)return 4;
 FixtureVector empty;float five=5;empty._M_fill_insert(empty.begin(),2,five);float expected4[]={5,5};if(!equals(empty,expected4,2)||empty.capacity()!=2)return 5;
 std::puts("00441020_TEST PASS");return 0;}