// _STL::_Rb_tree<CCharString,pair<long,long>>::erase(iterator first, iterator last) -- erase a node range of the region -> (crime duration, fine) map: clear() for [begin, end), else erase(first++) node by node
// Leaf: ?erase@_Rb_tree_CrimeMap@_STL@@QAEXU_Rb_tree_iterator_CrimeMap@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756420, 196 bytes (+12 int3), __fastcall `ret 8` (two by-value 4-byte iterators).
// Callees: _Rb_tree::_M_erase(node) @ 0x00755E80 (recursive subtree release; manifest "LTextTreeWalkThrough::BuildTreeArray"
//   is a propagation), _Rb_global::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl, landed
//   as Unlink_0042a34e), CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Caller: 0x00756CAB, the map's erase(const key&) (0x00756C60).
// Label: the manifest's "LTextBinTree_DeleteRange" is descriptive; the node layout (0x1C: colour/parent/left/right +
//   pair<const CCharString, pair<long,long>>) and the callers tie it to CTCHeroRememberCrimes'
//   RegionNamesToCrimeDurationAndFine map (its ctor 0x007555F0 mallocs the 0x1C header).
// Shape: STLport `if (first == begin() && last == end()) clear(); else while (first != last) erase(first++);` --
//   begin() (header->left) is loaded before the arguments; clear() tests the count against a zero register shared with
//   the root reset; the post-increment computes the in-order successor inline (right subtree's leftmost, else climb
//   while a right child) keeping the victim in ecx; erase(position) unlinks through the rebalance helper with the
//   addresses of the header's parent/left/right links, destroys the CCharString key, frees the node behind its null
//   test and decrements the count. Same shape as the landed 0x00835440 (trivial values) plus the key destructor.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

namespace _STL {
struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0C
};

struct SCrimeValue {                                         // pair<const CCharString, pair<long,long>>
    CCharString first;                                       // +0x10 in the node
    long        Duration;
    long        Fine;
};

struct _Rb_tree_node_CrimeMap : _Rb_tree_node_base {
    SCrimeValue _M_value_field;                              // +0x10
};

extern "C" _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
    _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E

struct _Rb_tree_iterator_CrimeMap {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator_CrimeMap(_Rb_tree_node_base* node) : _M_node(node) {}
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
    _Rb_tree_iterator_CrimeMap operator++(int)
    {
        _Rb_tree_iterator_CrimeMap tmp = *this;
        _M_increment();
        return tmp;
    }
    bool operator==(const _Rb_tree_iterator_CrimeMap& other) const { return _M_node == other._M_node; }
    bool operator!=(const _Rb_tree_iterator_CrimeMap& other) const { return _M_node != other._M_node; }
};

struct _Rb_tree_CrimeMap {
    typedef _Rb_tree_iterator_CrimeMap iterator;
    _Rb_tree_node_base* _M_header;                           // +0x00
    unsigned long       _M_node_count;                       // +0x04

    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Rb_tree_node_base* x);                    // 0x00755E80
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    void erase(iterator position)
    {
        _Rb_tree_node_CrimeMap* y = (_Rb_tree_node_CrimeMap*)_Rebalance_for_erase(position._M_node,
            _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        y->_M_value_field.first.~CCharString();
        if (y)
            free(y);
        --_M_node_count;
    }
    void erase(iterator first, iterator last);
};

void _Rb_tree_CrimeMap::erase(iterator first, iterator last)
{
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}
}