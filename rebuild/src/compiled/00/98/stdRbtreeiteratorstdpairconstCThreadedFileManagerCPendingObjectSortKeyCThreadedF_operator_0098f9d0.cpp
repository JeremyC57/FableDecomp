// std::_Rb_tree_iterator::operator++() -- map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> iterator step to the in-order successor (leftmost of the right subtree, else climb past right children; end() stays put) (STLport 4.6, _Nonconst_traits): 
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098F9D0, 62 bytes, thiscall on the iterator, returns it in eax.
// Identity (sweep2 1b, owner decision 1b): called from the tree's insert path 0x98E770 (CThreadedFileManager::RemoveObjectFromPendingMap), 0x990DC0 (_global::map_operator_square_bracket); 2 call(s) up, 0x991350 std::map::operator[] binds this map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> instantiation (reloc_bindings); Ego_r.pdb member type map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>: CPendingObjectMap::Objects
//   The catalog name CRetailTreeIterator::Increment was a generated stand-in shared by every byte-identical body of this
//   shape; retail has no identical-code folding, so each address is one instantiation (one value type).
// Shape: STLport 4.6 `_Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }` with the template
//   static _Rb_global<bool>::_M_increment inlined. The iterator is the tree's own (non-const) iterator: its insert path
//   (_Rb_tree::insert_unique) steps it.
class CThreadedFileManager {
public:
    class CPendingObjectSortKey;
    class CFileObject;
};
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
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class _Tp> struct _Nonconst_traits { };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Self& operator++();
};

template <class _Value, class _Traits>
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator++()
{
    _M_node = _Rb_global_inst::_M_increment(_M_node);
    return *this;
}

typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject* > _TreeValue;
template _Rb_tree_iterator<_TreeValue, _Nonconst_traits<_TreeValue> >&
_Rb_tree_iterator<_TreeValue, _Nonconst_traits<_TreeValue> >::operator++();
}
