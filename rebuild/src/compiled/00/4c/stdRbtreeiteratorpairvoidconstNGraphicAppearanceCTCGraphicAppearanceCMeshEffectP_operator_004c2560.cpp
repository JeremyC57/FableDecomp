// std::_Rb_tree_iterator<pair<void* const, CMeshEffectPriorityEntry>, _Nonconst_traits<pair<void* const, CMeshEffectPriorityEntry> > >::operator++() -- the appearance mesh-effect map's iterator step: the in-order successor node (leftmost of the right subtree, else climb past right children; end() stays put)
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@U?$_Nonconst_traits@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C2560, 62 bytes, thiscall on the iterator, returns it in eax.
// Identity (sweep1, 2026-10-25): FINDINGS (near-miss pass 2, hand902): "CRetailTreeIterator_Increment_004c2560 ... use
//   invented classes (STLport _Rb_tree_iterator::operator++ ...)". Its three callers are the
//   CTCGraphicAppearance::MeshEffects map code: the landed NGraphicAppearance::CTCGraphicAppearance::GetPMeshEffect
//   0x004BCF50 (call 0x004BCF77; its reloc binding already names this Leaf -> 004c2560) and the map's hinted
//   insert_unique 0x004C5480 (calls 0x004C5506 / 0x004C562A). Ego_r.pdb: CTCGraphicAppearance::MeshEffects is
//   map<void*, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>. The catalog name
//   CRetailTreeIterator::Increment is a generated stand-in shared by 48 bodies of this shape (one COMDAT per value type).
// Shape: STLport 4.6 `operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }` with the
//   template static _Rb_global<bool>::_M_increment inlined (its out-of-line copy is 0x004292C0).
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    struct CMeshEffectPriorityEntry {                       // Ego_r.pdb, 8 bytes
        void* PEffect;                                      // +0x0
        long  Priority;                                     // +0x4 EAppearanceMeshEffectPriority
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

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

typedef pair<void* const, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry> _MeshEffectValue;
template _Rb_tree_iterator<_MeshEffectValue, _Nonconst_traits<_MeshEffectValue> >&
_Rb_tree_iterator<_MeshEffectValue, _Nonconst_traits<_MeshEffectValue> >::operator++();
}
