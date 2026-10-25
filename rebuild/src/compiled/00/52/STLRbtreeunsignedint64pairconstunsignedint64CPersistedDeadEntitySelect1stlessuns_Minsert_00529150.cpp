// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CEntityRuntimePersistence::PersistedDeadEntities: allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529150, 210 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x28-byte node: 0x10 links + the 8-byte UID + the 16-byte CPersistedDeadEntity),
//   _M_create_node 0x005288F0 (the out-of-line copy of the inline member, manifest "CCreatureAction_NavToPos::Clone"
//   is propagated), _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique pair at 0x0052A18B / 0x0052A1CD (reached from the landed
//   CEntityRuntimePersistence::AddPersistedDeadEntity 0x00520330 through insert 0x0052A130).
// Label correction: catalogued as "CRedBlackTree_Insert_With_Clone"; it is STLport `_Rb_tree::_M_insert` of
//   map<unsigned __int64, CPersistedDeadEntity> (Ego_r.pdb CEntityRuntimePersistence::PersistedDeadEntities).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; header parent = root (+4), left =
//   leftmost (+8), right = rightmost (+0xc); the value sits at node +0x10: the UID key then the record
//   {UID +0, GamePersistent +8, SectionNameCRC +0xc}.
// Shape: the same STLport `_M_insert` source as the landed UIDToThingMap / CThingCompareMap inserts; with the larger
//   value copy VC7.1 inlines `_M_create_node` in the left arm only and calls its out-of-line copy (0x005288F0) from
//   the right arm (the compiler's inline-once band, nothing in the source differs). The pair's memberwise copy
//   constructor copies the key words, the POD record goes as a block through re-based pointers (`add esi,8 ;
//   add ecx,8` then four words).
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

extern "C" void __cdecl Rebalance_0042971d(_Rb_tree_node_base* node, _Rb_tree_node_base*& root);   // 0x0042971D

struct CDeadEntityTree {                                    // _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* node) : _M_node(node) { }
    };
    SDeadEntityNode* _M_create_node(const SDeadEntityPair& v)
    {
        SDeadEntityNode* tmp = (SDeadEntityNode*)malloc(sizeof(SDeadEntityNode));
        new (&tmp->_M_value_field) SDeadEntityPair(v);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SDeadEntityPair& v, _Rb_tree_node_base* w_);
};

CDeadEntityTree::iterator CDeadEntityTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SDeadEntityPair& v, _Rb_tree_node_base* w_)
{
    SDeadEntityNode* w = (SDeadEntityNode*)w_;
    SDeadEntityNode* y = (SDeadEntityNode*)y_;
    SDeadEntityNode* x = (SDeadEntityNode*)x_;
    SDeadEntityNode* z;
    if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    Rebalance_0042971d(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}