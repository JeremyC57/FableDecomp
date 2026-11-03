#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#include "stlport/fable_containers.h"
// STLport4.6.2 _tree.h86-113,140-170,197-218,220-265 and _map.h43-85.
// Genuine names, class keys, inheritance and full native three-word tree storage.
namespace std {
#ifdef FABLE_RETAIL_FACTIONS
template<class I>inline ptrdiff_t __fastcall __distance(const I& first,const I& last,const bidirectional_iterator_tag&){
    ptrdiff_t n=0;I it(first);while(it!=last){++it;++n;}return n;
}
template<class I>inline ptrdiff_t __fastcall distance(const I& first,const I& last){return __distance(first,last,bidirectional_iterator_tag());}
#endif
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
template<class Dummy> class _Rb_global {
#if defined(FABLE_RETAIL_SHOP_TREE_BODY) || defined(FABLE_RETAIL_HERO_LOGBOOK)
    typedef _Rb_tree_node_base* _Base_ptr;
#endif
public:
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_SHOP_TREE_BODY) || defined(FABLE_RETAIL_HERO_LOGBOOK)
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
#endif
#if defined(FABLE_RETAIL_SHOP_TREE_BODY) || defined(FABLE_RETAIL_HERO_LOGBOOK)
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node) {
  if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
    _M_node = _M_node->_M_right;
  else if (_M_node->_M_left != 0) {
    _Base_ptr __y = _M_node->_M_left;
    while (__y->_M_right != 0)
      __y = __y->_M_right;
    _M_node = __y;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_left) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    _M_node = __y;
  }
  return _M_node;
}
#endif
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
#endif
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#if defined(FABLE_RETAIL_SHOP_TREE_BODY) || defined(FABLE_RETAIL_HERO_LOGBOOK)
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
#endif
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
public:
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
#else
    _Rb_tree_base(const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
#else
    ~_Rb_tree_base();
#endif
#endif
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
#ifdef FABLE_RETAIL_COMBAT_TYPE
    iterator insert_equal(const V&);
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
#endif
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#else
    _Rb_tree(const Compare&,const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree(){clear();}
#else
    ~_Rb_tree();
#endif
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
private:
    void _M_erase(_Rb_tree_node<V>*);
public:
    void clear() {
        if(_M_node_count!=0) {
            _M_erase(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_parent));
            this->_M_header._M_data->_M_left=this->_M_header._M_data;
            this->_M_header._M_data->_M_parent=0;
            this->_M_header._M_data->_M_right=this->_M_header._M_data;
            _M_node_count=0;
        }
    }
#ifndef FABLE_RETAIL_QUICK_ACCESS
    iterator find(const K&);
#endif
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_FACTIONS)
protected:
    typedef _Rb_tree_node<V>* _Link_type;
#if defined(FABLE_RETAIL_SHOP_TREE_BODY) || defined(FABLE_RETAIL_HERO_LOGBOOK)
    _Link_type& _M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
    _Link_type& _M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
    static _Link_type& __fastcall _S_parent(_Link_type x){return (_Link_type&)x->_M_parent;}
    static _Link_type& __fastcall _S_left(_Rb_tree_node_base* x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Rb_tree_node_base* x){return (_Link_type&)x->_M_right;}
    static const K& __fastcall _S_key(_Rb_tree_node_base* x){return KeyOfValue()(static_cast<_Link_type>(x)->_M_value_field);}
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
    size_t size()const{return _M_node_count;}
protected:
#endif
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
public:
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_QUICK_ACCESS)
    iterator find(const K& key){return iterator(_M_find(key));}
#endif
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
#ifdef FABLE_RETAIL_FACTIONS
    iterator lower_bound(const K& key){return iterator(_M_lower_bound(key));}
    iterator upper_bound(const K& key){return iterator(_M_upper_bound(key));}
#else
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
#endif
#endif
#ifdef FABLE_RETAIL_FACTIONS
private:
    _Link_type _M_lower_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
    _Link_type _M_upper_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(_M_key_compare(key,_S_key(x))){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
public:
    pair<iterator,iterator> equal_range(const K& key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
    void erase(iterator,iterator);
    size_t erase(const K& key){pair<iterator,iterator> p=equal_range(key);size_t n=distance(p.first,p.second);erase(p.first,p.second);return n;}
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
#if defined(FABLE_RETAIL_QUICK_ACCESS)
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Rb_tree_node<V>* x) {
    while(x!=0) {
        _M_erase(static_cast<_Rb_tree_node<V>*>(x->_M_right));
        _Rb_tree_node<V>* y=static_cast<_Rb_tree_node<V>*>(x->_M_left);
        _Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x,1);
        x=y;
    }
}
#endif
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#ifdef FABLE_RETAIL_WASP
    ~map();
#endif
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    size_t size() const { return _M_t.size(); }
#endif
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_EXPOSE_MAYOR) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    iterator find(const K& key){return _M_t.find(key);}
#endif
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif
