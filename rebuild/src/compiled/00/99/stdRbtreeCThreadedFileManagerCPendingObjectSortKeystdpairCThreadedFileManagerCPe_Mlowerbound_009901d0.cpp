// std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const key, CFileObject*>, _Select1st, less, allocator>::_M_lower_bound(const CPendingObjectSortKey& k) const -- STLport _Rb_tree::_M_lower_bound of CThreadedFileManager's map<CPendingObjectSortKey, CFileObject*>: the first pending object that does not sort before k (by physical file sort key, file position, then object address), the header when none
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009901D0, length 89 bytes, `ret 4`, the node pointer in eax.
// Callers: CThreadedFileManager::CloseAllStreamsForFile @ 0x0098E7D0 (FINDINGS chunk 482) (call 0x0098EA21), the landed range erase @ 0x009905D0 (call
//   0x00990695), the map's equal_range @ 0x009906B0 (call 0x009906C4), the tree teardown @ 0x00990C60 (call 0x00990CB5),
//   RemovePendingObject @ 0x00990CD0 (call 0x00990CE8) and 0x0099135E.
// Callees: none (the key compare is inline).
// Layout (Ego_r.pdb): CThreadedFileManager::CPendingObjectSortKey = { CFileObject* Object }; CFileObject: Desc
//   (CThreadedFileObjectDesc: PDst +0, FilePos +4, ...) +0x00, PFile (CThreadedFile*) +0x28; CThreadedFile::
//   PhysicalSortKey +0x14. STLport tree = { _M_header +0, _M_node_count +4, _M_key_compare +8 }; node base { _M_color +0,
//   _M_parent +4, _M_left +8, _M_right +0xC }, key at +0x10. FableWin.pdb names the member operator
//   `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`.
// Shape: `y = header; x = root; while (x) if (!comp(key(x), k)) y = x, x = left(x); else x = right(x); return y;` with
//   the sort-key operator< inline: unsigned compare of the physical sort keys (`jb`/`ja`), then of the file positions,
//   then `Object < o.Object` as a `setb` bool that the walk tests (`jne` to the right branch). No call in the loop, so
//   k's object and its physical sort key are hoisted into ebx/ebp.
// Label: the catalog's `_Tree<_Tmap_traits<CPendingObjectSortKey,...>>::_Lbound` is the Dinkumware name of this lower bound
//   (0x0098FCA0 is the upper bound, 0x0098FE70 the find).
// Model: the real STLport 4.6 class template std::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> (5 parameters,
//   _Rb_tree_base's _STLP_alloc_proxy header) instantiated for CThreadedFileManager's pending map (the same tree whose
//   _M_insert / insert_unique / iterator operator++ are already bound under these std:: names); the class keys are the PDB's
//   (class CFileObject, class CPendingObjectSortKey). Earlier landed through the invented stand-in class
//   CPendingObjectTree_009901d0 (FINDINGS chunk 482); same bytes. The catalog's `_Tree<_Tmap_traits<...>>::_Lbound` is
//   propagated (0x0098FE70 is the find, 0x0098FCA0 the upper bound, 0x009901D0 the lower bound).
class CThreadedFile {
public:
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;             // +0x14
};
class CThreadedFileObjectDesc {
public:
    void*         PDst;                        // +0x00
    unsigned long FilePos;                     // +0x04
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    class CFileObject {
    public:
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

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x00
    _Rb_tree_node_base* _M_parent;             // +0x04
    _Rb_tree_node_base* _M_left;               // +0x08
    _Rb_tree_node_base* _M_right;              // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                     // +0x10
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Alloc> _M_header;     // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                // +0x04
    _Compare     _M_key_compare;               // +0x08
    protected:
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    public:
    static _Link_type _S_left(_Base_ptr __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Base_ptr __x) { return (_Link_type)__x->_M_right; }
    static _Value& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(__x)); }
    private:
    _Base_ptr _M_lower_bound(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;  // last node which is not less than __k
    _Link_type __x = _M_root();                // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}

typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> _PendingValue;
typedef _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, _PendingValue, _Select1st<_PendingValue>,
                 less<CThreadedFileManager::CPendingObjectSortKey>, allocator<_PendingValue> > _PendingTree;
template _Rb_tree_node_base* _PendingTree::_M_lower_bound(const CThreadedFileManager::CPendingObjectSortKey&) const;

} // namespace std
