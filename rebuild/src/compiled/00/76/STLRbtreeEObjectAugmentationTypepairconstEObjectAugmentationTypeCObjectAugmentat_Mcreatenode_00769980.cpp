// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> >::_M_create_node(const value_type& x) -- allocate a 0x20-byte tree node and copy-construct the (augmentation type, sound set) pair into it
// Leaf: ?_M_create_node@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769980, 62 bytes (+int3 pad to 64), `ret 4` (thiscall; `this` is never read).
// Caller: 0x0076A110 _M_insert (its right-hand branch; the left-hand branch carries an inlined copy).
// Callees: malloc 0x00BFEA0E (STLport's malloc allocator, n = 1 folded into `push 20h`), CCharString copy
//   constructor 0x0099EC30.
// Identity: Ego_r.pdb CTCObjectAugmentations::InitSounds / LoopingSounds are map<EObjectAugmentationType,
//   CObjectAugmentationSoundSet>; the 0x10-byte value (key +0, Played +4, Sound +8, Handle +0xc) sits after the
//   0x10-byte node links. The catalog's "RBTree_Node_Allocate_And_Construct" is a descriptive auto label.
// Shape: STLport `_M_create_node`: allocate, then `_Construct(&tmp->_M_value_field, x)` (placement new, null-tested);
//   the pair's user copy constructor copies the key, then the sound set's implicit memberwise copy (byte, string
//   copy, dword). The node is returned in eax.
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

struct CSoundSetTree {                                      // _Rb_tree<EObjectAugmentationType, SSoundSetValue, ...>
    std::_Rb_tree_node_base* _M_header;                    // +0x00
    unsigned long             _M_node_count;                // +0x04
    SSoundSetNode* _M_create_node(const SSoundSetValue& __x);
};

SSoundSetNode* CSoundSetTree::_M_create_node(const SSoundSetValue& __x)
{
    SSoundSetNode* __tmp = (SSoundSetNode*)malloc(sizeof(SSoundSetNode));
    new (&__tmp->_M_value_field) SSoundSetValue(__x);
    return __tmp;
}