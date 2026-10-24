// CPendingObjectTree_0098fe70::_M_find(const CPendingObjectSortKey& k) const -- STLport _Rb_tree::_M_find of CThreadedFileManager's map<CPendingObjectSortKey, CFileObject*>: the node whose key equals k (lower bound, then a k < key check), the header when absent
// Grade: MATCH (near-miss retry agent901) -- STLport's _S_key chain (`_Select1st<value_type>()(_S_value(x))`) fixes the k/y register assignment of the post-loop compare, and CPendingObjectSortKey::operator< reads the two physical sort keys into locals in statement order (own key, then the other's), which gives retail's unscheduled load chains
// Leaf: ?_M_find@CPendingObjectTree_0098fe70@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAUCFileObject@2@@std@@@@ABVCPendingObjectSortKey@CThreadedFileManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098FE70, length 150 bytes, `ret 4`, the node pointer in eax.
// Callers: the landed CThreadedFileManager::RemoveObjectFromPendingMap @ 0x0098E770 (call 0x0098E79D), the request pump
//   @ 0x0098E7D0 (call 0x0098E9B7), 0x009902B5 and the landed find wrapper @ 0x00990320 (call 0x00990415).
// Callees: none (the key compare is inline).
// Layout (Ego_r.pdb): CThreadedFileManager::CPendingObjectSortKey = { CFileObject* Object }; CFileObject: Desc
//   (CThreadedFileObjectDesc: PDst +0, FilePos +4, ...) +0x00, PFile (CThreadedFile*) +0x28; CThreadedFile::
//   PhysicalSortKey +0x14. STLport tree = { _M_header +0, comparator, _M_node_count }; node base { _M_color +0,
//   _M_parent +4, _M_left +8, _M_right +0xC }, key at +0x10. FableWin.pdb names the member operator
//   `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`.
// Shape: STLport 4.x `_M_find`: the lower-bound walk (`if (!comp(key(x), k)) y = x, x = left(x); else x = right(x)`), then
//   `if (y == header || comp(k, key(y))) y = header; return y;` (the header is kept in the `push ecx` frame word), with
//   the sort-key operator< inline: unsigned compare of the physical sort keys (`jb`/`ja`), then of the file positions,
//   then `Object < o.Object` as a `setb` bool that the walk tests. No call in the loop, so k's object and its physical
//   sort key are hoisted into ebx/ebp. Tried: the if/assign form, the returned ternary, `if (y != header && !comp) return
//   y;` (all 150v150 with the same swap) and an inline _M_lower_bound + ternary (not inlined, 73 bytes).
// Label correction: the catalog's `_Tree<_Tmap_traits<CPendingObjectSortKey,...>>::_Lbound` is propagated (this is the
//   find; 0x009901D0 is the lower bound, 0x0098FCA0 the upper bound).
struct CThreadedFile {
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;             // +0x14
};
struct CThreadedFileObjectDesc {
    void*         PDst;                        // +0x00
    unsigned long FilePos;                     // +0x04
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    struct CFileObject {
        CThreadedFileObjectDesc Desc;          // +0x00
        unsigned long  UID;                    // +0x18
        long           LoadingStatus;          // +0x1C
        void*          Overlapped;             // +0x20
        CThreadedFileManager* PManager;        // +0x24
        CThreadedFile* PFile;                  // +0x28
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey& o) const
        {
            unsigned long key = Object->PFile->PhysicalSortKey;
            unsigned long otherKey = o.Object->PFile->PhysicalSortKey;
            if (key < otherKey)
                return true;
            if (key > otherKey)
                return false;
            if (Object->Desc.FilePos < o.Object->Desc.FilePos)
                return true;
            if (Object->Desc.FilePos > o.Object->Desc.FilePos)
                return false;
            return Object < o.Object;
        }
    };
};
typedef CThreadedFileManager::CPendingObjectSortKey CPendingObjectSortKey;

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CPendingObjectTree_0098fe70 {            // STLport _Rb_tree<CPendingObjectSortKey, pair<const key, CFileObject*>, ...>
public:
    typedef std::pair<const CPendingObjectSortKey, CThreadedFileManager::CFileObject*> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*                           _M_header;        // +0x0
    std::less<CPendingObjectSortKey> _M_key_compare;
    unsigned long                    _M_node_count;
    static _Node* _S_left(_Rb_tree_node_base* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Rb_tree_node_base* x) { return (_Node*)x->_M_right; }
    static value_type& _S_value(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field; }
    static const CPendingObjectSortKey& _S_key(_Rb_tree_node_base* x) { return std::_Select1st<value_type>()(_S_value(x)); }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }
    _Node* _M_find(const CPendingObjectSortKey& k) const;
};

CPendingObjectTree_0098fe70::_Node* CPendingObjectTree_0098fe70::_M_find(const CPendingObjectSortKey& k) const
{
    _Node* y = _M_header;                      // last node which is not less than k
    _Node* x = _M_root();                      // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
    if (y == _M_header || _M_key_compare(k, _S_key(y)))
        y = _M_header;
    return y;
}