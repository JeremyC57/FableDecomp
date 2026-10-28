// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, ...>::_M_clone_node(_Rb_tree_node*) -- STLport: copy one dead-entity map node (value, colour) with null children
// Leaf: ?_M_clone_node@?$_Rb_tree@_KU?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@
// Retail: Fable.exe @ 0x00528930, 81 bytes (16-aligned /O2 entry), thiscall `ret 4` (this unused), returns the node.
// Identity: the out-of-line _M_clone_node of NThingManager::CEntityRuntimePersistence::PersistedDeadEntities
// (map<unsigned __int64, CPersistedDeadEntity>): its only caller is that tree's _M_copy 0x00529230 (landed; it calls
// this for `top` and inlines the in-loop clone). The catalog label CCreatureAction_NavToPos::Clone is propagated.
// Callee: malloc 0x00BFEA0E (the 0x28-byte node: links 0x10 + the UID key 8 + the 16-byte CPersistedDeadEntity).
// Shape: STLport 4.6 `_M_clone_node`: `__tmp = _M_create_node(__x->_M_value_field)` (inlined: allocate(1), placement
// new with its null test, the pair copy = the key's two words, then the POD record as four words through re-based
// pointers), then colour copied and both children nulled from the shared zero register.
// Layout: Ego_r.pdb CPersistedDeadEntity { UID +0, GamePersistent +8, SectionNameCRC +0xC } (16 bytes).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace NThingManager {
class CEntityRuntimePersistence { public:
    class CPersistedDeadEntity { public:
        unsigned __int64 UID;                                // +0x00
        bool             GamePersistent;                     // +0x08
        unsigned long    SectionNameCRC;                     // +0x0C
    };
};
}

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                        // +0x10
};
template <class T> class allocator { public:
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> struct less {};
template <class P> struct _Select1st {};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class T1, class T2>
inline void _Construct(T1* p, const T2& val) { new (p) T1(val); }

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>  _Node;
    typedef _Rb_tree_node<V>* _Link_type;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                      // +0x04
    C            _M_key_compare;                                     // +0x08
protected:
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x);
};
template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::_Link_type _Rb_tree<K, V, KoV, C, A>::_M_clone_node(_Link_type x)
{
    _Link_type tmp = _M_create_node(x->_M_value_field);
    tmp->_M_color = x->_M_color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
}
typedef NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity CDeadEntityRecord;
template class _Rb_tree<unsigned __int64, pair<const unsigned __int64, CDeadEntityRecord>, _Select1st<pair<const unsigned __int64, CDeadEntityRecord> >,
                        less<unsigned __int64>, allocator<pair<const unsigned __int64, CDeadEntityRecord> > >;
}