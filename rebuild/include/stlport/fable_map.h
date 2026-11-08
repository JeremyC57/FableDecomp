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
#ifdef FABLE_RETAIL_STLP_TREE_POSTINC
    // chunk 778 (row 0x00ACCBF0): STLport 4.6 _tree.h post-increment (`_Self __tmp = *this; _M_increment(); return __tmp;`).
    _Rb_tree_iterator operator++(int){_Rb_tree_iterator tmp=*this;this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return tmp;}
#endif
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
#ifdef FABLE_RETAIL_STLP_SET_EMPTY
    // chunk 849 (row 0x00501450): STLport 4.6 _tree.h `bool empty() const { return _M_node_count == 0; }`.
    bool empty() const { return _M_node_count==0; }
#endif
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
#ifdef FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
    // Per-TU opt-in (chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList; like
    // FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE): STLport's clear is one inline body, which VC7.1 expanded in some TUs and
    // called as a COMDAT in others; that TU calls the map<long,NUISystem::CComponent*> tree's clear 0x00593235 twice (the
    // landed CTCInventory::CloseInventory 0x00589D30 calls the same body). Declaration only; the body below is retail's.
    void clear();
#else
    void clear() {
        if(_M_node_count!=0) {
            _M_erase(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_parent));
            this->_M_header._M_data->_M_left=this->_M_header._M_data;
            this->_M_header._M_data->_M_parent=0;
            this->_M_header._M_data->_M_right=this->_M_header._M_data;
            _M_node_count=0;
        }
    }
#endif
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
#ifdef FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
    // Retail opt-in (chunk 890, row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw): owner decision D1 (2026-11-03) for the
    // non-const twin -- STLport 4.6.1 _tree.h 457-467 `template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }`
    // under _STLP_MEMBER_TEMPLATES. Retail calls the map<ETableSprites,CCountedPointer<CComponent> > instance out of line (0x00553520,
    // `push &key ; push &result ; call`, hidden iterator result) at three sites and expands it at the others. Same body.
    template<class KT> iterator find(const KT& key){return iterator(_M_find(key));}
#else
    iterator find(const K& key){return iterator(_M_find(key));}
#endif
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
#ifdef FABLE_RETAIL_STLP_TREE_CONSTFIND
    // Retail opt-in (chunk 846, row 0x0051FD80 via CEntityRuntimePersistence::IsEntityPersistedAsDead): STLport 4.6 _tree.h
    // `const_iterator find(const _KT& __k) const { return const_iterator(_M_find(__k)); }` -- the const twin calls the out-of-line
    // _M_find (0x00527D20, landed) as the non-const one does. Body only: no layout change.
    const_iterator find(const K& key) const { return const_iterator(_M_find(key)); }
#else
#ifdef FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
    // Retail opt-in (chunk 848, row 0x00B2D9F0 CEngineSurfaceCompositionManager::UpdateDiskCache): owner decision D1 (2026-11-03) --
    // STLport 4.6.1 _tree.h 457-467 declare find as member templates under _STLP_MEMBER_TEMPLATES (VC7.1: set). Retail calls the
    // const instance out of line (0x00B2EB20 for set<long>: `push &key ; push &result ; call`, hidden const_iterator result).
    // Declaration only.
    template<class KT> const_iterator find(const KT& key) const;
#else
    const_iterator find(const K&) const;
#endif
#endif
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    // Retail opt-in (chunk 778, row 0x00ACCBF0 COptimisedPrimitive::PerformSubContraction): STLport 4.6 _tree.h erase(iterator)
    // (rebalance, destroy, deallocate; out of line in retail: 0x00AD4CA0 map<unsigned long,CContractionReference>, 0x00AD3C30
    // multimap<float,CCountedPointer<CVertexPairContraction> >). Declaration only.
    void erase(iterator position);
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_KEY
    // Retail opt-in (chunk 867, row 0x005B01FE CTCInventoryTrade::SellItemPC): STLport 4.6 _tree.h upper_bound / equal_range and
    // _tree.c `size_type erase(const key_type&)` (equal_range, distance, erase(first, last); returns the number removed) and
    // `void erase(iterator, iterator)` (clear() for the whole range, else erase(first++) until last). The bodies are visible, as in
    // every retail TU that includes <map>, so VC7.1's mod/ref summary sees the key only read through its reference (the caller's
    // dead parameter word can then be shared: 0x005B01FE keeps its GetTC keys and `found` flag in [ebp+8]); VC7.1 still calls
    // erase(const K&) out of line: 0x005B2779 for map<NUISystem::CComponent*,std::pair<long,long> > (ItemsToSell), 0x005B27D6 for
    // map<NUISystem::CComponent*,NUISystem::CList*> (SellListsByCategory). erase(iterator) itself is the FABLE_RETAIL_STLP_TREE_ERASE
    // declaration (out of line in retail). map::erase(const K&) below forwards here as _map.h does.
    iterator upper_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
    pair<iterator,iterator> equal_range(const K& key) { return pair<iterator,iterator>(lower_bound(key),upper_bound(key)); }
    void erase(iterator first,iterator last) {
        if(first==begin()&&last==end()) clear();
        else while(first!=last) erase(first++);
    }
    size_t erase(const K& key) {
        pair<iterator,iterator> p=equal_range(key);
        size_t n=0;
        for(iterator i=p.first;i!=p.second;++i) ++n;
        erase(p.first,p.second);
        return n;
    }
