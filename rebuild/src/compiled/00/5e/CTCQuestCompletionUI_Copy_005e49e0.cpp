// CTCQuestCompletionUI::Copy(const CTCBase& other) -- copy the quest-start statistic maps (renown, gold, experience) from another quest completion TC, then CTCBase::Copy
// Leaf: ?Copy@CTCQuestCompletionUI@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E49E0, 497 bytes, tail jump to CTCBase::Copy.
// Identity: slot 12 of the CTCQuestCompletionUI vtable 0x0125589C (RTTI `.?AVCTCQuestCompletionUI@@`); slot 12 of CTCBase is
//   0x006868D0, bound `?Copy@CTCBase@@UAEXABV1@@Z`, the tail-jump target here; Ego_r.pdb has
//   `?Copy@CTCQuestCompletionUI@@UAEXABVCTCBase@@@Z`. The catalog label `CTCQuestCompletionUI::vfunc_12` is a placeholder.
// Callees: std::map<long, unsigned long>::operator[] 0x005E9760 (landed with this spelling), std::_Rb_tree<long, pair<const long,
//   unsigned long>, ...>::_M_erase 0x005E8C00 (private, inside the inlined clear), CTCBase::Copy 0x006868D0 (tail jump).
// Layout: as ~CTCQuestCompletionUI 0x005E41E0: StartQuestExperience +0x84, StartQuestGold +0x90, StartQuestRenown +0x9C (Ego_r.pdb
//   0x8C/0x9C/0xAC with retail STLport sizes).
// Shape: the three clears first (Renown, Gold, Experience: retail order), then one copy loop per map in the same order through
//   `StartQuestX[it->first] = it->second` over the other object's map (const_iterator; STLport's _Rb_global<bool>::_M_increment
//   inlined; other's end() re-read each pass), then the base Copy as a tail call.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x0
    _Base_ptr _M_parent;                                    // +0x4
    _Base_ptr _M_left;                                      // +0x8
    _Base_ptr _M_right;                                     // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global { public:          // STLport 4.6 _tree.c
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Const_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x0
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;                                         // +0x0 (0xC bytes)
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    void clear() { _M_t.clear(); }
    _Tp& operator[](const _Key& __k);                       // out of line (0x005E9760 for map<long, unsigned long>)
};
} // namespace std

class CBaseClass { public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual void Copy(const CTCBase& other);                // 0x006868D0
};

class CTCQuestCompletionUI : public CTCBase { public:
    unsigned char                 _body0c[0x84 - 0x0c];     // +0x0C .. +0x84 (see ~CTCQuestCompletionUI 0x005E41E0 for the members)
    std::map<long, unsigned long> StartQuestExperience;     // +0x84
    std::map<long, unsigned long> StartQuestGold;           // +0x90
    std::map<long, unsigned long> StartQuestRenown;         // +0x9C
    unsigned char                 _bodya8[0xd8 - 0xa8];     // +0xA8 .. +0xD8
    virtual void Copy(const CTCBase& other);
};

void CTCQuestCompletionUI::Copy(const CTCBase& other)
{
    const CTCQuestCompletionUI& source = (const CTCQuestCompletionUI&)other;
    StartQuestRenown.clear();
    StartQuestGold.clear();
    StartQuestExperience.clear();
    std::map<long, unsigned long>::const_iterator it;
    for (it = source.StartQuestRenown.begin(); it != source.StartQuestRenown.end(); ++it)
        StartQuestRenown[it->first] = it->second;
    for (it = source.StartQuestGold.begin(); it != source.StartQuestGold.end(); ++it)
        StartQuestGold[it->first] = it->second;
    for (it = source.StartQuestExperience.begin(); it != source.StartQuestExperience.end(); ++it)
        StartQuestExperience[it->first] = it->second;
    CTCBase::Copy(other);
}