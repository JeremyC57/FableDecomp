#ifndef FABLE_STLPORT_PARTICLE_SPRINGS_H
#define FABLE_STLPORT_PARTICLE_SPRINGS_H
// STLport4.6.2 excerpts, modified to omit operations unused by this TU.
// Copyright1994 Hewlett-Packard,1996-1997 Silicon Graphics,1997 Moscow Center
// for SPARC Technology,1999 Boris Fomitchev. Permission to use, copy, modify,
// distribute without fee; provided as is, without warranty; retain notices.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned long>{typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<long>{typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity{const T&operator()(const T&x)const{return x;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class U>struct pair{T first;U second;pair(){}pair(const T&a,const U&b):first(a),second(b){} };
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline void swap(T&a,T&b){T x=a;a=b;b=x;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>__declspec(noinline) void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
template<class T>class allocator{public:allocator(){}allocator(const allocator&){}~allocator(){}T*allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class Value,class T,class A>struct _STLP_alloc_proxy:public A{Value _M_data;_STLP_alloc_proxy(const A&a,Value p):A(a),_M_data(p){} };
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>struct _Rb_global{static _Rb_tree_node_base*__fastcall _M_increment(_Rb_tree_node_base*x){if(x->_M_right!=0){x=x->_M_right;while(x->_M_left!=0)x=x->_M_left;}else{_Rb_tree_node_base*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef typename Traits::reference reference;_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&x){_M_node=x._M_node;}reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};
template<class T,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;pair<iterator,bool>insert_unique(const V&);const_iterator begin()const{return (_Rb_tree_node<V>*)this->_M_header._M_data->_M_left;}const_iterator end()const{return this->_M_header._M_data;}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::const_iterator const_iterator;typedef const_iterator iterator;set();set(const set&);~set();pair<iterator,bool>insert(const K&x){pair<typename _Rep_type::iterator,bool>p=_M_t.insert_unique(x);return pair<iterator,bool>((const iterator&)p.first,p.second);}iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}};
template<>struct __type_traits<set<unsigned long> >{typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}_Vector_base(unsigned int n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}};
template<class T>inline T*__uninitialized_copy(const T*first,const T*last,T*out,const __false_type&){T*cur=out;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}
inline void*__copy_trivial(const void*first,const void*last,void*out){return last==first?out:(char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T>inline T*__uninitialized_copy(const T*first,const T*last,T*out,const __true_type&){return (T*)__copy_trivial(first,last,out);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(cur,x);return cur;}
template<class T>inline T*fill_n(T*p,unsigned int n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{typedef _Vector_base<T,A>Base;public:typedef T*iterator;typedef const T*const_iterator;typedef typename __type_traits<T>::has_trivial_assignment_operator IsPOD;explicit vector(const A&a=A()):Base(a){}explicit vector(unsigned int n):Base(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),IsPOD());}vector(unsigned int n,const T&x,const A&a=A()):Base(n,a){this->_M_finish=fill_n(this->_M_start,n,x);}vector(const vector&x):Base(x.size(),x.get_allocator()){this->_M_finish=__uninitialized_copy(x._M_start,x._M_finish,this->_M_start,IsPOD());}A get_allocator()const{return (const A&)this->_M_end_of_storage;}~vector(){_Destroy(this->_M_start,this->_M_finish);}unsigned int size()const{return this->_M_finish-this->_M_start;}bool empty()const{return this->_M_start==this->_M_finish;}iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}void swap(vector&x){std::swap(this->_M_start,x._M_start);std::swap(this->_M_finish,x._M_finish);std::swap(this->_M_end_of_storage,x._M_end_of_storage);}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,IsPOD(),1,true);}protected:
void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
void _M_set(T*a,T*b,T*c){this->_M_start=a;this->_M_finish=b;this->_M_end_of_storage._M_data=c;}
void _M_insert_overflow(T*pos,const T&x,const __true_type&,unsigned int n,bool atEnd=false){unsigned int old=size(),len=old+(max)(old,n);T*start=this->_M_end_of_storage.allocate(len);T*finish=(T*)__copy_trivial(this->_M_start,pos,start);finish=fill_n(finish,n,x);if(!atEnd)finish=(T*)__copy_trivial(pos,this->_M_finish,finish);_M_clear();_M_set(start,finish,start+len);}
void _M_insert_overflow(T*pos,const T&x,const __false_type&,unsigned int n,bool atEnd=false){unsigned int old=size(),len=old+(max)(old,n);T*start=this->_M_end_of_storage.allocate(len);T*finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());if(n==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,n,x,__false_type());if(!atEnd)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());_M_clear();_M_set(start,finish,start+len);}
};
template<class I>void __fastcall sort(I,I);
}
#endif
