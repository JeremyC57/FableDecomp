#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#define FABLE_STATIC_ASSERT(x)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CONSTRUCTORS
#define FABLE_RETAIL_CWIDESTRING_CTOR
struct UIDtorFixture;
static void* trackedAlloc(size_t);
static void trackedFree(void*);
#define malloc trackedAlloc
#define free trackedFree


#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_UI_DEF_DTOR
#define FABLE_VECTOR_MAP_GEOMETRY
#ifndef FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUIMISCTHINGSDEF_H


#ifndef FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {friend struct ::UIDtorFixture;
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
#endif
};
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
#endif
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{friend struct ::UIDtorFixture;
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{friend struct ::UIDtorFixture;
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}
#endif

#ifdef FABLE_RETAIL_UI_DEF_DTOR
#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H


class CCharString;
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
private:
 long TablePos;
};
#endif

#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY

#else
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#ifdef FABLE_STLPORT_UI_ACTIONS
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
struct __true_type { };
#ifdef FABLE_STLPORT_HTTP
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; };
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) { free(p); }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {friend struct ::UIDtorFixture;
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#ifdef FABLE_STLPORT_UI
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_STLPORT_UI
    vector(const vector&);
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_SCOREBOARD
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    __declspec(noinline) void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
protected:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
    void push_back(const _Tp&);
#endif
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_STLPORT_UI_ACTIONS
protected:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif

#endif


template<class T> class CArray : public std::vector<T,std::allocator<T> > { };
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
#endif

#define FABLE_PDCB_TYPED_MEMBERS
#endif
#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H

























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H




#include <stddef.h>

#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H







#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H















#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
    CIVCountedPointeeBase();
#endif
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    long Release() const { return --IVRefCount; }                        
    long GetRefCount() const { return IVRefCount; }                      
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
private:
#endif
    mutable unsigned long IVRefCount;                                    
};
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)


#endif 

#endif 

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         
    virtual ~CResource();                                                
    virtual bool UnloadResource();                                       
    virtual unsigned long GetUnloadableMemory() const;                   
    virtual bool IsResourceLoaded() const;                               
    virtual void OnResourceLoaded();                                     
    virtual bool CanAbortResourceLoad(long) const;                       
    virtual void OnResourceLoadAborted();                                
    virtual unsigned long GetDebugMemoryInUse() const;                   
    virtual void ResetDebugMemoryInUse();                                
    CResourceList* ResourceList;                                         
    CResource*     PrevResource;                                         
    CResource*     NextResource;                                         
    unsigned long  ResourceSize;                                         
    unsigned long  LastUsedFrame;                                        
};
#else 
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                
    unsigned char  _pad_0x04[0x4];                           
    CResourceList* ResourceList;                             
    CResource*     PrevResource;                             
    CResource*     NextResource;                             
    unsigned long  ResourceSize;                             
    unsigned long  LastUsedFrame;                            
};
#pragma pack(pop)


#endif 

#endif 


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};

#endif 

class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
#ifdef FABLE_RETAIL_DEF_CONSTRUCTORS
private:
    CDefClassBase(); 
    friend class CParentDefClassBase;
#endif
public:
    class CSubDefInfo {                                                  
    public:
        long DefIndex;                                                   
        long OriginalParentDefIndex;                                     
    };
    CDefClassBase(const CDefClassBase&);                                 
    CDefClassBase& operator=(const CDefClassBase&);                      
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   
    virtual ~CDefClassBase() { }                                         
    virtual bool UnloadResource();                                       
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void PostTransfer();                                         
    virtual void Transfer(CPersistContext&) = 0;                         
    virtual void Copy(const CDefClassBase*) = 0;                         
    virtual unsigned long GetSizeofClass() = 0;                          
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
    CDefinitionManager* PDefManager;                                     
    unsigned long       GlobalIndex;                                     
    unsigned char       Setup : 1;                                       
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
#else 
#include <stddef.h>


