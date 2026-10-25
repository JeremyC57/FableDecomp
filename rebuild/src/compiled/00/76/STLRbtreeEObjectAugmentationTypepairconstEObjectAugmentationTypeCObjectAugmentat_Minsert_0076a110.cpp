// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- link a new (augmentation type, sound set) node left or right of y, keep leftmost/rightmost, rebalance, count it
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A110, 199 bytes (+int3 pad to 208), `ret 14h` (hidden iterator result + 4 arguments).
// Callers: nine sites in 0x0076A1E0..0x0076A891 (insert_unique / hint insert of the same tree).
// Callees: malloc 0x00BFEA0E and CCharString copy constructor 0x0099EC30 (the inlined left-hand node construction),
//   _M_create_node 0x00769980 (right-hand branch), _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (__cdecl).
// Identity: Ego_r.pdb CTCObjectAugmentations::InitSounds / LoopingSounds = map<EObjectAugmentationType,
//   CObjectAugmentationSoundSet>; the value is the 0x10-byte pair at node +0x10 (key compared signed: `jl`).
// Shape: the landed STLport _M_insert 0x0069D520 with `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`; this
//   instantiation inlines _M_create_node in the left-hand branch only (a `__forceinline` copy, GOTCHAS 0x4C4890) and
//   calls the out-of-line member in the right-hand branch. The iterator returns through the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_NONE };

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
};

struct CObjectAugmentationSoundSet {                        // Ego_r.pdb, 12 bytes
    bool          Played;                                   // +0x00
    CCharString   Sound;                                    // +0x04
    unsigned long Handle;                                   // +0x08
};

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};

struct _Rb_tree_node_base {
    long                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

}

typedef std::pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> SSoundSetValue;   // 0x10 bytes
typedef std::_Rb_tree_node<SSoundSetValue> SSoundSetNode;                                          // 0x20 bytes

extern "C" void __cdecl Rebalance_0042971d(std::_Rb_tree_node_base* node, std::_Rb_tree_node_base*& root);   // 0x0042971D

struct CSoundSetTree {                                      // _Rb_tree<EObjectAugmentationType, SSoundSetValue, ...>
    std::_Rb_tree_node_base* _M_header;                    // +0x00
    unsigned long             _M_node_count;                // +0x04
    struct iterator {
        std::_Rb_tree_node_base* _M_node;
        iterator(std::_Rb_tree_node_base* node) : _M_node(node) { }
    };
    SSoundSetNode* _M_create_node(const SSoundSetValue& __x);   // 0x00769980
    __forceinline SSoundSetNode* _M_create_node_inline(const SSoundSetValue& __x)
    {
        SSoundSetNode* __tmp = (SSoundSetNode*)malloc(sizeof(SSoundSetNode));
        new (&__tmp->_M_value_field) SSoundSetValue(__x);
        return __tmp;
    }
    iterator _M_insert(std::_Rb_tree_node_base* __x_, std::_Rb_tree_node_base* __y_, const SSoundSetValue& __v, std::_Rb_tree_node_base* __w_);
};

CSoundSetTree::iterator CSoundSetTree::_M_insert(std::_Rb_tree_node_base* __x_, std::_Rb_tree_node_base* __y_, const SSoundSetValue& __v, std::_Rb_tree_node_base* __w_)
{
    SSoundSetNode* __w = (SSoundSetNode*)__w_;
    SSoundSetNode* __y = (SSoundSetNode*)__y_;
    SSoundSetNode* __x = (SSoundSetNode*)__x_;
    SSoundSetNode* __z;
    if (__y == _M_header || (__w == 0 && (__x != 0 || __v.first < __y->_M_value_field.first))) {
        __z = _M_create_node_inline(__v);
        __y->_M_left = __z;
        if (__y == _M_header) {
            _M_header->_M_parent = __z;
            _M_header->_M_right = __z;
        }
        else if (__y == _M_header->_M_left)
            _M_header->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == _M_header->_M_right)
            _M_header->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    Rebalance_0042971d(__z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}