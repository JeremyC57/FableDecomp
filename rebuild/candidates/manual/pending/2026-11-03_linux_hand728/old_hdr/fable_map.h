#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
// STLport4.6.2 _tree.h86-113,140-170,197-218,220-265 and _map.h43-85.
// One model for every translation unit (Codex pass 11 consolidation).
// Genuine names, class keys, inheritance and full native three-word tree storage.
namespace std {
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
template<class Dummy> class _Rb_global {
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
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
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
};
template<class V,class A>struct _Rb_tree_base {
public:
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
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
    iterator insert_equal(const V&);
private:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
    ~_Rb_tree(){clear();}
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
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
protected:
    typedef _Rb_tree_node<V>* _Link_type;
    _Link_type& _M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
    _Link_type& _M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
    static _Link_type& __fastcall _S_parent(_Link_type x){return (_Link_type&)x->_M_parent;}
    static _Link_type& __fastcall _S_left(_Rb_tree_node_base* x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Rb_tree_node_base* x){return (_Link_type&)x->_M_right;}
    static const K& __fastcall _S_key(_Rb_tree_node_base* x){return KeyOfValue()(static_cast<_Link_type>(x)->_M_value_field);}
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
protected:
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
    iterator find(const K& key){return iterator(_M_find(key));}
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
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
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::iterator iterator;
#ifdef FABLE_RETAIL_STLP_MAP
    // Retail branch (pass 12 header wave, 0x00E0A820): STLport 4.6 _map.h declares the no-argument constructor separately
    // (`map() : _M_t(_Compare(), allocator_type()) {}` and `explicit map(const _Compare&, const allocator_type& = ...)`), so a
    // default-constructed local passes no argument temporaries: retail calls ??0?$map@...@QAE@XZ (0x00CDBF70) with only `this`.
    map():_M_t(Compare(),A()){}
    explicit map(const Compare& c,const A& a=A()):_M_t(c,a){}
#else
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
#endif
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
    size_t size() const { return _M_t.size(); }
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
// STLport 4.6.2 _function.h _Identity, _set.h set and _multimap.h multimap (Codex pass 11 consolidation 2, row 0x007355F0:
// CTCCombat::CombatAbilities is a multimap<ECombatAbilityDataType,...>, CCombatAbilityBlockingData::ValidBlockWeaponTypes a
// set<EWeaponClass>). The members the rows use; the tree is the same three-word _Rb_tree.
template<class T> struct _Identity:public unary_function<T,T>{const T& operator()(const T& value)const{return value;}};
template<class K,class Compare=less<K>,class A=allocator<K> >
class set {
private:
    typedef _Rb_tree<K,K,_Identity<K>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator iterator;
    set():_M_t(Compare(),A()){}
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
    size_t erase(const K& value){return _M_t.erase(value);}
#ifdef FABLE_RETAIL_STLP_SET
    // Retail branch (chunk 728, row 0x006D2BD0 CTCWife::OnSerialise): STLport 4.6 _set.h begin() / end() / size() (inline: retail
    // walks CTCWife::ThingsSeenThePlayerWear with the expanded _Rb_global::_M_increment and reads _M_node_count +4 in place).
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    size_t size() const { return _M_t.size(); }
#endif
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >
class multimap {
public:
    typedef pair<const K,T> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::iterator iterator;
    multimap():_M_t(Compare(),A()){}
    iterator insert(const value_type& x){return _M_t.insert_equal(x);}
    void clear(){_M_t.clear();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif
