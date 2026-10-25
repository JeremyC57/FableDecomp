// std::_Rb_tree::insert_unique(const value_type& v) -- map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> unique insert (STLport 4.6): descend comparing sort keys, step back once through the out-of-line decrement unless at begin(), insert through _M_insert when the key is new; {iterator, inserted} through the hidden pointer
// Leaf: ?insert_unique@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00990900, 272 bytes (+ int3 padding), thiscall `ret 8` (hidden pair result + the
//   value reference).
// Identity: the catalog's "RBTree_InsertOrFind" is descriptive. Callers: map::insert 0x00991000 (call 0x0099100B) and an
//   .rdata reference 0x012AE7AC. The decrement callee 0x0098FDD0 is the landed
//   _Rb_tree_iterator<pair<const CPendingObjectSortKey, CFileObject*>, _Nonconst_traits<..> >::operator--, and _M_insert
//   0x00990830 is the landed _Rb_tree<CPendingObjectSortKey, ..>::_M_insert: FableWin.pdb
//   CThreadedFileManager::CPendingObjectMap::Objects = map<CPendingObjectSortKey, CFileObject*>.
// Shape: STLport 4.6 `insert_unique` verbatim (the landed map<unsigned long, CFileObject*> twin 0x00990AD0) with the
//   key compare less<CPendingObjectSortKey> INLINED here (its out-of-line copy 0x0098F570 is what _M_insert calls): the
//   file's physical sort key, then the file position (both unsigned, `jb`/`ja`), then the object address (`setb`).
//   The iterator `__j` and _M_insert's iterator result share the dead `__v` argument slot.
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
        CFileObject* Object;                   // +0x00
        bool operator<(const CPendingObjectSortKey& o) const
        {
            unsigned long mySortKey = Object->PFile->PhysicalSortKey;
            unsigned long otherSortKey = o.Object->PFile->PhysicalSortKey;
            if (mySortKey < otherSortKey)
                return true;
            if (mySortKey > otherSortKey)
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
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Base_ptr __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    _Self& operator--();
};
// The out-of-line decrement (its own retail row, 0x0098FDD0) is defined in this TU as in retail; noinline keeps it a call
// and lets VC7.1 home the iterator in the dead argument slot.
#pragma auto_inline(off)
template <class _Value, class _Traits> __declspec(noinline)
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{
    _M_node = _Rb_global_inst::_M_decrement(_M_node);
    return *this;
}
#pragma auto_inline(on)
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
protected:
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00990830
public:
    iterator begin() { return iterator(_M_leftmost()); }
    pair<iterator, bool> insert_unique(const value_type& __v);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
typedef CThreadedFileManager::CPendingObjectSortKey _TreeKey;
typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> _TreeValue;
typedef _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey>, allocator<_TreeValue> > _Tree;
template pair<_Tree::iterator, bool> _Tree::insert_unique(const _TreeValue&);
}