#endif
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
#ifdef FABLE_RETAIL_STLP_MAP
// Retail branch (Codex pass 13, row 0x00501450): the STLport 4.6 _tree.c insert_unique(const V&) body, visible as in every
// retail TU that includes <map> (STLport compiles _tree.c into the header unless _STLP_LINK_TIME_INSTANTIATION).
template<class K,class V,class KeyOfValue,class Compare,class A>
pair<_Rb_tree_iterator<V,_Nonconst_traits<V> >,bool> _Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(const V& v) {
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    bool comp=true;
    while(x!=0) {
        y=x;
        comp=_M_key_compare(KeyOfValue()(v),_S_key(x));
        x=comp?_S_left(x):_S_right(x);
    }
    iterator j=iterator(y);
    if(comp)
        if(j==begin()) return pair<iterator,bool>(_M_insert(y,y,v),true);
        else --j;
    if(_M_key_compare(_S_key(j._M_node),KeyOfValue()(v)))
        return pair<iterator,bool>(_M_insert(x,y,v),true);
    return pair<iterator,bool>(j,false);
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
#ifdef FABLE_RETAIL_STLP_MAP
    // Retail branch (chunk 726): STLport 4.6 _map.h defines operator[] in the class (lower_bound, then insert(i,
    // value_type(k, V())) when the key is absent). VC7.1 keeps it out of line (0x0042D5B1 for map<unsigned long,CUIState>)
    // but the visible body lets it see that the key temporary is only read, so it hoists the loop-invariant key stores
    // (NUISystem::CList::ChangeState 0x0053A0A1) and schedules each key store right before its call (CList::ScrollUp
    // 0x005385AE / ScrollDown 0x005392FC). Regraded: 0x00E0A820 (the only landed FABLE_RETAIL_STLP_MAP user) stays matched.
    V& operator[](const K& k) {
        iterator i=lower_bound(k);
        if(i==end() || key_comp()(k,(*i).first)) i=insert(i,value_type(k,V()));
        return (*i).second;
    }
#else
    V& operator[](const K&);
#endif
    size_t size() const { return _M_t.size(); }
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    void erase(iterator position){_M_t.erase(position);}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_KEY
    size_t erase(const K& key){return _M_t.erase(key);}
#endif
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
#ifdef FABLE_RETAIL_STLP_SET_COPY
    // Retail branch (chunk 777, row 0x00AC92C0 COptimisedPrimitive::ComputeContraction): STLport 4.6 _set.h `set(const _Self& x)
    // : _M_t(x._M_t) {}` with the _Rb_tree copy expanded is too large for VC7.1 to inline: retail calls the out-of-line copy
    // ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z (0x00AD3A10, landed) for each CIndexList copy.
    set(const set& x);
#endif
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
    size_t erase(const K& value){return _M_t.erase(value);}
#ifdef FABLE_RETAIL_STLP_SET_FIND
    // Retail opt-in (chunk 848, row 0x00B2D9F0): STLport 4.6 _set.h `iterator find(const key_type& x) const { return _M_t.find(x); }`
    // (inline; the tree's const find stays the out-of-line call 0x00B2EB20). Pair with FABLE_RETAIL_STLP_SET for end().
    iterator find(const K& key) const { return _M_t.find(key); }
#endif
#ifdef FABLE_RETAIL_STLP_SET
    // Retail branch (chunk 728, row 0x006D2BD0 CTCWife::OnSerialise): STLport 4.6 _set.h begin() / end() / size() (inline: retail
    // walks CTCWife::ThingsSeenThePlayerWear with the expanded _Rb_global::_M_increment and reads _M_node_count +4 in place).
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    size_t size() const { return _M_t.size(); }
#endif
#ifdef FABLE_RETAIL_STLP_SET_EMPTY
    // Per-TU opt-in (chunk 849, row 0x00501450 CWorldMap::GenerateRegionConnectivityGraph): STLport 4.6 _set.h `bool empty() const
    // { return _M_t.empty(); }`, inline (the row tests the local set's _M_node_count in place: `mov eax,[esp+58h] ; test eax,eax`,
    // 0x005016E6).
    bool empty() const { return _M_t.empty(); }
#endif
#ifdef FABLE_RETAIL_STLP_TREE_CONSTFIND
    // (same opt-in, chunk 846 row 0x00520D00) STLport 4.6 _set.h `iterator find(const key_type& __x) const { return _M_t.find(__x); }`:
    // retail calls the set<CCharString> _M_find 0x004B9370 (landed) and compares with the header node.
    iterator find(const K& x) const { return _M_t.find(x); }
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    // chunk 778 (row 0x00ACCBF0): STLport 4.6 _set.h clear() (`_M_t.clear()`; retail 0x00AD3900 is its out-of-line copy there).
    void clear(){_M_t.clear();}
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
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    void erase(iterator position){_M_t.erase(position);}
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif
