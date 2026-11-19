// std::map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[](const NUISystem::ETableSprites& __k) -- the value for k, inserting an empty counted pointer when missing
// Grade: DIFFER(155v150) -- residue: retail schedules the pair temporary's two zero stores after the three argument pushes and releases the counted pointer with `dec dword ptr [esi] ; jne` (flags of the decrement); VC7.1 here stores the zeros before the pushes and re-reads the count (`dec [eax] ; cmp dword ptr [esi],0`); 8 dtor / mode variants tried (member vs local block pointer, signed count, split ifs, /Os with and without __forceinline helpers)
// Leaf: ??A?$map@W4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@U?$less@W4ETableSprites@NUISystem@@@std@@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@5@@std@@QAEAAV?$CCountedPointer@VCComponent@NUISystem@@@@ABW4ETableSprites@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00554030, 150 bytes, thiscall, `ret 4`.
// Identity (proven): called four times by NUISystem::CTable::GetDataFromDef 0x00551340 (Ego_r.pdb ?GetDataFromDef@CTable@NUISystem@@QAEXXZ); Ego_r.pdb CTable's only map is Sprites-map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >; signed key compare (`jl`), the counted pointer value at node +0x14.
// Callees: _Rb_tree<...>::insert_unique(iterator, const value_type&) 0x00553C50 (the iterator through a hidden pointer),
//   the count block's deleter (+4, object in ecx), operator delete 0x00BFE9BC.
// Shape (family M130, representative 0x00554030): STLport 4.6 `iterator __i = lower_bound(__k); if (__i == end() ||
//   key_comp()(__k, (*__i).first)) __i = insert(__i, value_type(__k, _Tp())); return (*__i).second;` -- the hint iterator
//   copy-constructed in its argument slot, the result homed in the dead k slot, then the pair temporary's CCountedPointer
//   destructor (`if (--refs == 0) { destroy(object); delete block; }`; the `_Tp()` temporary folds away).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace NUISystem { enum ETableSprites { TABLE_SPRITES_NONE = 0 }; class CComponent; }
class CCPPointerInfo { public:
    unsigned long refs;                                     // +0x0
    void (__fastcall* destroy)(void*);                      // +0x4
    void* object;                                           // +0x8
};
template <class T> class CCountedPointer {                  // the landed 0x00514790 model
public:
    CCountedPointer() : object(0), control(0) { }
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control) { if (control) ++control->refs; }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = control;
        if (info && --info->refs == 0) {
            info->destroy(info->object);
            operator delete(info);
        }
    }
    T* object;                                              // +0x0
    CCPPointerInfo* control;                                // +0x4
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    _Rb_tree_node_base* _M_node;
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Rb_tree_node<_Value>*)__x)->_M_value_field); }
    _Base_ptr _M_lower_bound(const _Key& __k) const
    {
        _Base_ptr __y = _M_header._M_data;
        _Base_ptr __x = _M_header._M_data->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator end() { return iterator(_M_header._M_data); }
    iterator insert_unique(iterator __position, const _Value& __x);   // 0x00553C50
    _STLP_alloc_proxy<_Base_ptr, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t._M_key_compare; }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
    _Rep_type _M_t;
};
template CCountedPointer<NUISystem::CComponent >& map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent > >::operator[](const NUISystem::ETableSprites&);
}