class CDefinitionManager;

#pragma pack(push, 1)
class CDefClassBase {
public:
    void*               __vftable;                           
    unsigned char       _base_0x04[0x18];                    
    CDefinitionManager* PDefManager;                         
    unsigned long       GlobalIndex;                         
    unsigned char       DefaultValsApplied[0x1];             
};
#pragma pack(pop)


#endif 

#endif 

class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               
    CParentDefClassBase(const CParentDefClassBase&);                     
    CParentDefClassBase& operator=(const CParentDefClassBase&);          
#ifdef FABLE_RETAIL_UI_DEF_DTOR
    
#else
    virtual ~CParentDefClassBase();                                      
#endif
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
#ifdef FABLE_PDCB_TYPED_MEMBERS
    CDefString InstantiationName;                                        
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   
#else
    long          InstantiationName_TablePos;                            
    unsigned char SubDefInfoMap[0x10];                                   
#endif
};
#pragma pack(pop)
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CParentDefClassBase {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x21];                          
    long          InstantiationName_TablePos;                
    unsigned char SubDefInfoMap[0x10];                       
};
#pragma pack(pop)


#endif 

#endif 

#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING

class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#ifdef FABLE_RETAIL_SCOREBOARD
    CCharString ToCharString() const;
#endif
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#ifdef FABLE_RETAIL_SCOREBOARD
#define FABLE_RETAIL_CCHARSTRING_DATA
#pragma once

#include <stddef.h>



#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
public:
    const T* PeekData() const { return PData; }
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(long length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();

protected:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        long length);

private:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        long length);

private:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];


#endif

#endif
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_SCOREBOARD
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#ifdef FABLE_RETAIL_EFFECT_LOOKUP
    unsigned long GetCRC() const; 
#endif
    ~CCharString();
protected:
#ifdef FABLE_RETAIL_SCOREBOARD
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_C2DVECTOR_H
#define FABLE_ENGINE_C2DVECTOR_H


#ifdef FABLE_RETAIL_C2DVECTOR_UI


class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    C2DVector(float x,float y) : X(x),Y(y) { }
    C2DVector(const C2DVector& p) : X(p.X),Y(p.Y) { }
    C2DVector operator+(const C2DVector& vector) const { return C2DVector(X+vector.X,Y+vector.Y); }
    C2DVector operator-(const C2DVector& vector) const { return C2DVector(X-vector.X,Y-vector.Y); }
};
#else
class C2DVector { public: float X,Y; };
#endif
#endif

#ifndef FABLE_ENGINE_EMUSICSETTYPE_H
#define FABLE_ENGINE_EMUSICSETTYPE_H

