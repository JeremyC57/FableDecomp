#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#include "fable_compiler.h"   // NF61 opt-in: FABLE_FORCEINLINE for _M_clone_node below
#endif
// STLport4.6.2 _tree.h86-113,140-170,197-218,220-265 and _map.h43-85.
// One model for every translation unit (Codex pass 11 consolidation).
// Genuine names, class keys, inheritance and full native three-word tree storage.
namespace std {
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
// Original STLport type_traits.h283..350,389..404 compatibility default-construction path.
template<class T>struct _Is_integer{typedef __false_type _Integral;};
#define FABLE_PASS22_STLP_INTEGER(T) template<>struct _Is_integer<T>{typedef __true_type _Integral;};
FABLE_PASS22_STLP_INTEGER(bool)
FABLE_PASS22_STLP_INTEGER(char)
FABLE_PASS22_STLP_INTEGER(signed char)
FABLE_PASS22_STLP_INTEGER(unsigned char)
FABLE_PASS22_STLP_INTEGER(short)
FABLE_PASS22_STLP_INTEGER(unsigned short)
FABLE_PASS22_STLP_INTEGER(int)
FABLE_PASS22_STLP_INTEGER(unsigned int)
FABLE_PASS22_STLP_INTEGER(long)
FABLE_PASS22_STLP_INTEGER(unsigned long)
FABLE_PASS22_STLP_INTEGER(__int64)
FABLE_PASS22_STLP_INTEGER(unsigned __int64)
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
FABLE_PASS22_STLP_INTEGER(wchar_t)
#endif
#undef FABLE_PASS22_STLP_INTEGER
template<class T>inline T __default_constructed_aux(T*,const __false_type&){return T();}
template<class T>inline T __default_constructed_aux(T*,const __true_type&){return T(0);}
template<class T>inline T __default_constructed(T*p){typedef typename _Is_integer<T>::_Integral Integral;return __default_constructed_aux(p,Integral());}
#endif
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
#ifdef FABLE_RETAIL_STLP_TREE_COPY_CTOR_SMINMAX
    // NF84 per-TU opt-in (round 7, row 0x0053CB9B map<unsigned long, NUISystem::CUIState>::map(const map&)): STLport 4.6 _tree.h's static
    // node-base walks, declared only: that TU calls them out of line (`call 0x0041C7E0` / `call 0x0041C800`, __cdecl, landed
    // _Rb_tree_node_base::_S_minimum / _S_maximum; the landed 0x0042CE7A binds the same spellings). No layout change.
    static _Rb_tree_node_base* _S_minimum(_Rb_tree_node_base* x);       // 0x0041C7E0
    static _Rb_tree_node_base* _S_maximum(_Rb_tree_node_base* x);       // 0x0041C800
#endif
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
template<class Dummy> class _Rb_global {
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
public:
#endif
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
#ifdef FABLE_RETAIL_STLP_RB_GLOBAL_ROTATE
    // Opt-in (NF42 round 5, row 0x0042955B): STLport 4.6 _tree.c _Rb_global::_Rotate_left / _Rotate_right, out of line in retail
    // (0x0042951B / 0x0042955B, static __cdecl (node, root&)); the including row defines the body. Declarations only.
    static void __cdecl _Rotate_left(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static void __cdecl _Rotate_right(_Rb_tree_node_base*,_Rb_tree_node_base*&);
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_BODY
    // Nested opt-in (agentFAM12, the erase(first, last) rows 0x00638EE0 ..): STLport 4.6 _tree.c _Rb_global::_Rebalance_for_erase,
    // out of line (0x0042A34E, landed ?_Rebalance_for_erase@?$_Rb_global@_N@std@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z).
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base*,_Rb_tree_node_base*&,_Rb_tree_node_base*&,_Rb_tree_node_base*&);
#endif
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node);
#else
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
#ifdef FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);
#else
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
#endif
};
#if defined(FABLE_RETAIL_INVENTORY_ITER_CALL_PASS29) && defined(_MSC_VER)
// Inline-budget: native 005b1704 and the expanded increment at 005a33f9 both call the global member.
template<> __declspec(noinline) inline _Rb_tree_node_base* __cdecl _Rb_global<bool>::_M_increment(_Rb_tree_node_base*);
#endif
#ifdef FABLE_RETAIL_COMBATRING_ASSIGN_PASS27
// The same library specialization is selected from a size-built compiland at 004292C0.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline _Rb_tree_node_base* __cdecl _Rb_global<bool>::_M_increment(_Rb_tree_node_base* _M_node)
{
  if (_M_node->_M_right != 0) {
    _M_node = _M_node->_M_right;
    while (_M_node->_M_left != 0)
      _M_node = _M_node->_M_left;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_right) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    if (_M_node->_M_right != __y)
      _M_node = __y;
  }
  return _M_node;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
