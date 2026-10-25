// std::_Rb_tree_iterator::operator--() -- set<unsigned long> iterator step back to the in-order predecessor (the header steps to the rightmost node) (STLport 4.6, _Nonconst_traits): 
// Leaf: ??F?$_Rb_tree_iterator@KU?$_Nonconst_traits@K@std@@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050E130, 79 bytes, thiscall on the iterator, returns it in eax.
// Identity (sweep2 1b, owner decision 1b): called from the tree's insert path 0x514C10 (std::map::std::map::find), 0xAD28E0 (_global::RedBlackTree_LowerBoundInsert); 2 call(s) up, 0x4FE7F0 CWorldMap::GetSoundThemesUsedInCurrentRegion, 0xA385A0 CCRCSymbolMap::AddSymbolDirty, 0xAD4810 std::insert_iterator::operator= binds this set<unsigned long> instantiation (reloc_bindings); Ego_r.pdb member type set<unsigned long>: CCRCSymbolMap::NamesUsed; CDebugManager::ErrorsOff; CDebugManager::LoggedErrorsOff (+2 more)
//   The catalog name CRetailTreeIterator::Decrement was a generated stand-in shared by every byte-identical body of this
//   shape; retail has no identical-code folding, so each address is one instantiation (one value type).
// Shape: STLport 4.6 `_Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }` with the template
//   static _Rb_global<bool>::_M_decrement inlined. The iterator is the tree's own (non-const) iterator: its insert path
//   (_Rb_tree::insert_unique) steps it.

namespace std {
template <class _T1, class _T2> struct pair;
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
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

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class _Tp> struct _Nonconst_traits { };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Self& operator--();
};

template <class _Value, class _Traits>
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{
    _M_node = _Rb_global_inst::_M_decrement(_M_node);
    return *this;
}

typedef unsigned long _TreeValue;
template _Rb_tree_iterator<_TreeValue, _Nonconst_traits<_TreeValue> >&
_Rb_tree_iterator<_TreeValue, _Nonconst_traits<_TreeValue> >::operator--();
}