enum EMusicSetType {
    MUSIC_SET_NULL=1,
    MUSIC_SET_FRESCO_WEDDING=2,
    MUSIC_SET_CHAPEL_OF_EVIL=3,
    MUSIC_SET_GUILD=4,
    MUSIC_SET_GUILD_DAY=5,
    MUSIC_SET_GUILD_NIGHT=6,
    MUSIC_SET_HALL_OF_HEROES=7,
    MUSIC_SET_TEMPLE_OF_LIGHT=8,
    MUSIC_SET_ARENA_FANFARE=9,
    MUSIC_SET_BANDIT_CAMP=10,
    MUSIC_SET_BOWERSTONE=11,
    MUSIC_SET_CAVES=12,
    MUSIC_SET_DARKWOOD=13,
    MUSIC_SET_GRAVEYARD=14,
    MUSIC_SET_GRAVEYARD_PASSAGE=15,
    MUSIC_SET_GREATWOOD=16,
    MUSIC_SET_HAUNTED_HOUSE=17,
    MUSIC_SET_LOOKOUT_POINT=18,
    MUSIC_SET_OAKVALE=19,
    MUSIC_SET_WITCHWOOD=20,
    MUSIC_SET_QUEST_SUCCEEDED=21,
    MUSIC_SET_QUEST_FAILED=22,
    MUSIC_SET_BOSS=23,
    MUSIC_SET_DANGER=24,
    MUSIC_SET_PRISON=25,
    MUSIC_SET_HOOK_COAST=26,
    MUSIC_SET_KNOTHOLE_GLADE=27,
    MUSIC_SET_EXECUTION_TREE=28,
    MUSIC_SET_GIBBET_WOODS=29,
    MUSIC_SET_KRAKEN_CHAMBER=30,
    MUSIC_SET_INTERLUDE=31,
    MUSIC_SET_DANGER_ONLY=32,
    MUSIC_SET_DRAGON=33,
    MUSIC_SET_ARENA_FIGHT=34,
    MUSIC_SET_ARENA_FANFARE_01=35,
    MUSIC_SET_ARENA_FANFARE_02=36,
    MUSIC_SET_ARENA_FANFARE_03=37,
    MUSIC_SET_ARENA_FANFARE_04=38,
    MUSIC_SET_ARENA_FANFARE_05=39,
    MUSIC_SET_ARENA_FANFARE_06=40,
    MUSIC_SET_ARENA_FANFARE_07=41,
    MUSIC_SET_ARENA_FANFARE_08=42,
    MUSIC_SET_ARENA_FANFARE_09=43,
    MUSIC_SET_ARENA_FANFARE_10=44,
    MUSIC_SET_CUTSCENE_DEAD_DAD=45,
    MUSIC_SET_CUTSCENE_FEET=46,
    MUSIC_SET_CUTSCENE_GUILD_CEREMONY=47,
    MUSIC_SET_CUTSCENE_TWINBLADE=48,
    MUSIC_SET_CUTSCENE_THERESA_01=49,
    MUSIC_SET_CUTSCENE_THERESA_02=50,
    MUSIC_SET_CUTSCENE_WIZARD_BATTLE_INTRO=51,
    MUSIC_SET_CUTSCENE_WIZARD_BATTLE_OUTRO=52,
    MUSIC_SET_CUTSCENE_PRISON_MOTHER=53,
    MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_INTRO=54,
    MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_OUTRO=55,
    MUSIC_SET_CUTSCENE_JACK_CAPTURES=56,
    MUSIC_SET_CUTSCENE_GUILD_ARRIVAL=57,
    MUSIC_SET_CUTSCENE_COLLECT_FIREHEART=58,
    MUSIC_SET_CUTSCENE_DRAGON_DEATH=59,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_INTRO=60,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_EVIL=61,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_GOOD=62,
    MUSIC_SET_CUTSCENE_DRAGON_FIGHT_OUTRO_CHOICE=63,
    MUSIC_SET_CUTSCENE_ONE_YEAR_LATER=64,
    MUSIC_SET_CUTSCENE_ORACLE_AWAKENS=65,
    MUSIC_SET_CUTSCENE_SCYTHE_MESSAGE=66,
    MUSIC_SET_CUTSCENE_SOUL2_MOTHER_INTRO=67,
    MUSIC_SET_CUTSCENE_SOUL2_MOTHER_SUCCESS=68,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_INTRO=69,
    MUSIC_SET_CUTSCENE_SUMMON_SHIP_INTRO=70,
    MUSIC_SET_CUTSCENE_SUMMON_SHIP_OUTRO=71,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_OUTRO_EVIL=72,
    MUSIC_SET_CUTSCENE_SOUL3_GUILDMASTER_OUTRO_GOOD=73,
    MUSIC_SET_CUTSCENE_GATE_OPENS_BRIAR_ROSE=74,
    MUSIC_SET_CUTSCENE_GATE_OPENS_SCYTHE=75,
    MUSIC_SET_INTRO=76,
    NO_MUSIC_SETS=77
};
#endif

