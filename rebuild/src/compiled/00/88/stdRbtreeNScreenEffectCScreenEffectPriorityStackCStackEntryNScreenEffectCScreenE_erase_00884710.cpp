// Original STLport erase-by-key for independently typed nested pointee.
// Leaf: ?erase@?$_Rb_tree@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@PAU123@U?$_Identity@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@U?$less@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@5@V?$allocator@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@5@@std@@QAEIABQAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@Z
// Flags:
// Grade: pending first original model/name sourcegrade.
namespace NScreenEffect{class CScreenEffectPriorityStack{public:struct CStackEntry;};}
#include <stddef.h>
namespace std {
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct __true_type {};struct __false_type {};
template<class T>class allocator {public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;template<class U>struct rebind {typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}~allocator(){}};
template<class T,class A>struct _Alloc_traits {typedef A _Orig;typedef A allocator_type;};template<class T,class U>struct _Alloc_traits<T,allocator<U> > {typedef allocator<U>_Orig;typedef allocator<T>allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {typedef A _Base;typedef _STLP_alloc_proxy<V,T,A>_Self;public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};template<class A,class B,class R>struct binary_function {typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class A,class B>struct pair {typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}};
struct input_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};
template<class T>struct _Nonconst_traits {typedef T value_type;typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits {typedef T value_type;typedef const T*pointer;typedef const T&reference;typedef _Nonconst_traits<T>_Non_const_traits;};
template<class I>struct iterator_traits {typedef typename I::value_type value_type;typedef typename I::pointer pointer;typedef typename I::reference reference;typedef typename I::iterator_category iterator_category;typedef typename I::difference_type difference_type;};
struct _Rb_tree_node_base {typedef _Rb_tree_node_base*_Base_ptr;bool _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base {V _M_value_field;};
template<class Dummy>class _Rb_global {public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);};typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator {typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type x){_M_node=x;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&x){_M_node=x._M_node;}reference operator*()const{return static_cast<_Link_type>(_M_node)->_M_value_field;}_Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}};
#pragma auto_inline(off)
template<class I>__declspec(noinline) typename iterator_traits<I>::difference_type __cdecl __distance(const I&first,const I&last,const input_iterator_tag&){typename iterator_traits<I>::difference_type n=0;I it(first);while(it!=last){++it;++n;}return n;}
#pragma auto_inline(on)
template<class I>inline typename iterator_traits<I>::difference_type __cdecl distance(const I&first,const I&last){return __distance(first,last,typename iterator_traits<I>::iterator_category());}
template<class T,class A>struct _Rb_tree_base {typedef _Rb_tree_node<T>_Node;typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Rb_tree_base(const allocator_type&);~_Rb_tree_base();protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;};
template<class K,class V,class Select,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A> {typedef _Rb_tree_base<V,A>_Base;protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 static _Link_type&__cdecl _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
 static _Link_type&__cdecl _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
 static reference __cdecl _S_value(_Link_type x){return x->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type x){return Select()(_S_value(x));}
private:
 _Link_type _M_lower_bound(const key_type&key)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}return y;}
 _Link_type _M_upper_bound(const key_type&key)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x){if(_M_key_compare(key,_S_key(x))){y=x;x=_S_left(x);}else x=_S_right(x);}return y;}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;_Rb_tree();~_Rb_tree();iterator lower_bound(const key_type&key){return iterator(_M_lower_bound(key));}iterator upper_bound(const key_type&key){return iterator(_M_upper_bound(key));}__declspec(noinline) pair<iterator,iterator>equal_range(const key_type&key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}void erase(iterator,iterator);size_type erase(const key_type&key){pair<iterator,iterator>p=equal_range(key);size_type n=distance(p.first,p.second);erase(p.first,p.second);return n;}};
}

namespace std {
typedef NScreenEffect::CScreenEffectPriorityStack::CStackEntry* _V;
template unsigned int _Rb_tree<_V,_V,_Identity<_V>,less<_V>,allocator<_V> >::erase(_V const&);
}