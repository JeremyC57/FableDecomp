// std::map<long,pair<long,long> >::operator[] -- actual lowerbound/hinted insertion/value reference.
// Leaf: ??A?$map@JU?$pair@JJ@std@@U?$less@J@2@V?$allocator@U?$pair@$$CBJU?$pair@JJ@std@@@std@@@2@@std@@QAEAAU?$pair@JJ@1@ABJ@Z
// Flags: /O2 /Oy /W3
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
#include <new>
extern "C" void*__cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T,class U>inline void __fastcall _Construct(T*p,const U&x){::new(static_cast<void*>(p))T(x);}
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}T*allocate(size_t n,const void* =0){return n?static_cast<T*>(malloc(n*sizeof(T))):0;}void deallocate(T*p,size_t){free(p);}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static void __cdecl _Rebalance(_Base_ptr,_Base_ptr&);static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type&);~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type _M_create_node(const value_type&x){_Link_type n=this->_M_header.allocate(1);_Construct(&n->_M_value_field,x);return n;}
 _Link_type&_M_leftmost()const{return(_Link_type&)this->_M_header._M_data->_M_left;}
 static _Link_type&__cdecl _S_parent(_Link_type p){return(_Link_type&)p->_M_parent;}
 _Link_type&_M_root()const{return(_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_left(_Link_type p){return(_Link_type&)p->_M_left;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
private:_Link_type _M_lower_bound(const key_type&key)const{_Link_type y=this->_M_header._M_data;_Link_type x=_M_root();while(x!=0)if(!_M_key_compare(_S_key(x),key))y=x,x=_S_left(x);else x=_S_right(x);return y;}
public:iterator begin(){return iterator(_M_leftmost());}iterator lower_bound(const key_type&key){return iterator(_M_lower_bound(key));}iterator end(){return iterator(this->_M_header._M_data);}Compare key_comp()const{return _M_key_compare;}
 size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class K,class V,class Key,class C,class A>typename _Rb_tree<K,V,Key,C,A>::iterator _Rb_tree<K,V,Key,C,A>::_M_insert(_Base_ptr x0,_Base_ptr y0,const value_type&value,_Base_ptr w0){_Link_type w=(_Link_type)w0,x=(_Link_type)x0,y=(_Link_type)y0,z;if(y==this->_M_header._M_data||(w==0&&(x!=0||_M_key_compare(Key()(value),_S_key(y))))){z=_M_create_node(value);_S_left(y)=z;if(y==this->_M_header._M_data){_M_root()=z;_M_rightmost()=z;}else if(y==_M_leftmost())_M_leftmost()=z;}else{z=_M_create_node(value);_S_right(y)=z;if(y==_M_rightmost())_M_rightmost()=z;}_S_parent(z)=y;_S_left(z)=0;_S_right(z)=0;_Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;return iterator(z);}
template<class K,class V,class Key,class C,class A>pair<typename _Rb_tree<K,V,Key,C,A>::iterator,bool> _Rb_tree<K,V,Key,C,A>::insert_unique(const value_type&value){_Link_type y=this->_M_header._M_data,x=_M_root();bool comp=true;while(x!=0){y=x;comp=_M_key_compare(Key()(value),_S_key(x));x=comp?_S_left(x):_S_right(x);}iterator j=iterator(y);if(comp){if(j==begin())return pair<iterator,bool>(_M_insert(y,y,value),true);else --j;}if(_M_key_compare(_S_key(j._M_node),Key()(value)))return pair<iterator,bool>(_M_insert(x,y,value),true);return pair<iterator,bool>(j,false);}
template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc> typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> ::insert_unique(iterator __position, const _Value& __v)
{
  if (__position._M_node == this->_M_header._M_data->_M_left) { // begin()

    // if the container is empty, fall back on insert_unique.
    if (size() <= 0)
      return insert_unique(__v).first;

    if ( _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
      return _M_insert(__position._M_node, __position._M_node, __v);
    // first argument just needs to be non-null 
    else
      {
	bool __comp_pos_v = _M_key_compare( _S_key(__position._M_node), _KeyOfValue()(__v) );
	
	if (__comp_pos_v == false)  // compare > and compare < both false so compare equal
	  return __position;
	//Below __comp_pos_v == true

	// Standard-conformance - does the insertion point fall immediately AFTER
	// the hint?
	iterator __after = __position;
	++__after;

	// Check for only one member -- in that case, __position points to itself,
	// and attempting to increment will cause an infinite loop.
	if (__after._M_node == this->_M_header._M_data)
	  // Check guarantees exactly one member, so comparison was already
	  // performed and we know the result; skip repeating it in _M_insert
	  // by specifying a non-zero fourth argument.
	  return _M_insert(0, __position._M_node, __v, __position._M_node);
		
	
	// All other cases:
	
	// Optimization to catch insert-equivalent -- save comparison results,
	// and we get this for free.
	if(_M_key_compare( _KeyOfValue()(__v), _S_key(__after._M_node) )) {
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	    return insert_unique(__v).first;
	}
      }

  } else if (__position._M_node == this->_M_header._M_data) { // end()
    if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
      // pass along to _M_insert that it can skip comparing
      // v, Key ; since compare Key, v was true, compare v, Key must be false.
      return _M_insert(0, _M_rightmost(), __v, __position._M_node); // Last argument only needs to be non-null
    else
      return insert_unique(__v).first;
  } else {
    iterator __before = __position;
    --__before;
    
    bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));

    if (__comp_v_pos
      && _M_key_compare( _S_key(__before._M_node), _KeyOfValue()(__v) )) {

      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v, __before._M_node); // Last argument only needs to be non-null
      else
        return _M_insert(__position._M_node, __position._M_node, __v);
    // first argument just needs to be non-null 
    } else
      {
	// Does the insertion point fall immediately AFTER the hint?
	iterator __after = __position;
	++__after;
	
	// Optimization to catch equivalent cases and avoid unnecessary comparisons
	bool __comp_pos_v = !__comp_v_pos;  // Stored this result earlier
	// If the earlier comparison was true, this comparison doesn't need to be
	// performed because it must be false.  However, if the earlier comparison
	// was false, we need to perform this one because in the equal case, both will
	// be false.
	if (!__comp_v_pos) __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
	
	if ( (!__comp_v_pos) // comp_v_pos true implies comp_v_pos false
	     && __comp_pos_v
	     && (__after._M_node == this->_M_header._M_data ||
	        _M_key_compare( _KeyOfValue()(__v), _S_key(__after._M_node) ))) {
	  
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	  // Test for equivalent case
	  if (__comp_v_pos == __comp_pos_v)
	    return __position;
	  else
	    return insert_unique(__v).first;
	}
      }
  }
}

template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef K key_type;typedef V mapped_type;typedef pair<const K,V>value_type;typedef Compare key_compare;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,key_compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;
 iterator end(){return _M_t.end();}key_compare key_comp()const{return _M_t.key_comp();}
 iterator lower_bound(const key_type&key){return _M_t.lower_bound(key);}
 iterator insert(iterator hint,const value_type&value){return _M_t.insert_unique(hint,value);}
 V&operator[](const key_type&key);
};
template<class K,class V,class C,class A>V&map<K,V,C,A>::operator[](const key_type&key){
 iterator i=lower_bound(key);
 if(i==end()||key_comp()(key,(*i).first))i=insert(i,value_type(key,V()));
 return(*i).second;
}
template pair<long,long>&map<long,pair<long,long> >::operator[](const long&);
}