namespace NUISystem {


class CUIMiscThingsDef : public CParentDefClassBase {
public:
#ifndef FABLE_RETAIL_UI_DEF_DTOR
    CUIMiscThingsDef();
    CUIMiscThingsDef(const CUIMiscThingsDef&);
    virtual ~CUIMiscThingsDef();
#endif
    static CDefClassBase* __fastcall Alloc();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CWideString SpaceSeparator; 
    CWideString CommaSeparator; 
    CWideString ColonSeparator; 
    CWideString NewLineSeparator; 
    CWideString OpenBracket; 
    CWideString CloseBracket; 
    CWideString Positive; 
    CWideString WeaponValueString; 
    CWideString WeaponAugString; 
    CWideString WeaponAugNone; 
    CWideString WeaponWeightString; 
    CWideString WeaponLightString; 
    CWideString WeaponHeavyString; 
    CWideString WeaponKillsString; 
    CWideString WeaponCatMeleeString; 
    CWideString WeaponCatRangedString; 
    CWideString WeaponDamageString; 
    CWideString TradeCostString; 
    CWideString TradeProfitString; 
    CWideString TradeLossString; 
    CWideString TradeAlreadyOwnsString; 
    CWideString TradeNumberInStockString; 
    CWideString TradeDeliveryString; 
    CWideString TradeNoDeliveryString; 
    CWideString TradeDaysString; 
    CWideString TradeBuyString; 
    CWideString TradeSellString; 
    CWideString TradeWantedString; 
    CWideString QuestFailedString; 
    CWideString FailedString; 
    CWideString SucceededString; 
    CWideString Plus; 
    CWideString Minus; 
    unsigned long CoreGraphic; 
    unsigned long VignetteGraphic; 
    unsigned long OptionalGraphic; 
    unsigned long FeatGraphic; 
    CWideString ObjectsRewardString; 
    CWideString NoneString; 
    CWideString CheckGuildString; 
    CWideString QuestStartingString; 
    C2DVector RingCenter; 
    C2DVector PCRingCenter; 
    C2DVector WorldMapOffset; 
    float WorldMapWidth; 
    float WorldMapHeight; 
    CWideString YouString; 
    CWideString OwnString; 
    CWideString NoString; 
    CWideString HousesString; 
    CWideString HouseString; 
    CWideString InString; 
    CWideString ShopsString; 
    CWideString ShopString; 
    CWideString ThereString; 
    CWideString AreString; 
    CWideString IsString; 
    CWideString ForString; 
    CWideString SaleString; 
    CWideString GeneralString; 
    CWideString TitleString; 
    CWideString BarberString; 
    CWideString TatooString; 
    CWideString LevelString; 
    unsigned long TotalSpellsInPalettes; 
    unsigned long TotalSpellsInContainer; 
    CWideString LogBookBasicsCategoryString; 
    CWideString LogBookObjectsCategoryString; 
    CWideString LogBookTownsCategoryString; 
    CWideString LogBookHeroCategoryString; 
    CWideString LogBookCombatCategoryString; 
    CWideString LogBookQuestCategoryString; 
    CWideString LogBookStoryCategoryString; 
    CWideString LogBookBasicsCategoryNameString; 
    CWideString LogBookObjectsCategoryNameString; 
    CWideString LogBookTownsCategoryNameString; 
    CWideString LogBookHeroCategoryNameString; 
    CWideString LogBookCombatCategoryNameString; 
    CWideString LogBookQuestCategoryNameString; 
    CWideString LogBookStoryCategoryNameString; 
    std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > > MapPaths; 
    CCharString SoundUpDown; 
    CCharString SoundSlider; 
    CCharString SoundBack; 
    CCharString SoundForward; 
    CCharString SoundError; 
    CCharString SoundExit; 
    CCharString CountUpSound; 
    CCharString SoundKeyboardUp; 
    CCharString SoundKeyboardDown; 
    CCharString SoundKeyboardLeft; 
    CCharString SoundKeyboardRight; 
    CCharString SoundKeyboardEnterCharacter; 
    CCharString SoundKeyboardDeleteCharacter; 
    CCharString SoundKeyboardDone; 
    CWideString FrontEndMusic; 
    C2DVector HeroDollTL; 
    C2DVector HeroDollBR; 
    float HeroDollSphereRadius; 
    C2DVector HeroDollTL_PC; 
    C2DVector HeroDollBR_PC; 
    float HeroDollSphereRadius_PC; 
    C2DVector HeroDollFrameTL_PC; 
    float HeroDollFrameEmulateListOffset; 
    EMusicSetType QuestStartScreenMusic; 
    EMusicSetType QuestCompleteScreenMusic; 
    EMusicSetType QuestFailureScreenMusic; 
    EMusicSetType DeathScreenMusic; 
    float DigitCountTime; 
    unsigned long SaveHeroGraphicIndex; 
    std::map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString const ,long> > > MiniMapGraphics; 
    long KeyboardSmallKeyGraphic; 
    long KeyboardLargeKeyGraphic; 
    unsigned long TotalAssignableThings; 
    float TimeInSecsForFade; 
    float BackBufferFilterSaturation; 
    float BackBufferFilterContrast; 
    float BackBufferFilterBrightness; 
    float BackBufferFilterTintR; 
    float BackBufferFilterTintG; 
    float BackBufferFilterTintB; 
    float BackBufferFilterTintScale; 
    float BackBufferDiffuseScale; 
    float BackBufferAmbientScale; 
    float MinimumFilterColor; 
};
}
#endif



