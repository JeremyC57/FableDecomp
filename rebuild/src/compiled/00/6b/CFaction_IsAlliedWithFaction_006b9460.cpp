// CFaction::IsAlliedWithFaction(const CFaction& faction) const -- a faction is allied with itself (same name) and with every faction named in its AlliedFactions set
// Leaf: ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B9460, 80 bytes (+ int3 padding), `ret 4` (thiscall).
// Callers: CTCEnemy::IsAlliedWithFaction 0x0076C7F0 (tail jump) and CTCEnemy::IsAllyOf 0x0076D460 (0x0076D4D1).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString equality, landed body
//   pasted under auto_inline(off)), set<CCharString>'s _Rb_tree::_M_find(const CCharString&) const @ 0x004B9370 (node in eax, `ret 4`; the landed F012
//   row's own symbol, through the real STLport set template).
// Identity: Ego_r.pdb ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z (its sibling IsEnemyOfFaction tests EnemyFactions).
// Layout: CFaction (Ego_r.pdb, no engine header) Name CCharString +0, AlliedFactions set<CCharString> +4, EnemyFactions
//   +0x10 -- the retail STLport set is 12 bytes (_M_header +0, _M_node_count +4, the empty less<> +8), so `end()` is the
//   header word at this+4.
// Shape: `return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;` -- the `||` value in a bool function
//   gives retail's full-width `mov eax,1` / `xor eax,eax` exits (two `if (..) return true;` give `mov al,1` + `setne`).
//   The inline CCharString operator== takes references (same block -> equal; both blocks non-null with equal lengths ->
//   `Compare(..) ? false : true` = `neg ; sbb al,al ; inc al`); STLport `set::count` compares `_M_find(k)` with the header node, which
//   reads the header word AFTER the out-of-line `_M_find` call (`find(..) != end()` evaluates end() first and hoists it).
// Label correction: the catalog `FactionMap_FindAndCompare` is descriptive.
class CCharStringData { public:                                // the shared string block
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    bool operator<(const CCharString& other) const;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                         // +0x04
    _Compare _M_key_compare;                            // +0x08 (empty)
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;          // out of line (_tree.c)
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {                                             // STLport 4.6 (SGI): 12 bytes
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) const { return _M_t.find(__x); }
    iterator end() const { return _M_t.end(); }
    // unique-key count: the found node against the header node (the header word is read after the _M_find call)
    unsigned int count(const _Key& __x) const { return _M_t._M_find(__x) == _M_t._M_header._M_data ? 0 : 1; }
};

} // namespace std

class CFaction {
public:
    CCharString    Name;                                // +0x00
    std::set<CCharString> AlliedFactions;               // +0x04 (STLport, 12 bytes)
    std::set<CCharString> EnemyFactions;                // +0x10
    bool IsAlliedWithFaction(const CFaction& faction) const;
};

bool CFaction::IsAlliedWithFaction(const CFaction& faction) const
{
    return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;
}