#ifdef FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
// Fidelity: scaffolding, not retail.
// The standalone 0x004292C0 owner uses optimize("s", on), as recorded in
// stdRbtreebase_Rbtreeincrement_004292c0.cpp. The block resets with
// optimize("", on). Put the consuming TU's own pragmas after this include.
#pragma optimize("s", on)
template<> inline _Rb_tree_node_base* __cdecl _Rb_global<bool>::_M_increment(_Rb_tree_node_base* node) {
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
#pragma optimize("", on)
#endif

#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
// Original _tree.c272: the global template member is defined out of class.
template<class Dummy> _Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
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
#if defined(FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY) && !defined(FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ) && defined(_MSC_VER)
// NF210 opt-in (2026-11-20, q4389; row 0x006EDA80 CCombatManager::RegisterAttackingCombatant). Fidelity: scaffolding, not retail.
// That /O2 TU sees the STLport insert_unique body (FABLE_RETAIL_STLP_MAP), whose `--j` calls the iterator's operator-- (pick-any copy
// of 0x006F0030), which calls _Rb_global<bool>::_M_decrement out of line, so the TU emits _M_decrement as a pick-any COMDAT. Retail
// kept the 57-byte copy from a size-optimised TU, row 0x004294E2; the plain /O2 copy differs. The advisor pick-any rule (EXTRA_RULES
// 2026-11-20) needs the owner's body, so this explicit specialization compiles the same STLport 4.6 _tree.c body (the in-class body
// above) under optimize("s"), as the owner's TU did (an out-of-class template definition under the pragma keeps the TU's /O2: the
// instance is compiled at the end of the TU, NF207). Owner check: the owner row 0x004294E2 sets `#pragma optimize("s", on)` itself.
// `#pragma optimize("", on)` below returns to the command-line settings (VC7.1 has no push / pop for optimize), so a row that uses
// this opt-in must not carry a row-head optimize pragma before its include of this header (nf189). Needs no use of
// _Rb_global<bool>::_M_decrement before this point.
#pragma optimize("s", on)
template<> inline _Rb_tree_node_base* __cdecl _Rb_global<bool>::_M_decrement(_Rb_tree_node_base* _M_node)
{
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
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. The 44-byte 0x004292C0 STLport
// owner is compiled with optimize("s", on). Instantiate that original body
// and restore the caller's command-line mode with optimize("", on).
#pragma optimize("s", on)
template<> inline _Rb_tree_node_base* __cdecl _Rb_global<bool>::_M_increment(_Rb_tree_node_base* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        _Rb_tree_node_base* parent = node->_M_parent;
        while (node == parent->_M_right) {
            node = parent;
            parent = parent->_M_parent;
        }
        if (node->_M_right != parent)
            node = parent;
    }
    return node;
}
#pragma optimize("", on)
#endif
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
#ifdef FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
    // NF62 opt-in (row 0x006C7370 CTCHeroOpinionDeedLog::OnSerialise, a map range through std::copy): the STLport 4.6 _tree.h iterator
    // typedefs (value_type here; iterator_category / difference_type on _Rb_tree_base_iterator) that iterator_traits<I> reads for the
    // __copy_aux dispatch. Typedefs only: no layout or code change.
    typedef V value_type;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
#endif
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#if defined(FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE) && defined(_MSC_VER)
    // Per-TU opt-in (wf20, row 0x00839100 CThingGameObject::GetWeaponTrailGraphicIndex): the same STLport body, visible but kept
    // out of line -- retail calls the map<EObjectAugmentationType,CWeaponTrailGraphicSet> iterator's operator-- as a thiscall COMDAT
    // (0x0083AEA0, landed as CRetailTreeIterator::Decrement: `mov eax,ecx ; push edi ; mov edi,[eax] ; cmp byte ptr [edi],0 ..`,
    // _M_decrement inlined, the node stored back once) from the reverse_iterator operator* / operator++ in that row.
    __declspec(noinline) _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
#else
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
#endif
#if defined(FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE) && defined(_MSC_VER)
    // NF58 opt-in (round 7, rows 0x00801EC0 / 0x00801CB0, the /O2 hinted insert_unique of map<EIdleStateGroup, long> /
    // map<EIdleStateGroup, CAIStateGroupBase*>): like FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE above, the same STLport body visible but
    // kept out of line -- retail calls each instance's operator++ as a thiscall COMDAT (0x008010D0 / 0x00801200, 62 bytes,
    // _M_increment inlined).
    __declspec(noinline) _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#else
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
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
#if defined(FABLE_RETAIL_STLP_TREE_MINSERT_NOINLINE) && defined(_MSC_VER)
    // NF67 opt-in (round 7, row 0x00869020 CHitParameters::SetAsModifiedByArmour, with FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): the
    // STLport _tree.c body visible but called out of line, as retail calls the armour multimap's _M_insert 0x00869CA0 there. Seeing that
    // it only reads the value, VC7.1 keeps the converted pair's def pointer in esi through the call and both releases (0x008690CB..
    // 0x008690E4), as retail does; an opaque call reloads it from the pair's frame slot.
    __declspec(noinline)
#endif
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
#ifdef FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
    // Per-TU opt-in (chunk Q32, row 0x00465C54 CCombatAbilityBlockDefBase::Transfer; like FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE):
    // STLport's inline tree constructor and destructor, which VC7.1 called as COMDATs in that size TU -- the set<EWeaponClass> tree
    // ctor 0x00451ADD (`push &alloc ; push &pred ; call`, base ctor 0x00451303 then _M_empty_initialize) and dtor 0x00451B07
    // (clear 0x004512DA, then the header node freed). STLport's bodies, visible but kept out of line (like
    // FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE): VC7.1 then knows they keep no reference to the caller's empty less/allocator
    // temporaries and packs those into a dead argument slot (0x00465CE6 `lea eax,[ebp+0Bh]` twice).
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
#else
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
#endif
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
#ifdef FABLE_RETAIL_STLP_SET_EMPTY
    // chunk 849 (row 0x00501450): STLport 4.6 _tree.h `bool empty() const { return _M_node_count == 0; }`.
    bool empty() const { return _M_node_count==0; }
#endif
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#ifdef FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY
    // NF178b opt-in (2026-11-20): the tree destructor declared only. VC7.1 instantiates ~_Rb_tree (and through it clear / _M_erase)
    // from the map constructor and emits them as pick-any COMDATs even where nothing calls them (only .debug$F refers); the 0x00CD3699
    // clear copy that makes is 51 bytes, retail's /Os body 41. Users: the cut-scene cast-map rows that already take
    // FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY (their leaf calls ~map 0x00CDBFB0; no ~_Rb_tree use is inlined in them).
    ~_Rb_tree();
#else
#ifdef FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    ~_Rb_tree(){clear();}
#else
    ~_Rb_tree(){clear();}
#endif
#endif
#ifdef FABLE_RETAIL_STLP_MAP_FORCEINLINE_BEGIN
    FABLE_FORCEINLINE   // with map::begin below
#endif
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
private:
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#ifdef _MSC_VER
#ifndef FABLE_RETAIL_STLP_TREE_MERASE_PASS22_INLINE
    // (The noinline is ReadFromFile 0x00A495F0's: that TU calls the tree erase. Opt-out FABLE_RETAIL_STLP_TREE_MERASE_PASS22_INLINE,
    // hand3c2 row 0x00A48BA0 CLipSyncSampleData::WriteToFile: the same map's destructor there has one level of the recursion in place,
    // 0x00A48EB0..0x00A48ED5 -- the right subtree through `call 0x00630D90`, the string destroyed and the node freed in the loop --
    // which is what VC7.1 does with the plain STLport body.)
    __declspec(noinline)
#endif
#endif
#else
#if defined(FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (row 0x004BD6E0 CTCGraphicAppearance::SetPrimitive): STLport's recursive _M_erase, visible but called out of line
    // (retail 0x004BD995 `call 0x004ABA70`, the set<CCharString> COMDAT); VC7.1 otherwise expands one level of the recursion in place.
    __declspec(noinline)
#endif
#endif
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
#if defined(FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE) && defined(_MSC_VER)
    // NF58 opt-in (round 7, row 0x00783F00, the /O2 _M_insert of map<unsigned __int64, CIntelligentPointer<CThing> >): the same
    // STLport body, visible but kept out of line -- retail calls that instance's _M_create_node as a thiscall COMDAT (0x00783DD0)
    // from both arms of _M_insert, where the /O2 siblings expand it.
    __declspec(noinline) _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
#else
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_MCOPY_BODY
    // NF61 opt-in (round 7, the size-TU tree copies 0x00432AA8 .. 0x004EC3B1): STLport 4.6 _tree.h `_M_clone_node` (create the node,
    // copy the colour, clear both children) and the private _tree.c `_M_copy(x, p)` structural copy (body below). Retail calls each
    // instance's _M_create_node COMDAT twice from _M_copy (`lea eax,[esi+10h] ; push eax ; call` per cloned node, then the colour /
    // child stores in place) and calls _M_copy from the tree's operator= and copy constructor. Inline-budget (EXTRA_RULES 2026-11-08,
    // preference 3): retail expands _M_clone_node at both sites; VC7.1 /Os calls it out of line, also under /d2inlS20 (0x00432AA8:
    // 115 v 144 bytes both ways), so it is forced here (the landed 0x0045D871 _M_copy needed the same).
    FABLE_FORCEINLINE _Link_type _M_clone_node(_Link_type x){_Link_type tmp=_M_create_node(x->_M_value_field);tmp->_M_color=x->_M_color;tmp->_M_left=0;tmp->_M_right=0;return tmp;}
private:
    _Link_type _M_copy(_Link_type x,_Link_type p);
#endif
#if defined(FABLE_RETAIL_STLP_TREE_ASSIGN_BODY) && !defined(FABLE_RETAIL_STLP_TREE_MCOPY_BODY)
    // NF64 opt-in (round 7, the size-TU tree assignments 0x00432A35 .. 0x004EC33E): the private _tree.c `_M_copy(x, p)` declared
    // only (retail calls each instance's _M_copy COMDAT from operator=; NF61's MCOPY_BODY rows define them). Body of operator= below.
private:
    _Link_type _M_copy(_Link_type x,_Link_type p);
#endif
#ifdef FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY
    // NF78 opt-in (round 7, row 0x004F97B3 map<EDamageAttribute,float>::map(const map&)): STLport 4.6.2 _tree.h copy constructor (the
    // header node allocated by the base, then an empty tree or the source root _M_copy'd under the header with leftmost / rightmost
    // re-derived, then the count). Inline; the map copy constructor (FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY) expands it and calls the
    // instance's _M_copy COMDAT (0x00461533 for that tree). The base takes no allocator state (fable_map.h's allocator is empty).
    // Inline-budget (EXTRA_RULES 2026-11-08, preference 3): forced -- VC7.1 /Os calls it out of line at every budget (row 0x004F97B3
    // grades: default, /d2inlS22, /d2inlS60 all 18 v 126 bytes).
public:
    FABLE_FORCEINLINE _Rb_tree(const _Rb_tree& x):_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(x._M_key_compare) {
        if(x._M_root()==0)
            _M_empty_initialize();
        else {
            this->_M_header._M_data->_M_color=false;
            _M_root()=_M_copy(x._M_root(),this->_M_header._M_data);
#ifdef FABLE_RETAIL_STLP_TREE_COPY_CTOR_SMINMAX
            // NF84 (row 0x0053CB9B): STLport's `_S_minimum(_M_root())` / `_S_maximum(_M_root())`, the node-base walks out of line.
            _M_leftmost()=static_cast<_Link_type>(_Rb_tree_node_base::_S_minimum(_M_root()));
            _M_rightmost()=static_cast<_Link_type>(_Rb_tree_node_base::_S_maximum(_M_root()));
#else
            _Rb_tree_node_base* n=_M_root();
            while(n->_M_left!=0) n=n->_M_left;
            _M_leftmost()=static_cast<_Link_type>(n);
            n=_M_root();
            while(n->_M_right!=0) n=n->_M_right;
            _M_rightmost()=static_cast<_Link_type>(n);
#endif
        }
        _M_node_count=x._M_node_count;
    }
#if !defined(FABLE_RETAIL_STLP_TREE_MCOPY_BODY) && !defined(FABLE_RETAIL_STLP_TREE_ASSIGN_BODY)
private:
    _Link_type _M_copy(_Link_type x,_Link_type p);
#endif
#endif
protected:
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
#ifdef FABLE_RETAIL_STLP_TREE_S_VALUE_CDECL
    // chunk 897 (row 0x0083AE10, an unreferenced COMDAT): STLport 4.6 _tree.h's plain `static reference _S_value(_Link_type)`
    // (?_S_value@...@KAAAU..., cdecl); the default __fastcall spelling below is the one the landed callers bind.
    static V& _S_value(_Link_type x){return x->_M_value_field;}
#else
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
#endif
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
#ifdef FABLE_RETAIL_STLP_TREE_MFIND_DECL
    // Per-TU opt-in (chunk 1020, row 0x006E94D0 CCombatManager::IsCreatureInAttackRingSector): STLport 4.6.1 _tree.h _M_find, called
    // out of line in that TU (map<unsigned __int64,CCombatantInfo> 0x006F0250: `push &key ; mov ecx,&Combatants ; call`, node in eax)
    // as an opaque call -- retail reloads the attacker's ring index from the first node after the second lookup (0x006E9528), which the
    // visible body lets VC7.1 hoist above it. Declaration only; same function.
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const;
#else
#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES
    // Inline-budget: native 00769fe0 expands the original search inside find.
    template<class Key> FABLE_FORCEINLINE _Rb_tree_node<V>* _M_find(const Key& key)const {
#elif defined(FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (NF50 round 7, rows 0x0059DB82 / 0x0059E6BD / 0x0059FAFE CTCInventoryTrade refresh): the same STLport 4.6.1 _tree.h
    // _M_find body, visible but kept out of line, as those TUs call the map<NUISystem::CComponent*,NUISystem::CList*> instance 0x005B1A43
    // (`push &key ; mov ecx,edi ; call`); seeing it writes nothing, VC7.1 keeps the map pointer in edi across the call (0x0059FB38..
    // 0x0059FB3F `cmp eax,[edi]`), where the declaration-only FABLE_RETAIL_STLP_TREE_MFIND_DECL reloads it (293 v 281 bytes).
    template<class Key> __declspec(noinline) _Rb_tree_node<V>* _M_find(const Key& key)const {
#else
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
#endif
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
#endif
public:
#if defined(FABLE_RETAIL_STLP_TREE_ASTAR_FIND_CALL_PASS23)
    // Native00a1d3a6 calls original non-template findA216A0; original full body visible.
    FABLE_STLP_NOINLINE iterator find(const K& key){return iterator(_M_find(key));}
#elif defined(FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL)
    // Row 0x00881A40 opt-in: the non-const member-template find declared only (retail calls the map<CThing*,
    // CThingGraphicByPassFlagState> instance out of line, 0x00883860, through the hidden iterator result).
    template<class KT> iterator find(const KT& key);
#elif defined(FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE) && defined(_MSC_VER)
    // Row 0x00881A40 opt-in: the same STLport body, visible but kept out of line (compiler steering, justified by retail bytes).
    // Retail calls the map<CThing*,CThingGraphicByPassFlagState> instance out of line at 0x00881ABB (call 0x00883860; the default
    // inline spelling expands the walk: 609 vs 581 bytes). Its compiler still saw the body: after the call it keeps the node in edi
    // (0x00881AC0 8B7C2410 mov edi,[esp+10h]; 0x00881AC4 3B3B cmp edi,[ebx]; 0x00881AC8 83C714 add edi,14h), where a declaration-only
    // find (_DECL above, opaque call) gives mov eax,[esp+10h]; cmp eax,[ebx]; lea edi,[eax+14h]. Only body + noinline gives both.
    template<class KT> __declspec(noinline) iterator find(const KT& key){return iterator(_M_find(key));}
#elif defined(FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE)
    // Retail opt-in (chunk 890, row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw): owner decision D1 (2026-11-03) for the
    // non-const twin -- STLport 4.6.1 _tree.h 457-467 `template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }`
    // under _STLP_MEMBER_TEMPLATES. Retail calls the map<ETableSprites,CCountedPointer<CComponent> > instance out of line (0x00553520,
    // `push &key ; push &result ; call`, hidden iterator result) at three sites and expands it at the others. Same body.
    template<class KT> iterator find(const KT& key){return iterator(_M_find(key));}
#else
    iterator find(const K& key){return iterator(_M_find(key));}
#endif
    pair<iterator,bool> insert_unique(const V&);
#ifdef FABLE_RETAIL_STLP_SET_RANGE
    // Retail opt-in (chunk 1018, row 0x00465C54): STLport 4.6 _tree.h insert_unique(_InputIterator, _InputIterator), inline (the
    // loop over a vector<EWeaponClass> calling insert_unique(const V&) 0x00465D36 per element).
    template<class II> void insert_unique(II first,II last) { for ( ; first!=last; ++first) insert_unique(*first); }
#endif
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_DECL_ONLY) && !defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY) && !defined(FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE)
    // NF210 opt-in (2026-11-20, q4389; row 0x006ED510 CCombatManager::DeregisterAttackingCombatant): STLport 4.6 _tree.c
    // `size_type erase(const key_type&)`, declared only, so the TU calls the owner rows' out-of-line instances (map<unsigned __int64,
    // CCombatantInfo> 0x006F50B0, map<unsigned __int64,CThingCreatureBase*> 0x006F3200, both landed under the ?erase@?$_Rb_tree@..QAEIAB_K@Z
    // spelling) and emits no pick-any copy of it or its helpers. Same function; declaration only. The map wrapper below forwards to it.
    size_t erase(const K& key);
#endif
#ifdef FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
    // Per-TU opt-in (wf4, row 0x004B3CE0 CQuestManager::ActivateQuests): STLport 4.6 _tree.h spelling -- lower_bound / upper_bound /
    // equal_range are inline wrappers over the out-of-line `_Base_ptr _M_lower_bound(const key_type&) const` / `_M_upper_bound`
    // (retail 0x004B9D00 / 0x004B9D50 for the CCharString-keyed trees, both landed), erase(iterator, iterator) is the out-of-line
    // 0x004B9C10 (landed) and erase(const key_type&) is `equal_range ; distance ; erase(first, last)`. Replaces the expanded walks
    // of the default path and of FABLE_RETAIL_STLP_TREE_ERASE_KEY for this TU (do not define both).
private:
#ifdef FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
    // Retail opt-in (Codex pass22): original STLport _tree.c lower/upper walks, same visible bodies as the native TU.
    _Rb_tree_node_base* _M_lower_bound(const K& key) const {
        _Link_type y=this->_M_header._M_data,x=_M_root();
        while(x!=0) { if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);} else x=_S_right(x); }
        return y;
    }
    _Rb_tree_node_base* _M_upper_bound(const K& key) const {
        _Link_type y=this->_M_header._M_data,x=_M_root();
        while(x!=0) { if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);} else x=_S_right(x); }
        return y;
    }
#else
#ifdef FABLE_RETAIL_INVENTORY_TYPED_LOWER_BOUND_PASS29
    // STLport 4.6.2 _tree.h returns the typed node. The inventory map consumes its pair at the native lower bound.
    _Link_type _M_lower_bound(const K& key) const;
#else
    _Rb_tree_node_base* _M_lower_bound(const K& key) const;
#endif
    _Rb_tree_node_base* _M_upper_bound(const K& key) const;
#endif
public:
    iterator lower_bound(const K& key) { return iterator(static_cast<_Link_type>(_M_lower_bound(key))); }
    const_iterator lower_bound(const K& key) const { return const_iterator(static_cast<_Link_type>(_M_lower_bound(key))); }
    iterator upper_bound(const K& key) { return iterator(static_cast<_Link_type>(_M_upper_bound(key))); }
    const_iterator upper_bound(const K& key) const { return const_iterator(static_cast<_Link_type>(_M_upper_bound(key))); }
    pair<iterator,iterator> equal_range(const K& key) { return pair<iterator,iterator>(lower_bound(key),upper_bound(key)); }
    pair<const_iterator,const_iterator> equal_range(const K& key) const { return pair<const_iterator,const_iterator>(lower_bound(key),upper_bound(key)); }
    void erase(iterator first,iterator last);
    size_t erase(const K& key) {
        pair<iterator,iterator> p=equal_range(key);
        size_t n=distance(p.first,p.second);
        erase(p.first,p.second);
        return n;
    }
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
#if defined(FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE)
    // Retail opt-in (w22 agent3019, row 0x006D1250 CTCWife::CheckAppearanceChanges): the STLport 4.6.1 _tree.h const member-template
    // find with its body visible (`return const_iterator(_M_find(__k));`) but called out of line, as retail does at all three
    // set<long> sites (0x006D134D / 0x006D1467 / 0x006D1734 -> 0x00616580, whose body is this find with _M_find expanded). The
    // visible body tells VC7.1 the key is only read (the caller forwards the stored key across the following ~CCharString call and
    // shares its frame word, 0x006D170F..0x006D171E); declaration-only find cannot do that, and the plain inline body is expanded at
    // the loop-2 and tattoo sites (c2 cost scores 68 / 60 < 74, while the LowerBound sites retail expands score 63 / 60, so no
    // /d2inlT<N> separates them). Inline-budget device per EXTRA_RULES (owner decision 2026-11-08, preference 3).
#if defined(_MSC_VER)
    template<class KT> __declspec(noinline) const_iterator find(const KT& key) const { return const_iterator(_M_find(key)); }
#else
    template<class KT> const_iterator find(const KT& key) const { return const_iterator(_M_find(key)); }
#endif
#elif defined(FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY)
    // Retail opt-in (chunk 1016, row 0x006D8790 CTCHeroExperience::AddSuccessfulStrikeOfType): owner decision D1 (2026-11-03) with the
    // body visible -- STLport 4.6.1 _tree.h `template <class _KT> const_iterator find(const _KT& __k) const { return
    // const_iterator(_M_find(__k)); }`. Retail calls the map<EDamageAttribute,float> instance out of line (0x006DAB90, the landed
    // ??$find@W4EDamageAttribute@@@ member template) but the TU sees its body, so the caller keeps the found node in ebx across the
    // next call and reads `[ebx+14h]` (0x006D87D5..0x006D87F7); the declaration-only template re-reads the result slot and
    // precomputes &second (125 v 124 bytes). Same body as FABLE_RETAIL_STLP_TREE_CONSTFIND, member-template spelling.
#if defined(FABLE_RETAIL_AUGMENTATION_PARTICLES) && defined(_MSC_VER)
    // Inline-budget: native 00769fe0 returns the original hidden iterator result.
    template<class KT> __declspec(noinline) const_iterator find(const KT& key) const { return const_iterator(_M_find(key)); }
#else
    template<class KT> const_iterator find(const KT& key) const { return const_iterator(_M_find(key)); }
#endif
#elif defined(FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (wf20, row 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks): the same STLport const find body, visible but out of
    // line as the retail TU calls it (map<unsigned long,long> 0x006699C0, hidden iterator result); seeing it writes nothing, VC7.1
    // hoists &CurrentBlock->GroupToIndexMap out of the group loop (0x00ADA084).
    __declspec(noinline) const_iterator find(const K& key) const { return const_iterator(_M_find(key)); }
#elif defined(FABLE_RETAIL_STLP_TREE_CONSTFIND)
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
#ifdef FABLE_RETAIL_STLP_MAP_REVERSE
    // Per-TU opt-in (wf20, row 0x00839100; needs FABLE_RETAIL_STLP_LIST_REVERSE for reverse_iterator): STLport 4.6 _tree.h
    // `const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }` / `rend() const { ..(begin()); }`.
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    const_reverse_iterator rbegin()const{return const_reverse_iterator(end());}
    const_reverse_iterator rend()const{return const_reverse_iterator(begin());}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    // Retail opt-in (chunk 778, row 0x00ACCBF0 COptimisedPrimitive::PerformSubContraction): STLport 4.6 _tree.h erase(iterator)
    // (rebalance, destroy, deallocate; out of line in retail: 0x00AD4CA0 map<unsigned long,CContractionReference>, 0x00AD3C30
    // multimap<float,CCountedPointer<CVertexPairContraction> >). Declaration only.
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_BODY
    // Nested opt-in (agentFAM12): STLport 4.6 _tree.h erase(iterator) with its body visible, expanded in the out-of-line
    // erase(first, last) of FABLE_RETAIL_STLP_TREE_ERASE_KEY (0x00638EE0 set<long>, 0x007775B0 map<long,CShopItem>, 0x007E99F0
    // map<CThing*,float>, 0x009D7760 set<CBankFileAsyncData*>, ..: _Rebalance_for_erase 0x0042A34E, _Destroy, deallocate, --count).
    void erase(iterator position) {
        _Link_type y=(_Link_type)_Rb_global_inst::_Rebalance_for_erase(position._M_node,this->_M_header._M_data->_M_parent,
            this->_M_header._M_data->_M_left,this->_M_header._M_data->_M_right);
        _Destroy(&y->_M_value_field);
        this->_M_header.deallocate(y,1);
        --_M_node_count;
    }
#else
    void erase(iterator position);
#endif
#endif
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY) && !defined(FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE)
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
#if defined(FABLE_RETAIL_STLP_POST_DEED_RANGE_OUTLINE) && !defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
// Inline-budget: key erase calls both equal_range and range erase in its native 83-byte owner.
    __declspec(noinline)
#endif
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
    // Nested opt-in (chunk 879, row 0x00502E90 CWorldMap::PopulateRegionWithVillagers; re-based by agent wfE10): that TU expands
    // erase(const K&) in place (0x005034BE, its distance count dead) and CALLS both halves out of line (equal_range 0x00514730, pair
    // through a hidden pointer; erase(first, last) 0x0051AF80, iterators by value): the same STLport bodies, kept out of line.
    __declspec(noinline)
#endif
    pair<iterator,iterator> equal_range(const K& key) { return pair<iterator,iterator>(lower_bound(key),upper_bound(key)); }
#ifdef FABLE_RETAIL_STLP_SET_UPPER_BOUND
    // Retail opt-in (chunk F2, row 0x009D5BF0 CBankFileAsync::HandleFailedAllocation): STLport 4.6 _tree.h const upper_bound (the
    // same walk as the non-const one, returning const_iterator), which _set.h's upper_bound forwards to; inline in retail.
    const_iterator upper_bound(const K& key) const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return const_iterator(y);
    }
#endif
#if defined(FABLE_RETAIL_STLP_POST_DEED_RANGE_OUTLINE) && !defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
// Inline-budget: key erase calls both equal_range and range erase in its native 83-byte owner.
    __declspec(noinline)
#endif
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
    __declspec(noinline)
#endif
    void erase(iterator first,iterator last) {
        if(first==begin()&&last==end()) clear();
        else while(first!=last) erase(first++);
    }
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
    FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
    // Nested opt-in (agent wfE10-5, row 0x00502E90): STLport 4.6 _tree.c's own text, `size_type __n = distance(__p.first, __p.second);`
    // (needs FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE's distance / __distance). VC7.1 then drops the dead count loop as retail does
    // (0x005034BE: equal_range 0x00514730 straight into erase(first, last) 0x0051AF80, no node walk); the hand loop below is kept.
    size_t erase(const K& key) {
        pair<iterator,iterator> p=equal_range(key);
        size_t n=distance(p.first,p.second);
        erase(p.first,p.second);
        return n;
    }
#else
    size_t erase(const K& key) {
        pair<iterator,iterator> p=equal_range(key);
        size_t n=0;
        for(iterator i=p.first;i!=p.second;++i) ++n;
        erase(p.first,p.second);
        return n;
    }
#endif
#endif
};
#ifndef FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
// (NF178b opt-in, 2026-11-20: FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY leaves _M_erase declared but not defined in the TU, so the
// TU references the owner row's COMDAT instead of emitting its own pick-any copy. Users: the 34 cut-scene cast-map rows, whose
// map<CCharString, CScriptGameResourceObjectScriptedThingBase> _M_erase is row 0x00CD344A; retail calls it out of line there.)
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
// Original STLport4.6.2 _tree.c lines633..646, actual reference-returning child accessors.
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Rb_tree_node<V>* x) {
    while(x!=0) {
        _M_erase(_S_right(x));
        _Link_type y=_S_left(x);
        std::_Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x,1);
        x=y;
    }
}
#else
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
#endif
#ifdef FABLE_RETAIL_STLP_TREE_MCOPY_BODY
// NF61 opt-in: STLport 4.6.2 stl/_tree.c _M_copy (structural copy; x and p must be non-null). Exceptions are off in retail
// (_STLP_TRY / _STLP_UNWIND(_M_erase(top)) expand to nothing: no unwind state in any retail copy).
template<class K,class V,class KeyOfValue,class Compare,class A>
_Rb_tree_node<V>* _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_copy(_Rb_tree_node<V>* x,_Rb_tree_node<V>* p) {
    _Link_type top=_M_clone_node(x);
    top->_M_parent=p;
    if(x->_M_right)
        top->_M_right=_M_copy(_S_right(x),top);
    p=top;
    x=_S_left(x);
    while(x!=0) {
        _Link_type y=_M_clone_node(x);
        p->_M_left=y;
        y->_M_parent=p;
        if(x->_M_right)
            y->_M_right=_M_copy(_S_right(x),y);
        p=y;
        x=_S_left(x);
    }
    return top;
}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
// NF64 opt-in: STLport 4.6.2 stl/_tree.c `operator=(const _Rb_tree&)`: unless self-assigned, clear() (retail calls each instance's
// clear COMDAT), zero the count, copy the comparator, then either reset the header to an empty tree or _M_copy the source root under
// the header and re-derive leftmost / rightmost (_S_minimum / _S_maximum walks, written out: fable_map.h has no node-base helpers).
template<class K,class V,class KeyOfValue,class Compare,class A>
_Rb_tree<K,V,KeyOfValue,Compare,A>& _Rb_tree<K,V,KeyOfValue,Compare,A>::operator=(const _Rb_tree<K,V,KeyOfValue,Compare,A>& x) {
    if(this!=&x) {
        clear();
        _M_node_count=0;
        _M_key_compare=x._M_key_compare;
        if(x._M_root()==0) {
            _M_root()=0;
            _M_leftmost()=this->_M_header._M_data;
            _M_rightmost()=this->_M_header._M_data;
        }
        else {
            _M_root()=_M_copy(x._M_root(),this->_M_header._M_data);
            _Rb_tree_node_base* n=_M_root();
            while(n->_M_left!=0) n=n->_M_left;
            _M_leftmost()=static_cast<_Link_type>(n);
            n=_M_root();
            while(n->_M_right!=0) n=n->_M_right;
            _M_rightmost()=static_cast<_Link_type>(n);
            _M_node_count=x._M_node_count;
        }
    }
    return *this;
}
#endif
#ifdef FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
// STLport 4.6.2 stl/_tree.c 345-383, including complete node construction.
template<class K,class V,class KeyOfValue,class Compare,class A>
_Rb_tree_iterator<V,_Nonconst_traits<V> > _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_insert(_Rb_tree_node_base* raw_x,_Rb_tree_node_base* raw_y,const V& value,_Rb_tree_node_base* raw_w) {
    _Link_type w=(_Link_type)raw_w;
    _Link_type x=(_Link_type)raw_x;
    _Link_type y=(_Link_type)raw_y;
    _Link_type z;
    if(y==this->_M_header._M_data || (w==0 && (x!=0 || _M_key_compare(KeyOfValue()(value),_S_key(y))))) {
        z=_M_create_node(value);
        _S_left(y)=z;
        if(y==this->_M_header._M_data) {_M_root()=z;_M_rightmost()=z;}
        else if(y==_M_leftmost()) _M_leftmost()=z;
    } else {
        z=_M_create_node(value);
        _S_right(y)=z;
        if(y==_M_rightmost()) _M_rightmost()=z;
    }
    _S_parent(z)=y;
    _S_left(z)=0;
    _S_right(z)=0;
    _Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(z);
}
#endif
#ifdef FABLE_RETAIL_STLP_MAP
#ifndef FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY
// (NF184 opt-in, 2026-11-20: FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_DECL_ONLY leaves insert_unique(const V&) declared but not defined in
// the TU, so the TU calls the owner row's COMDAT instead of emitting its own pick-any copy. User: row 0x009A7D40
// CBankFileManager::GetRetailBankFilePath, whose set<CWideString> insert_unique is row 0x009AAEB0; fable_string.h declares no
// CWideString::operator< (0x0099AF70), so a copy built here would compare the converted text pointers and differ from the owner.)
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
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
        else j._M_node=_Rb_global_inst::_M_decrement(j._M_node);
#else
        else --j;
#endif
    if(_M_key_compare(_S_key(j._M_node),KeyOfValue()(v)))
        return pair<iterator,bool>(_M_insert(x,y,v),true);
    return pair<iterator,bool>(j,false);
}
#endif
#endif
#if defined(FABLE_RETAIL_STLP_MAP) && defined(FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY)
// Per-TU opt-in (row 0x00ACA830 COptimisedPrimitive::Initialise): the STLport 4.6 _tree.c insert_equal(const V&) body, visible as
// _tree.c is in every retail TU; retail expands it twice there (0x00ACBD94, 0x00ACC109: the key walk, then a call to _M_insert
// 0x00AD4540 with w = 0) while the same-TU UpdateCost 0x00AC8A60 calls the COMDAT 0x00AD54B0.
template<class K,class V,class KeyOfValue,class Compare,class A>
_Rb_tree_iterator<V,_Nonconst_traits<V> > _Rb_tree<K,V,KeyOfValue,Compare,A>::insert_equal(const V& v) {
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    while(x!=0) {
        y=x;
        x=_M_key_compare(KeyOfValue()(v),_S_key(x))?_S_left(x):_S_right(x);
    }
    return _M_insert(x,y,v);
}
#endif
#if defined(FABLE_RETAIL_STLP_MAP) && defined(FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY)
// Opt-in (chunk 4007, row 0x0061B610 CTCInventoryQuests::ConstructQuestList): the STLport 4.6 _tree.c hinted
// insert_unique(iterator, const value_type&) body, visible in every retail TU that includes <map> like insert_unique(const V&)
// above (text = the landed row 0x0042D2C6, the size-TU COMDAT of the map<unsigned long, CUIState> instance the row calls).
// With the body visible VC7.1 knows the callee only writes its hidden iterator result: the result temporary of the expanded
// map::insert(iterator, const value_type&) shares state_index's frame word (0x0061DED6 `lea edx,[esp+34h]` = [esp+2Ch] + two
// pushes) instead of taking a word of its own. Out of line (never expanded in the row).
template<class K,class V,class KeyOfValue,class Compare,class A>
_Rb_tree_iterator<V,_Nonconst_traits<V> > _Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(iterator position,const V& v) {
    if(position._M_node==this->_M_header._M_data->_M_left) {
        if(size()<=0)
            return insert_unique(v).first;
        if(_M_key_compare(KeyOfValue()(v),_S_key(position._M_node)))
            return _M_insert(position._M_node,position._M_node,v);
        else {
            bool comp_pos_v=_M_key_compare(_S_key(position._M_node),KeyOfValue()(v));
            if(comp_pos_v==false)
                return position;
            iterator after=position;
            ++after;
            if(after._M_node==this->_M_header._M_data)
                return _M_insert(0,position._M_node,v,position._M_node);
            if(_M_key_compare(KeyOfValue()(v),_S_key(after._M_node))) {
                if(_S_right(position._M_node)==0)
                    return _M_insert(0,position._M_node,v,position._M_node);
                else
                    return _M_insert(after._M_node,after._M_node,v);
            }
            else
                return insert_unique(v).first;
        }
    }
    else if(position._M_node==this->_M_header._M_data) {
        if(_M_key_compare(_S_key(_M_rightmost()),KeyOfValue()(v)))
            return _M_insert(0,_M_rightmost(),v,position._M_node);
        else
            return insert_unique(v).first;
    }
    else {
        iterator before=position;
        --before;
        bool comp_v_pos=_M_key_compare(KeyOfValue()(v),_S_key(position._M_node));
        if(comp_v_pos && _M_key_compare(_S_key(before._M_node),KeyOfValue()(v))) {
            if(_S_right(before._M_node)==0)
                return _M_insert(0,before._M_node,v,before._M_node);
            else
                return _M_insert(position._M_node,position._M_node,v);
        }
        else {
            iterator after=position;
            ++after;
            bool comp_pos_v=!comp_v_pos;
            if(!comp_v_pos)
                comp_pos_v=_M_key_compare(_S_key(position._M_node),KeyOfValue()(v));
            if((!comp_v_pos) && comp_pos_v &&
               (after._M_node==this->_M_header._M_data || _M_key_compare(KeyOfValue()(v),_S_key(after._M_node)))) {
                if(_S_right(position._M_node)==0)
                    return _M_insert(0,position._M_node,v,position._M_node);
                else
                    return _M_insert(after._M_node,after._M_node,v);
            }
            else {
                if(comp_v_pos==comp_pos_v)
                    return position;
                else
                    return insert_unique(v).first;
            }
        }
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
    typedef typename _Rep_type::iterator iterator;
#ifdef FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR
    // Per-TU inline opt-in (acctC agentC4, row 0x005FF190 CTCInventoryMap::CreateMapMarkers): STLport's map destructor (the tree's
    // `clear()` then the header node freed) written out and forced: retail expands the map<long, pair<long,long> > destructor at the
    // end of that row (0x00600136..0x0060019D: _M_erase 0x00613CE0 with one recursion level in place, two frees); VC7.1 never even
    // scores the implicit destructor there (no plain-inline decision in the c2 trace), so only a forced explicit one expands.
    FABLE_FORCEINLINE ~map() {}
#endif
#ifdef FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
    // Per-TU opt-in (chunk F1, row 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets; like FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE):
    // STLport's map copy constructor is one inline body, which VC7.1 called as a COMDAT in that TU: map<unsigned long, long>::map(const
    // map&) 0x00A96E40 (landed; call at 0x00AD97FB with ecx = &finish->GroupToIndexMap). Declaration only here.
    map(const map& x);
#endif
#ifdef FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY
    // NF78 opt-in (round 7, row 0x004F97B3): STLport 4.6.2 _map.h `map(const _Self& __x) : _M_t(__x._M_t) {}` (needs
    // FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY); the map<EDamageAttribute,float> COMDAT 0x004F97B3 that the landed 0x006DCB20 and 0x004F9784 call.
    map(const map& x):_M_t(x._M_t){}
#endif
#ifdef FABLE_RETAIL_STLP_MAP
    // Retail branch (pass 12 header wave, 0x00E0A820): STLport 4.6 _map.h declares the no-argument constructor separately
    // (`map() : _M_t(_Compare(), allocator_type()) {}` and `explicit map(const _Compare&, const allocator_type& = ...)`), so a
    // default-constructed local passes no argument temporaries: retail calls ??0?$map@...@QAE@XZ (0x00CDBF70) with only `this`.
#ifdef FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
    // Actual original nonvirtual destructor COMDAT00CDBFB0, called by00D29880.
    ~map();
#endif
#if defined(FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE) && defined(_MSC_VER)
    // NF61 opt-in (round 7, row 0x005907AB CTCInventory::ConstructItemCategoryListPC): the same STLport 4.6 _map.h no-argument
    // constructor, visible but kept out of line, as that size TU calls the map<long,long> COMDAT 0x004DF86C (`lea ecx,[local] ; call`;
    // the landed 0x004E0860 binds ??0?$map@JJ...@QAE@XZ there).
    __declspec(noinline)
#endif
    map():_M_t(Compare(),A()){}
    explicit map(const Compare& c,const A& a=A()):_M_t(c,a){}
#else
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
#endif
#ifdef FABLE_RETAIL_STLP_MAP_FORCEINLINE_BEGIN
    // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): PLAY_SOUND expands map::begin and the tree
    // begin under it at 0x00426AF8 (`mov eax,[edi] ; mov ecx,[eax+8]`) past VC7.1's caller-size inline budget.
    FABLE_FORCEINLINE
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
#ifdef FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
    // Retail opt-in (Pass22,00D29880): actual map index COMDAT00CD3D2E
    // is called; its complete STLport instantiation is external to this TU.
    V& operator[](const K&);
#else
    V& operator[](const K& k) {
        iterator i=lower_bound(k);
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
        if(i==end() || key_comp()(k,(*i).first)) i=insert(i,value_type(k,__default_constructed((V*)0)));
#else
        if(i==end() || key_comp()(k,(*i).first)) i=insert(i,value_type(k,V()));
#endif
        return (*i).second;
    }
#endif
#else
    V& operator[](const K&);
#endif
    size_t size() const { return _M_t.size(); }
#ifdef FABLE_RETAIL_STLP_MAP_EMPTY
    // Per-TU opt-in (chunk 1014, row 0x007EA2F0 via CBulletTimeManager::IsBulletTimeActive): STLport 4.6 _map.h
    // `bool empty() const { return _M_t.empty(); }` (the node count +4 tested in place).
    bool empty() const { return _M_t.empty(); }
#endif
#ifdef FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY
    // NF178 opt-in (2026-11-20, row 0x00E3E720 CV_BordelloScript::PlayCutscene): retail calls the map<CCharString, CCharString>
    // clear COMDAT 0x009AACE0 out of line there (`lea ecx,[csargs] ; call`), as landed 0x00D7C470 binds it. Declaration only.
    void clear();
#else
    void clear(){_M_t.clear();}
#endif
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
#ifdef FABLE_RETAIL_EXPERIENCE_ORB_CREATE_PASS29
    const_iterator lower_bound(const K& key)const{return _M_t.lower_bound(key);}
#endif
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
#ifdef FABLE_RETAIL_STLP_MAP_REVERSE
    // (same opt-in) STLport 4.6 _map.h `const_reverse_iterator rbegin() const { return _M_t.rbegin(); }` / rend.
    typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
    const_reverse_iterator rbegin()const{return _M_t.rbegin();}
    const_reverse_iterator rend()const{return _M_t.rend();}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    void erase(iterator position){_M_t.erase(position);}
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE_KEY
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS) && defined(_MSC_VER)
    FABLE_FORCEINLINE
#endif
    size_t erase(const K& key){return _M_t.erase(key);}
#endif
#if defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY_DECL_ONLY) && !defined(FABLE_RETAIL_STLP_TREE_ERASE_KEY) && !defined(FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE)
    // NF210 opt-in (row 0x006ED510): STLport 4.6 _map.h `size_type erase(const key_type& __x) { return _M_t.erase(__x); }` over the
    // declared-only tree erase above (inline wrapper, as in _map.h).
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
#ifdef FABLE_RETAIL_STLP_SET_RANGE
    // Retail opt-in (chunk 1018, row 0x00465C54 CCombatAbilityBlockDefBase::Transfer): STLport 4.6 _set.h
    // `set(_InputIterator __first, _InputIterator __last) : _M_t(_Compare(), allocator_type()) { _M_t.insert_unique(__first, __last); }`
    // (the tree constructor stays the out-of-line 0x00451ADD there) and `operator=` (`_M_t = __x._M_t`, the tree's out-of-line
    // assignment 0x00461BF0).
    template<class II> set(II first,II last):_M_t(Compare(),A()){_M_t.insert_unique(first,last);}
    set& operator=(const set& x){_M_t=x._M_t;return *this;}
    template<class II> void insert(II first,II last){_M_t.insert_unique(first,last);}
#endif
#ifdef FABLE_RETAIL_STLP_SET_COPY
    // Retail branch (chunk 777, row 0x00AC92C0 COptimisedPrimitive::ComputeContraction): STLport 4.6 _set.h `set(const _Self& x)
    // : _M_t(x._M_t) {}` with the _Rb_tree copy expanded is too large for VC7.1 to inline: retail calls the out-of-line copy
    // ??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z (0x00AD3A10, landed) for each CIndexList copy.
    set(const set& x);
#endif
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
    size_t erase(const K& value){return _M_t.erase(value);}
#ifdef FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
    // (same opt-in, wf4 row 0x004B3CE0) STLport 4.6 _set.h lower_bound / upper_bound / equal_range (const, forwarding to the tree) and
    // erase(iterator, iterator) (`_M_t.erase((_Rep_iterator&)__first, (_Rep_iterator&)__last)`).
    iterator lower_bound(const K& x) const { return _M_t.lower_bound(x); }
    iterator upper_bound(const K& x) const { return _M_t.upper_bound(x); }
    pair<iterator,iterator> equal_range(const K& x) const { return _M_t.equal_range(x); }
    void erase(iterator first,iterator last){typedef typename _Rep_type::iterator RepIterator;_M_t.erase((RepIterator&)first,(RepIterator&)last);}
#endif
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
#ifdef FABLE_RETAIL_STLP_SET_UPPER_BOUND
    // Retail opt-in (chunk F2, row 0x009D5BF0): STLport 4.6 _set.h `iterator upper_bound(const key_type& __x) const { return
    // _M_t.upper_bound(__x); }`. Needs FABLE_RETAIL_STLP_TREE_ERASE_KEY (the tree's upper_bound block).
    iterator upper_bound(const K& x) const { return _M_t.upper_bound(x); }
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
#ifdef FABLE_RETAIL_STLP_MULTIMAP_PASS22_ATMOS_ITERATION
    // Retail opt-in (Codex pass 22): STLport 4.6 _multimap.h const iteration, 0x006B17BB..0x006B1862.
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator begin() const { return _M_t.begin(); }
#ifndef FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND
    const_iterator end() const { return _M_t.end(); }
#endif
#endif
    multimap():_M_t(Compare(),A()){}
    iterator insert(const value_type& x){return _M_t.insert_equal(x);}
#ifdef FABLE_RETAIL_STLP_MULTIMAP_EMPTY
    // NF67 opt-in (round 7, row 0x00869020 CHitParameters::SetAsModifiedByArmour): STLport 4.6 _multimap.h `bool empty() const
    // { return _M_t.empty(); }` (the node count tested in place, 0x00869071 `cmp [esp+30h],ebp`). Needs FABLE_RETAIL_STLP_SET_EMPTY (the
    // tree's empty). Body only, no layout change.
    bool empty() const { return _M_t.empty(); }
#endif
#ifdef FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
    // Per-TU opt-in (row 0x00ACA830): STLport 4.6 _multimap.h begin() / end() (inline; 0x00ACC24F walks VertexPairContractions
    // from the header's left link to the header).
    iterator begin(){return _M_t.begin();}
    iterator end(){return _M_t.end();}
#endif
    void clear(){_M_t.clear();}
#ifdef FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE
    // Retail opt-in (wf13, row 0x009CCEF0 CBankFile::FindIndicesByFilename): STLport 4.6 _map.h multimap const equal_range, forwarding
    // to the tree (pair with FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: upper bound called first, then lower bound).
    typedef typename _Rep_type::const_iterator const_iterator;
    pair<const_iterator,const_iterator> equal_range(const K& x) const { return _M_t.equal_range(x); }
#endif
#ifdef FABLE_RETAIL_STLP_TREE_ERASE
    void erase(iterator position){_M_t.erase(position);}
#endif
#ifdef FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND
    // Retail opt-in (wf5, row 0x007F53B0 CTCSoundAtmosphere::FrameUpdate): STLport 4.6 _map.h multimap
    // `const_iterator find(const key_type& __x) const { return _M_t.find(__x); }` and `const_iterator end() const { return _M_t.end(); }`
    // (both inline; retail calls the tree's _M_find<CCharString> 0x0042B467 on CSoundThemeDef::AtmosGroupMap and compares the node
    // with the header word). Pair with FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY (or _CONSTFIND) for the tree's const find body.
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator find(const K& x)const{return _M_t.find(x);}
    const_iterator end()const{return _M_t.end();}
#endif
};
}
FABLE_STATIC_ASSERT_AT(699, sizeof(std::map<unsigned long,long>)==12);
#if defined(FABLE_RETAIL_STLP_TREE_ULONG_ULONG_SIZE_OWNER_PASS26) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. The owners 0042FE07 and 00430046 use optimize("s", on).
// These are the original STLport 4.6 bodies for the CUIDef alias map's tree. The reset
// returns to optimize("", on), so any caller's own optimization pragmas follow this include.
#pragma optimize("s", on)
namespace std {
typedef pair<const unsigned long, unsigned long> CUIAliasEntry;
typedef _Rb_tree<unsigned long, CUIAliasEntry, _Select1st<CUIAliasEntry>, less<unsigned long>, allocator<CUIAliasEntry> > CUIAliasTree;

template<>
inline void CUIAliasTree::_M_erase(_Rb_tree_node<CUIAliasEntry>* node)
{
    while (node != 0) {
        _M_erase(static_cast<_Rb_tree_node<CUIAliasEntry>*>(node->_M_right));
        _Rb_tree_node<CUIAliasEntry>* left = static_cast<_Rb_tree_node<CUIAliasEntry>*>(node->_M_left);
        _Destroy(&node->_M_value_field);
        this->_M_header.deallocate(node, 1);
        node = left;
    }
}

template<>
inline void CUIAliasTree::clear()
{
    if (_M_node_count != 0) {
        _M_erase(static_cast<_Rb_tree_node<CUIAliasEntry>*>(this->_M_header._M_data->_M_parent));
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
        _M_node_count = 0;
    }
}
}
#pragma optimize("", on)
#endif // FABLE_RETAIL_STLP_TREE_ULONG_ULONG_SIZE_OWNER_PASS26
#ifdef FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY
// NF184 opt-in (2026-11-20, q4391; row 0x009A80B0 CBankFileManager::BankExists). Fidelity: scaffolding, not retail. That TU's
// out-of-line _M_find copies call std::less<CCharString>::operator() (Ego_r.pdb ??R?$less@VCCharString@@@std@@QBE_NABVCCharString@@0@Z),
// so the TU emits it as a pick-any COMDAT. Retail kept the 41-byte copy from a size-optimised TU, row 0x0042AAB3. The plain /O2 copy
// is 43 bytes: two `ret 8` exits where the owner has `jmp` exits. The advisor pick-any rule (EXTRA_RULES 2026-11-20) needs the owner's body.
// Declaration-only does not work: BankExists must see the body. With it VC7.1 knows the finds write no memory and keeps the bank-list
// head in ebx across them (0x009A80DF). Declared only, it reloads the head: DIFFER(140v145).
// So this explicit specialization compiles the same STLport body (`return x<y;`) under optimize("s"), as the owner's TU did. Pair it
// with FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS, which expands CCharString::operator< as the owner does. The copy then grades
// RELOCATION_MATCH at 0x0042AAB3. Needs fable_string.h first, and no use of the operator before this point.
// Owner check (landing nf184): the owner row sets the same optimize pragmas itself. `#pragma optimize("", on)` below returns to the
// command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this opt-in must not carry a row-head optimize pragma before its include of this header (nf189).
#pragma optimize("s", on)
namespace std { template<> inline bool less<CCharString>::operator()(const CCharString& x,const CCharString& y) const { return x<y; } }
#pragma optimize("", on)
#endif
#if defined(FABLE_RETAIL_STLP_ULONG_LONG_MFIND_SIZE_PASS28) && defined(_MSC_VER)
// Fidelity scaffolding: use the size setting of the retail unsigned-long lookup owner 0x005D83B3.
// The genuine STLport body remains visible to the speed-optimized Divine Wrath caller.
#pragma optimize("s", on)
namespace std {
typedef pair<const unsigned long,long> EffectStringMapValue;
typedef _Rb_tree<unsigned long,EffectStringMapValue,_Select1st<EffectStringMapValue>,
                 less<unsigned long>,allocator<EffectStringMapValue> > EffectStringMapTree;
template<> template<> inline _Rb_tree_node<EffectStringMapValue>*
EffectStringMapTree::_M_find<unsigned long>(const unsigned long& key) const
{
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    while (x!=0) {
        if (!_M_key_compare(_S_key(x),key)) { y=x; x=_S_left(x); }
        else x=_S_right(x);
    }
    if (y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))
        y=this->_M_header._M_data;
    return y;
}
}
#pragma optimize("", on)
#endif
#ifdef FABLE_RETAIL_ARROW_TREE_FIND_SIZE_OWNER_PASS26
// Fidelity: scaffolding, not retail. Owner 005CC04A sets optimize("s",on).
// The reset optimize("",on) requires that row-level pragmas follow this header.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
typedef pair<const EObjectAugmentationType,long> ArrowTrailValue;
typedef _Rb_tree<EObjectAugmentationType,ArrowTrailValue,_Select1st<ArrowTrailValue>,
                 less<EObjectAugmentationType>,allocator<ArrowTrailValue> > ArrowTrailTree;
template<> template<> inline _Rb_tree_node<ArrowTrailValue>*
ArrowTrailTree::_M_find<EObjectAugmentationType>(const EObjectAugmentationType& key) const
{
    _Link_type y=this->_M_header._M_data;
    _Link_type x=_M_root();
    while (x) {
        if (!_M_key_compare(_S_key(x),key)) { y=x; x=_S_left(x); }
        else x=_S_right(x);
    }
    if (y==this->_M_header._M_data || _M_key_compare(key,_S_key(y))) y=this->_M_header._M_data;
    return y;
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#endif
#ifdef FABLE_RETAIL_STLP_TREE_FIND_LONG_LONG_SIZE_BODY
// NF189 opt-in (2026-11-20, q4385; row 0x006D0630 CTCWife::ReceivedNiceGiftIsOpinionReactionHappening). Fidelity: scaffolding, not retail.
// That /O2 TU calls the map<long,long> tree's member template find<long> out of line (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE
// with FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), so it emits find<long> as a pick-any COMDAT. Retail kept the 20-byte copy from a
// size-optimised TU, row 0x005937E1; the plain /O2 copy is 21 bytes (`mov eax,[esp+8] ; push eax` where the owner has `push [esp+8]`).
// The advisor pick-any rule (EXTRA_RULES 2026-11-20) needs the owner's body, so this explicit specialization compiles the same STLport
// body (_tree.h 457-467 `return iterator(_M_find(__k));`) under optimize("s"), as the owner's TU did; noinline, as retail calls it.
// Needs FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE (or _MEMBER_TEMPLATE) and no use of find<long> before this point.
// Owner check: the owner row 0x005937E1 sets the same optimize pragma itself (`#pragma optimize("s", on)`). `#pragma optimize("", on)`
// below returns to the command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this opt-in must not also carry
// a row-head optimize pragma before its include of this header (one after the include is not reset; advisor, nf189).
#pragma optimize("s", on)
namespace std {
template<> template<> inline __declspec(noinline)
_Rb_tree<long,pair<const long,long>,_Select1st<pair<const long,long> >,less<long>,allocator<pair<const long,long> > >::iterator
_Rb_tree<long,pair<const long,long>,_Select1st<pair<const long,long> >,less<long>,allocator<pair<const long,long> > >::find<long>(const long& key)
{ return iterator(_M_find(key)); }
}
#pragma optimize("", on)
#endif
#ifdef FABLE_RETAIL_STLP_MAP_FIND_ULONG_LONG_SIZE_BODY
// NF189 opt-in (2026-11-20, q4386; row 0x005D66BA CTCInventoryStats::SetCreatureAsKilled). Fidelity: scaffolding, not retail.
// That /O2 TU expands map<unsigned long,long>::find (retail calls the tree's _M_find 0x005D83B3 straight from the row) and emits the
// inline as a pick-any COMDAT. Retail kept the 20-byte out-of-line copy from the size-optimised CTCInventoryStats TU, row 0x005D8548
// (Leaf ?find@?$map@KJ...); the plain /O2 copy is 22 bytes. The advisor pick-any rule (EXTRA_RULES 2026-11-20) needs the owner's body,
// so this explicit specialization compiles the same STLport 4.6 _map.h body (`return _M_t.find(__x);`) under optimize("s"), as the
// owner's TU did; callers still expand it. Owner check: the owner row 0x005D8548 sets `#pragma optimize("s", on)` itself.
// `#pragma optimize("", on)` below returns to the command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this
// opt-in must not carry a row-head optimize pragma before its include of this header (nf189). Needs no use of map<unsigned long,long>::find before this point.
#pragma optimize("s", on)
namespace std {
template<> inline map<unsigned long,long,less<unsigned long>,allocator<pair<const unsigned long,long> > >::iterator
map<unsigned long,long,less<unsigned long>,allocator<pair<const unsigned long,long> > >::find(const unsigned long& key)
{ return _M_t.find(key); }
}
#pragma optimize("", on)
#endif

#if defined(FABLE_RETAIL_STLP_CHAR_LONG_ERASE_OWNER_PASS26) && defined(_MSC_VER)
// The actual0043020a owner is the standard recursive body compiled for size.
#pragma optimize("s", on)
namespace std {
template<> FABLE_FORCEINLINE void _Destroy(pair<const CCharString,long>* p) { p->~pair(); }
template<> inline void _Rb_tree<CCharString,pair<const CCharString,long>,_Select1st<pair<const CCharString,long> >,less<CCharString>,allocator<pair<const CCharString,long> > >::_M_erase(_Rb_tree_node<pair<const CCharString,long> >* x)
{
    while(x!=0) {
        _M_erase(static_cast<_Rb_tree_node<pair<const CCharString,long> >*>(x->_M_right));
        _Rb_tree_node<pair<const CCharString,long> >* y=static_cast<_Rb_tree_node<pair<const CCharString,long> >*>(x->_M_left);
        _Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x,1);
        x=y;
    }
}
template<> inline void _Rb_tree<CCharString,pair<const CCharString,long>,_Select1st<pair<const CCharString,long> >,less<CCharString>,allocator<pair<const CCharString,long> > >::clear(){
 if(_M_node_count!=0){
  _M_erase(static_cast<_Rb_tree_node<pair<const CCharString,long> >*>(this->_M_header._M_data->_M_parent));
  this->_M_header._M_data->_M_left=this->_M_header._M_data;
  this->_M_header._M_data->_M_parent=0;
  this->_M_header._M_data->_M_right=this->_M_header._M_data;
  _M_node_count=0;
 }
}
template<> inline _Rb_tree<CCharString,pair<const CCharString,long>,_Select1st<pair<const CCharString,long> >,less<CCharString>,allocator<pair<const CCharString,long> > >::~_Rb_tree(){clear();}
}
#pragma optimize("", on)
#endif