CDefClassBase* __fastcall NUISystem::CUIMiscThingsDef::Alloc()
{
    return new CUIMiscThingsDef;
}

#undef malloc
#undef free

static unsigned int allocationCount=0,freeCount=0,resourceDestroyed=0,visits=0,expectedCount=0;
static void* activePointers[256];
static void* expectedStrings[128];
static int expectedKinds[128];
static bool checking=false;
static void fail(int n){fprintf(stderr,"misc destructor check %d at %u\n",n,visits);exit(n);}
static void* trackedAlloc(size_t n){void* p=malloc(n?n:1);if(!p||allocationCount>=256)fail(1);activePointers[allocationCount++]=p;return p;}
static void trackedFree(void* p){if(!p)return;unsigned int i;for(i=0;i<allocationCount;++i)if(activePointers[i]==p){activePointers[i]=0;++freeCount;free(p);return;}fail(2);}
static void noteString(void* p,int kind){if(checking){if(visits>=expectedCount||expectedStrings[visits]!=p||expectedKinds[visits]!=kind)fail(3);++visits;}}
void CIVCountedPointeeBase::Delete() const { throw 1; }
CResource::CResource() : ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0) { IVRefCount=0; }
CResource::~CResource() {++resourceDestroyed;}
bool CResource::UnloadResource(){throw 1;}
unsigned long CResource::GetUnloadableMemory()const{throw 1;}
bool CResource::IsResourceLoaded()const{throw 1;}
void CResource::OnResourceLoaded(){throw 1;}
bool CResource::CanAbortResourceLoad(long)const{throw 1;}
void CResource::OnResourceLoadAborted(){throw 1;}
unsigned long CResource::GetDebugMemoryInUse()const{throw 1;}
void CResource::ResetDebugMemoryInUse(){throw 1;}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(0),Template(0),DefaultValsApplied(0){}
bool CDefClassBase::UnloadResource(){throw 1;}
CDefString CDefClassBase::GetInstantiationName()const{throw 1;}
void CDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CDefClassBase::IsParentDef()const{throw 1;}
void CDefClassBase::PostTransfer(){throw 1;}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&,const CCharString&)const{throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){throw 1;}
CParentDefClassBase::CParentDefClassBase(){}

