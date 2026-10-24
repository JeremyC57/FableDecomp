// CMovementAccelerationMap::_M_insert(SRbNodeBase* x, SRbNodeBase* y, const SAccelerationValue& v, SRbNodeBase* w) -- STLport _Rb_tree<EGameAction, pair<const EGameAction, pair<C3DVector,long>>>::_M_insert: allocate a node, copy the 20-byte value in, link it left/right of y, rebalance and count
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AE900, 227 bytes (+ int3 padding), `ret 14h` (hidden iterator result + 4 arguments).
// Callees: malloc 0x00BFEA0E (the 0x24-byte node: 0x10 links + the 20-byte pair), _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (__cdecl).
// Identity: called only by the landed insert_unique 0x006AE9F0, which CThingPlayerCreature::
//   ApplyRelativeMovementAcceleration 0x006AD3C0 uses on Ego_r.pdb CThingPlayerCreature::
//   RelativeMovementAccelerationComponents = map<EGameAction, pair<C3DVector, long>>. The catalog's
//   "TreeSet_InsertLong5" is a descriptive auto label.
// Layout: tree {header, count} as 0x0069D520; the value at node +0x10: the EGameAction key, the C3DVector (+4..+0xc)
//   and the long (+0x10), copied as five straight dword moves.
// Shape: the _M_insert of 0x0069D520 / 0x006F1280 (same condition, duplicated _M_create_node, iterator by value).
//   The nested pair must copy IMPLICITLY: a declared memberwise copy constructor on both pair levels makes VC7.1
//   stop inlining _M_create_node, and forcing it walks the inner pair through advanced pointers (`add esi,4`).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

enum EGameAction { GAME_ACTION_NONE };

struct C3DVector {
    float X, Y, Z;
};

template <class T1, class T2>
struct pair {                                               // implicit (memberwise, POD) copy
    T1 first;
    T2 second;
};

typedef pair<const EGameAction, pair<C3DVector, long> > SAccelerationValue;   // 20 bytes

struct SRbNodeBase {
    long         _M_color;                                  // +0x00
    SRbNodeBase* _M_parent;                                 // +0x04
    SRbNodeBase* _M_left;                                   // +0x08
    SRbNodeBase* _M_right;                                  // +0x0c
};

struct SRbNode : SRbNodeBase {
    SAccelerationValue _M_value_field;                        // +0x10
};

extern "C" void __cdecl Rebalance_0042971d(SRbNodeBase* node, SRbNodeBase*& root);   // 0x0042971D

struct CMovementAccelerationMap {
    SRbNodeBase*  _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        SRbNodeBase* _M_node;
        iterator(SRbNodeBase* node) : _M_node(node) { }
    };
    SRbNode* _M_create_node(const SAccelerationValue& v)
    {
        SRbNode* tmp = (SRbNode*)malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SAccelerationValue(v);
        return tmp;
    }
    iterator _M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SAccelerationValue& v, SRbNodeBase* w_);
};

CMovementAccelerationMap::iterator CMovementAccelerationMap::_M_insert(SRbNodeBase* x_, SRbNodeBase* y_, const SAccelerationValue& v, SRbNodeBase* w_)
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