#if defined(FABLE_RETAIL_STLP_CUISTATE_MAP_OWNERS_PASS28) && defined(_MSC_VER)
// ScrollBar::ProcessEvent 0x00559A90 uses a visible map index body so the const
// key does not escape. Its distinct retail owners are the 0x0042D5B1 map index,
// 0x0042AD6C tree lower-bound walk and 0x0042AA96 pair constructor. Rebuild the
// original STLport declarations and bodies with each owner's size settings.
#include "engine/NUISystem_CUIState.h"
#pragma optimize("s", on)
namespace std {
typedef pair<const unsigned long, NUISystem::CUIState> CUIStatePair;
typedef allocator<CUIStatePair> CUIStateAllocator;
typedef _Rb_tree<unsigned long, CUIStatePair, _Select1st<CUIStatePair>, less<unsigned long>, CUIStateAllocator> CUIStateTree;
typedef map<unsigned long, NUISystem::CUIState> CUIStateMap;

template<> __declspec(noinline) inline
_Rb_tree_node_base* CUIStateTree::_M_lower_bound(const unsigned long& __k) const
{
    _Link_type y = this->_M_header._M_data;
    _Link_type x = _M_root();
    while (x != 0) {
        if (!_M_key_compare(_S_key(x), __k)) {
            y = x;
            x = _S_left(x);
        } else {
            x = _S_right(x);
        }
    }
    return y;
}

template<> inline
CUIStatePair::pair(const unsigned long& a, const NUISystem::CUIState& b) : first(a), second(b) {}

template<>
CUIStateMap::iterator CUIStateMap::insert(iterator, const value_type&);

#pragma optimize("y", off)
template<>
#ifdef FABLE_RETAIL_STLP_CUISTATE_MAP_GETSIZE_OUTOFLINE
// GetSize calls the native 0042d5b1 owner for each state lookup.
FABLE_STLP_NOINLINE
#endif
inline NUISystem::CUIState& CUIStateMap::operator[](const unsigned long& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first)) {
        NUISystem::CUIState __default;
        i = insert(i, value_type(k, __default));
    }
    return (*i).second;
}
}
#pragma optimize("", on)
#endif
#endif
