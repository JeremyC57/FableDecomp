// CPendingObjectTree_009901d0::_M_lower_bound(const CPendingObjectSortKey& k) const -- STLport _Rb_tree::_M_lower_bound of CThreadedFileManager's map<CPendingObjectSortKey, CFileObject*>: the first pending object that does not sort before k (by physical file sort key, file position, then object address), the header when none
// Leaf: _M_lower_bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009901D0, length 89 bytes, `ret 4`, the node pointer in eax.
// Callers: CThreadedFileManager's request pump @ 0x0098E7D0 (call 0x0098EA21), the landed range erase @ 0x009905D0 (call
//   0x00990695), the map's equal_range @ 0x009906B0 (call 0x009906C4), the tree teardown @ 0x00990C60 (call 0x00990CB5),
//   RemovePendingObject @ 0x00990CD0 (call 0x00990CE8) and 0x0099135E.
// Callees: none (the key compare is inline).
// Layout (Ego_r.pdb): CThreadedFileManager::CPendingObjectSortKey = { CFileObject* Object }; CFileObject: Desc
//   (CThreadedFileObjectDesc: PDst +0, FilePos +4, ...) +0x00, PFile (CThreadedFile*) +0x28; CThreadedFile::
//   PhysicalSortKey +0x14. STLport tree = { _M_header +0, comparator, _M_node_count }; node base { _M_color +0,
//   _M_parent +4, _M_left +8, _M_right +0xC }, key at +0x10. FableWin.pdb names the member operator
//   `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`.
// Shape: `y = header; x = root; while (x) if (!comp(key(x), k)) y = x, x = left(x); else x = right(x); return y;` with
//   the sort-key operator< inline: unsigned compare of the physical sort keys (`jb`/`ja`), then of the file positions,
//   then `Object < o.Object` as a `setb` bool that the walk tests (`jne` to the right branch). No call in the loop, so
//   k's object and its physical sort key are hoisted into ebx/ebp.
// Label: the catalog's `_Tree<_Tmap_traits<CPendingObjectSortKey,...>>::_Lbound` is the Dinkumware name of this lower bound
//   (0x0098FCA0 is the upper bound, 0x0098FE70 the find).
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
            if (Object->PFile->PhysicalSortKey < o.Object->PFile->PhysicalSortKey)
                return true;
            if (Object->PFile->PhysicalSortKey > o.Object->PFile->PhysicalSortKey)
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
    T1 first;
    T2 second;
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

class CPendingObjectTree_009901d0 {            // STLport _Rb_tree<CPendingObjectSortKey, pair<const key, CFileObject*>, ...>
public:
    typedef std::pair<const CPendingObjectSortKey, CThreadedFileManager::CFileObject*> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*                           _M_header;        // +0x0
    std::less<CPendingObjectSortKey> _M_key_compare;
    unsigned long                    _M_node_count;
    static _Node* _S_left(_Rb_tree_node_base* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Rb_tree_node_base* x) { return (_Node*)x->_M_right; }
    static const CPendingObjectSortKey& _S_key(_Node* x) { return x->_M_value_field.first; }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }
    _Node* _M_lower_bound(const CPendingObjectSortKey& k) const;
};

CPendingObjectTree_009901d0::_Node* CPendingObjectTree_009901d0::_M_lower_bound(const CPendingObjectSortKey& k) const
{
    _Node* y = _M_header;                      // last node which is not less than k
    _Node* x = _M_root();                      // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
    return y;
}