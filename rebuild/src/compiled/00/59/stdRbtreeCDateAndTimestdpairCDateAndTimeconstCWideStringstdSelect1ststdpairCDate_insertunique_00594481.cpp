// std::_Rb_tree::insert_unique(iterator,const value_type&) -- original hinted date/text insertion.
// Leaf: ?insert_unique@?$_Rb_tree@VCDateAndTime@@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@U?$_Select1st@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@3@U?$less@VCDateAndTime@@@3@V?$allocator@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@2@@2@U32@ABU?$pair@$$CBVCDateAndTime@@VCWideString@@@2@@Z
// Flags: /O2 /Oy /W3

#ifndef _MSC_VER
#define __cdecl
#endif
#pragma optimize("y",off)
#pragma optimize("s",on)
class CDateAndTime{public:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;bool operator<(const CDateAndTime&)const;};
class CWideStringData;class CWideString{public:CWideStringData*PStringData;};
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
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
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};
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

typedef pair<const CDateAndTime,CWideString>DateValue;typedef _Rb_tree<CDateAndTime,DateValue,_Select1st<DateValue>,less<CDateAndTime> >DateTree;
template DateTree::iterator DateTree::insert_unique(DateTree::iterator,const DateValue&);
}