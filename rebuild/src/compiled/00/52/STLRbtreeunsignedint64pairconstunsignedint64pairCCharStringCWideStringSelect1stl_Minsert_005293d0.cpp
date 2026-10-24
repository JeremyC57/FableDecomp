// _STL::_Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CThingManager::NPCNameMap: allocate a node, copy-construct the UID and the (name, display name) pair in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x005293D0, 214 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 8-byte UID + CCharString + CWideString),
//   _M_create_node 0x005289C0 (the out-of-line copy of the inline member; manifest "CSplineEntry_Create" is
//   propagated), CCharString copy ctor 0x0099EC30, CWideString copy ctor 0x0099B720, _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique / insert(hint) bodies 0x0052950B .. 0x0052A4D7 (nine call sites).
// Label correction: catalogued as "ListNode_InsertSpline"; it is STLport `_Rb_tree::_M_insert` of
//   map<unsigned __int64, pair<CCharString, CWideString> > (Ego_r.pdb CThingManager::NPCNameMap).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; header parent = root (+4), left =
//   leftmost (+8), right = rightmost (+0xc); the value at node +0x10: UID, then CCharString +0x18, CWideString +0x1c.
// Shape: the shared STLport `_M_insert` source; VC7.1 inlines `_M_create_node` in the left arm and calls the
//   out-of-line copy from the right arm (inline-once band). STLport's pair copy constructors are memberwise: the key
//   words, then the inner pair's two string copy constructors; the inner pair's `this` rides in the dead `y` argument
//   slot across the first call (`mov [esp+20h],eax` / `mov ecx,[esp+1Ch] ; add ecx,4`).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PData;                                            // CBasicString<char>* (4 bytes)
    CCharString(const CCharString& other);                  // 0x0099EC30
};

class CWideString {
public:
    void* PData;                                            // CBasicString<wchar_t>* (4 bytes)
    CWideString(const CWideString& other);                  // 0x0099B720
};

struct SNPCName {                                           // pair<CCharString, CWideString>
    CCharString first;                                      // +0x00
    CWideString second;                                     // +0x04
    SNPCName(const SNPCName& other) : first(other.first), second(other.second) { }
};

struct SNPCNamePair {                                       // pair<const unsigned __int64, pair<CCharString, CWideString> >
    unsigned __int64 first;                                 // +0x00
    SNPCName         second;                                // +0x08
    SNPCNamePair(const SNPCNamePair& other) : first(other.first), second(other.second) { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00 (STLport _Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

struct SNPCNameNode : _Rb_tree_node_base {
    SNPCNamePair _M_value_field;                           // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(_Rb_tree_node_base* node, _Rb_tree_node_base*& root);   // 0x0042971D

struct CNPCNameTree {                                    // _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* node) : _M_node(node) { }
    };
    SNPCNameNode* _M_create_node(const SNPCNamePair& v)
    {
        SNPCNameNode* tmp = (SNPCNameNode*)malloc(sizeof(SNPCNameNode));
        new (&tmp->_M_value_field) SNPCNamePair(v);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SNPCNamePair& v, _Rb_tree_node_base* w_);
};

CNPCNameTree::iterator CNPCNameTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SNPCNamePair& v, _Rb_tree_node_base* w_)
{
    SNPCNameNode* w = (SNPCNameNode*)w_;
    SNPCNameNode* y = (SNPCNameNode*)y_;
    SNPCNameNode* x = (SNPCNameNode*)x_;
    SNPCNameNode* z;
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