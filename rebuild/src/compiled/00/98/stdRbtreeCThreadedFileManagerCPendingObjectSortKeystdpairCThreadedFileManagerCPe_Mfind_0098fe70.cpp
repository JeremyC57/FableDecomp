// std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const key, CFileObject*>, _Select1st, less, allocator>::_M_find(const CPendingObjectSortKey& k) const -- STLport _Rb_tree::_M_find of CThreadedFileManager's map<CPendingObjectSortKey, CFileObject*>: the node whose key equals k (lower bound, then a k < key check), the header when absent
// Leaf: ?_M_find@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098FE70, length 150 bytes, `ret 4`, the node pointer in eax.
// Callers: the landed CThreadedFileManager::RemoveObjectFromPendingMap @ 0x0098E770 (call 0x0098E79D), CThreadedFileManager::CloseAllStreamsForFile
//   @ 0x0098E7D0 (call 0x0098E9B7), 0x009902B5 and the landed find wrapper @ 0x00990320 (call 0x00990415).
// Callees: none (the key compare is inline).
// Layout (Ego_r.pdb): CThreadedFileManager::CPendingObjectSortKey = { CFileObject* Object }; CFileObject: Desc
//   (CThreadedFileObjectDesc: PDst +0, FilePos +4, ...) +0x00, PFile (CThreadedFile*) +0x28; CThreadedFile::
//   PhysicalSortKey +0x14. STLport tree = { _M_header +0, _M_node_count +4, _M_key_compare +8 }; node base { _M_color +0,
//   _M_parent +4, _M_left +8, _M_right +0xC }, key at +0x10. FableWin.pdb names the member operator
//   `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`.
// Shape: STLport 4.x `_M_find`: the lower-bound walk (`if (!comp(key(x), k)) y = x, x = left(x); else x = right(x)`), then
//   `if (y == header || comp(k, key(y))) y = header; return y;` (the header is kept in the `push ecx` frame word), with
//   the sort-key operator< inline: unsigned compare of the physical sort keys (`jb`/`ja`), then of the file positions,
//   then `Object < o.Object` as a `setb` bool that the walk tests. No call in the loop, so k's object and its physical
//   sort key are hoisted into ebx/ebp. Tried: the if/assign form, the returned ternary, `if (y != header && !comp) return
//   y;` (all 150v150 with the same swap) and an inline _M_lower_bound + ternary (not inlined, 73 bytes).
// Model: the real STLport 4.6 class template std::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> (5 parameters,
//   _Rb_tree_base's _STLP_alloc_proxy header) instantiated for CThreadedFileManager's pending map (the same tree whose
//   _M_insert / insert_unique / iterator operator++ are already bound under these std:: names); the class keys are the PDB's
//   (class CFileObject, class CPendingObjectSortKey). Earlier landed through the invented stand-in class
//   CPendingObjectTree_0098fe70 (FINDINGS chunk 482); same bytes. The catalog's `_Tree<_Tmap_traits<...>>::_Lbound` is
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
    _Base_ptr _M_find(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;  // last node which is not less than __k
    _Link_type __x = _M_root();                // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}

typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> _PendingValue;
typedef _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, _PendingValue, _Select1st<_PendingValue>,
                 less<CThreadedFileManager::CPendingObjectSortKey>, allocator<_PendingValue> > _PendingTree;
template _Rb_tree_node_base* _PendingTree::_M_find(const CThreadedFileManager::CPendingObjectSortKey&) const;

} // namespace std