CDefString CParentDefClassBase::GetInstantiationName()const{throw 1;}
void CParentDefClassBase::SetInstantiationName(CDefString){throw 1;}
void CParentDefClassBase::AddSubDef(const CCharString&,long,long){throw 1;}
void CParentDefClassBase::RemoveSubDef(const CCharString&){throw 1;}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){throw 1;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&)const{throw 1;}
bool CParentDefClassBase::IsParentDef()const{throw 1;}
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&){throw 1;}
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&){throw 1;}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap(){throw 1;}
CCharString::CCharString():PStringData((CCharStringData*)trackedAlloc(4)){}
CCharString::~CCharString(){noteString(this,1);trackedFree(PStringData);}
CWideString::CWideString():PStringData((CWideStringData*)trackedAlloc(4)){}
CWideString::~CWideString(){noteString(this,0);trackedFree(PStringData);}


void NUISystem::CUIMiscThingsDef::Copy(const CDefClassBase*){throw 1;}
unsigned long NUISystem::CUIMiscThingsDef::GetSizeofClass(){return sizeof(*this);}

void NUISystem::CUIMiscThingsDef::Transfer(CPersistContext&){throw 1;}
static void expect(void* p,int kind){expectedStrings[expectedCount]=p;expectedKinds[expectedCount++]=kind;}
struct UIDtorFixture {
 template<class K,class V>static void fill(std::map<K,V>& m,unsigned int n){
  typedef std::_Rb_tree_node<std::pair<const K,V> > Node;
  Node* parent=0;
  for(unsigned int i=0;i<n;++i){Node* node=(Node*)trackedAlloc(sizeof(Node));new(&node->_M_value_field)std::pair<const K,V>();node->_M_left=0;node->_M_right=0;node->_M_parent=parent;node->_M_color=false;
   if(parent)parent->_M_left=node;else m._M_t._M_header._M_data->_M_parent=node;parent=node;}
  m._M_t._M_node_count=n;
 }
 static void keyOrder(std::map<CCharString,long>& m){
  typedef std::_Rb_tree_node<std::pair<const CCharString,long> > Node;
  Node* node=(Node*)m._M_t._M_header._M_data->_M_parent;
  while(node){expect((void*)&node->_M_value_field.first,1);node=(Node*)node->_M_left;}
 }
 static void parentStorage(NUISystem::CUIMiscThingsDef& d,unsigned int n){
  typedef std::pair<unsigned long,CDefClassBase::CSubDefInfo> Value;
  if(n){Value* p=(Value*)trackedAlloc(n*sizeof(Value));for(unsigned int i=0;i<n;++i)new(p+i)Value();d.SubDefInfoMap._M_start=p;d.SubDefInfoMap._M_finish=p+n;d.SubDefInfoMap._M_end_of_storage._M_data=p+n;}
 }
};
static void run(unsigned int first,unsigned int second,unsigned int parentSize){
 allocationCount=freeCount=resourceDestroyed=visits=expectedCount=0;
 NUISystem::CUIMiscThingsDef* p=static_cast<NUISystem::CUIMiscThingsDef*>(NUISystem::CUIMiscThingsDef::Alloc());
 UIDtorFixture::fill(p->MiniMapGraphics,first);UIDtorFixture::fill(p->MapPaths,second);UIDtorFixture::parentStorage(*p,parentSize);
 UIDtorFixture::keyOrder(p->MiniMapGraphics);
 expect(&p->FrontEndMusic,0);
 expect(&p->SoundKeyboardDone,1);
 expect(&p->SoundKeyboardDeleteCharacter,1);
 expect(&p->SoundKeyboardEnterCharacter,1);
 expect(&p->SoundKeyboardRight,1);
 expect(&p->SoundKeyboardLeft,1);
 expect(&p->SoundKeyboardDown,1);
 expect(&p->SoundKeyboardUp,1);
 expect(&p->CountUpSound,1);
 expect(&p->SoundExit,1);
 expect(&p->SoundError,1);
 expect(&p->SoundForward,1);
 expect(&p->SoundBack,1);
 expect(&p->SoundSlider,1);
 expect(&p->SoundUpDown,1);
 expect(&p->LogBookStoryCategoryNameString,0);
 expect(&p->LogBookQuestCategoryNameString,0);
 expect(&p->LogBookCombatCategoryNameString,0);
 expect(&p->LogBookHeroCategoryNameString,0);
 expect(&p->LogBookTownsCategoryNameString,0);
 expect(&p->LogBookObjectsCategoryNameString,0);
 expect(&p->LogBookBasicsCategoryNameString,0);
 expect(&p->LogBookStoryCategoryString,0);
 expect(&p->LogBookQuestCategoryString,0);
 expect(&p->LogBookCombatCategoryString,0);
 expect(&p->LogBookHeroCategoryString,0);
 expect(&p->LogBookTownsCategoryString,0);
 expect(&p->LogBookObjectsCategoryString,0);
 expect(&p->LogBookBasicsCategoryString,0);
 expect(&p->LevelString,0);
 expect(&p->TatooString,0);
 expect(&p->BarberString,0);
 expect(&p->TitleString,0);
 expect(&p->GeneralString,0);
 expect(&p->SaleString,0);
 expect(&p->ForString,0);
 expect(&p->IsString,0);
 expect(&p->AreString,0);
 expect(&p->ThereString,0);
 expect(&p->ShopString,0);
 expect(&p->ShopsString,0);
 expect(&p->InString,0);
 expect(&p->HouseString,0);
 expect(&p->HousesString,0);
 expect(&p->NoString,0);
 expect(&p->OwnString,0);
 expect(&p->YouString,0);
 expect(&p->QuestStartingString,0);
 expect(&p->CheckGuildString,0);
 expect(&p->NoneString,0);
 expect(&p->ObjectsRewardString,0);
 expect(&p->Minus,0);
 expect(&p->Plus,0);
 expect(&p->SucceededString,0);
 expect(&p->FailedString,0);
 expect(&p->QuestFailedString,0);
 expect(&p->TradeWantedString,0);
 expect(&p->TradeSellString,0);
 expect(&p->TradeBuyString,0);
 expect(&p->TradeDaysString,0);
 expect(&p->TradeNoDeliveryString,0);
 expect(&p->TradeDeliveryString,0);
 expect(&p->TradeNumberInStockString,0);
 expect(&p->TradeAlreadyOwnsString,0);
 expect(&p->TradeLossString,0);
 expect(&p->TradeProfitString,0);
 expect(&p->TradeCostString,0);
 expect(&p->WeaponDamageString,0);
 expect(&p->WeaponCatRangedString,0);
 expect(&p->WeaponCatMeleeString,0);
 expect(&p->WeaponKillsString,0);
 expect(&p->WeaponHeavyString,0);
 expect(&p->WeaponLightString,0);
 expect(&p->WeaponWeightString,0);
 expect(&p->WeaponAugNone,0);
 expect(&p->WeaponAugString,0);
 expect(&p->WeaponValueString,0);
 expect(&p->Positive,0);
 expect(&p->CloseBracket,0);
 expect(&p->OpenBracket,0);
 expect(&p->NewLineSeparator,0);
 expect(&p->ColonSeparator,0);
 expect(&p->CommaSeparator,0);
 expect(&p->SpaceSeparator,0);

 checking=true;delete p;checking=false;
 if(visits!=expectedCount||freeCount!=allocationCount||resourceDestroyed!=1)fail(4);
}
int main(){for(unsigned int a=0;a<4;++a)for(unsigned int b=0;b<4;++b)for(unsigned int c=0;c<3;++c)run(a,b,c);puts("004317d0_TEST PASS");return 0;}