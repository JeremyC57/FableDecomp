// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, _Select1st, less<unsigned __int64> >::_M_copy(_Rb_tree_node<value_type>* x, _Rb_tree_node<value_type>* p) -- STLport structural subtree copy of CEntityRuntimePersistence::PersistedDeadEntities: clone x under parent p, recurse into every right subtree and walk the left spine iteratively; returns the new subtree root
// Leaf: _M_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529230, 180 bytes, `ret 8` (thiscall: x, p).
// Callees: _M_clone_node 0x00528930 (the out-of-line copy of the inline member: create + colour + null children;
//   manifest "CCreatureAction_NavToPos::Clone" is propagated), malloc 0x00BFEA0E (the inlined clone in the loop:
//   the 0x28-byte node), itself (the right subtrees).
// Callers: itself (0x00529259, 0x005292C9) and the tree's operator= / copy 0x0052A0D4.
// Label correction: catalogued as "CCreatureAction_TreeClone"; it is STLport `_Rb_tree::_M_copy` of
//   map<unsigned __int64, CPersistedDeadEntity> (the 0x28-byte nodes of the twin _M_insert 0x00529150).
// Layout: nodes = { _M_color (bool) +0, _M_parent +4, _M_left +8, _M_right +0xc, value +0x10 (UID key, then the
//   16-byte record) }.
// Shape: STLport 4.x `_M_copy` verbatim (the _STLP_TRY / _STLP_UNWIND(_M_erase(top)) pair compiles away without
//   exceptions); VC7.1 calls the out-of-line `_M_clone_node` for `top` and inlines the in-loop clone (its inline-once
//   band: one expansion, the other site calls the copy). `this` lives in the `push ecx` frame word (every callee-saved
//   register holds x, p/y, top); the colour byte goes through dl, the two null children share `xor ecx,ecx`.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CPersistedDeadEntity {                               // 16 bytes (Ego_r.pdb NThingManager::CEntityRuntimePersistence)
    unsigned __int64 UID;                                   // +0x00
    bool             GamePersistent;                        // +0x08
    unsigned long    SectionNameCRC;                        // +0x0c
};

struct SDeadEntityPair {                                    // pair<const unsigned __int64, CPersistedDeadEntity>
    unsigned __int64     first;                             // +0x00
    CPersistedDeadEntity second;                            // +0x08
    SDeadEntityPair(const SDeadEntityPair& other) : first(other.first), second(other.second) { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00 (STLport _Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

struct SDeadEntityNode : _Rb_tree_node_base {
    SDeadEntityPair _M_value_field;                           // +0x10
};

struct CDeadEntityTree {                                    // _Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, ...>
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* x) { return x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* x) { return x->_M_right; }
    SDeadEntityNode* _M_create_node(const SDeadEntityPair& v)
    {
        SDeadEntityNode* tmp = (SDeadEntityNode*)malloc(sizeof(SDeadEntityNode));
        new (&tmp->_M_value_field) SDeadEntityPair(v);
        return tmp;
    }
    SDeadEntityNode* _M_clone_node(SDeadEntityNode* x)
    {
        SDeadEntityNode* tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
    SDeadEntityNode* _M_copy(SDeadEntityNode* x, SDeadEntityNode* p);
};

SDeadEntityNode* CDeadEntityTree::_M_copy(SDeadEntityNode* x, SDeadEntityNode* p)
{
    // structural copy: x and p must be non-null
    SDeadEntityNode* top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy((SDeadEntityNode*)_S_right(x), top);
    p = top;
    x = (SDeadEntityNode*)_S_left(x);
    while (x != 0) {
        SDeadEntityNode* y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy((SDeadEntityNode*)_S_right(x), y);
        p = y;
        x = (SDeadEntityNode*)_S_left(x);
    }
    return top;
}