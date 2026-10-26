// CCameraModeMap::_M_insert(SRbNodeBase* x, SRbNodeBase* y, const SCameraModeValue& v, SRbNodeBase* w) -- STLport _Rb_tree<NCameraModes::EMode, pair<const EMode, CCountedPointer<CBaseInit>>>::_M_insert: allocate a node, copy the value in (bumping the counted pointer), link it left/right of y, rebalance and count
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069D520, 211 bytes, `ret 14h` (hidden iterator result + 4 arguments).
// Callees: malloc 0x00BFEA0E (the 0x1c-byte node: 0x10 links + the 12-byte pair),
//   _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (__cdecl; the extern "C" spelling the landed tree inserts use;
//   the manifest's "CMemoryAllocatorVariableSize::BalanceTree" is propagated).
// Identity: called only by the landed insert_unique 0x0069D900, which CGameCameraManager's constructor uses six times
//   to fill Ego_r.pdb CGameCameraManager::RegisteredModes = map<NCameraModes::EMode, CCountedPointer<
//   NCameraModes::CBaseInit>>. The catalog's "TreeSet_InsertIntInt" is a descriptive auto label.
// Layout: the tree is {header node pointer, node count}; header parent = root (+4), left = leftmost (+8), right =
//   rightmost (+0xc); the value at node +0x10: the EMode key, then CCountedPointer {Data +4, Info +8} whose copy
//   bumps CCPPointerInfo::RefCount (+0 of the info block, null-tested).
// Shape: STLport's _M_insert condition `y == header || (w == 0 && (x != 0 || key(v) < key(y)))` (signed compare);
//   _M_create_node is the placement-new construct with its null test, duplicated in both branches; the iterator is
//   returned by value through the hidden pointer (the landed CThingCompareMap::Insert 0x006F1280 shape).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace NCameraModes { class CBaseInit; enum EMode { MODE_NONE }; }

class CCPPointerInfo { public:
    unsigned long RefCount;                                 // +0x00
    void (__cdecl* DeleteFunc)(void*);                      // +0x04
    void* Data;                                             // +0x08
};

template <class T>
class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

struct SCameraModeValue {                                   // pair<const NCameraModes::EMode, CCountedPointer<CBaseInit>>
    const NCameraModes::EMode first;                        // +0x00
    CCountedPointer<NCameraModes::CBaseInit> second;        // +0x04
    SCameraModeValue(const SCameraModeValue& other) : first(other.first), second(other.second) { }
};

struct SRbNodeBase {
    long         _M_color;                                  // +0x00
    SRbNodeBase* _M_parent;                                 // +0x04
    SRbNodeBase* _M_left;                                   // +0x08
    SRbNodeBase* _M_right;                                  // +0x0c
};

struct SRbNode : SRbNodeBase {
    SCameraModeValue _M_value_field;                        // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(SRbNodeBase* node, SRbNodeBase*& root);   // 0x0042971D

struct CCameraModeMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SCameraModeValue& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SCameraModeValue(v);
        return tmp;
    }
    iterator _M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SCameraModeValue& v, SRbNodeBase* w_);
};

CCameraModeMap::iterator CCameraModeMap::_M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SCameraModeValue& v, SRbNodeBase* w_)
{
    SRbNode* w = (SRbNode*)w_;
    SRbNode* y = (SRbNode*)y_;
    SRbNode* x = (SRbNode*)x_;
    SRbNode* z;
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