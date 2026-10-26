// std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const key, CFileObject*>, _Select1st, less, allocator>::_M_erase(_Link_type x)
//   -- STLport: free a subtree without rebalancing (recurse right, keep left, destroy + deallocate the node, continue left)
// Leaf: ?_M_erase@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098FD40, 51 bytes, thiscall, `ret 4`.
// Identity: the _M_erase of CThreadedFileManager's pending map<CPendingObjectSortKey, CFileObject*>: called by this tree's
//   erase(first, last) 0x009905D0 (its inline clear()), by the inlined first level of the same walk at 0x0098E710 in
//   CThreadedFileManager code, and by the three tree destructors 0x009904F0 / 0x00990BD0 / 0x00991280 (and 0x009900C2);
//   it recurses into itself (0x0098FD56). The value pair<const CPendingObjectSortKey, CFileObject*> is trivially destroyed,
//   so the node goes straight to free (the allocator's null test folds away inside the loop). The catalog's
//   "_unattributed::FreeTree_CTreeOwner_0098fd40" and the manifest's "LTextTreeWalkThrough::BuildTreeArray" are
//   descriptive / propagated (FINDINGS chunk 482: these tree helpers are std::_Rb_tree members).
// Callees: free 0x00BFEA14 (STLport's malloc allocator), itself.
// Shape: STLport 4.6 `_M_erase`: `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
//   _M_header.deallocate(x, 1); x = y; }`.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

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
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x00
    _Rb_tree_node_base* _M_parent;             // +0x04
    _Rb_tree_node_base* _M_left;               // +0x08
    _Rb_tree_node_base* _M_right;              // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                     // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                             _Rb_tree_node_base*& __leftmost,
                                                             _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};

inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self operator++(int) { _Self __tmp = *this; _M_increment(); return __tmp; }
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, allocator<_Rb_tree_node<_Tp> > > _M_header;     // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;                // +0x04
    _Compare     _M_key_compare;               // +0x08
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Link_type _S_left(_Base_ptr __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Base_ptr __x) { return (_Link_type)__x->_M_right; }
private:
    void _M_erase(_Link_type __x);             // 0x0098FD40 (this function)
public:
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    void erase(iterator __first, iterator __last);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> _PendingValue;
typedef _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, _PendingValue, _Select1st<_PendingValue>,
                 less<CThreadedFileManager::CPendingObjectSortKey>, allocator<_PendingValue> > _PendingTree;
template void _PendingTree::_M_erase(_PendingTree::_Link_type);

} // namespace std
