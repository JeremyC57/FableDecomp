// std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const key, CFileObject*>, _Select1st, less, allocator>::erase(iterator first, iterator last)
//   -- STLport: when [first, last) is [begin(), end()) the tree is cleared, otherwise `erase(first++)` repeats
// Leaf: ?erase@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Select1st@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@U?$less@VCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009905D0, 189 bytes, thiscall, `ret 8` (two by-value iterators).
// Identity: CThreadedFileManager's pending map<CPendingObjectSortKey, CFileObject*> (DeviceQueue[2][7]). Callers: this tree's
//   erase(const key_type&) 0x00990CD0 (equal_range inline over the landed upper/lower bounds 0x0098FCA0 / 0x009901D0, distance
//   0x0098FF50, then this; it is the map erase the landed CThreadedFileManager::RemoveObjectFromPendingMap 0x0098E770 calls)
//   and CThreadedFileManager::CloseAllStreamsForFile 0x0098E7D0 (call 0x0098EA32). Its clear() calls 0x0098FD40, the same
//   _M_erase that 0x0098E716 and the tree's other members call. The catalog's "EraseRange_CRetailTree_009905d0" and the
//   manifest's "LTextTreeWalkThrough::Erase" are descriptive / propagated (FINDINGS chunk 482: invented stand-in class
//   names, not std::_Rb_tree).
// Callees: _M_erase(_Link_type) 0x0098FD40 (thiscall, the root pushed), _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E
//   (__cdecl: the victim and the header's parent/left/right links), free 0x00BFEA14 (STLport's malloc allocator).
// Shape: STLport 4.6 erase(first, last) with clear(), erase(iterator) (rebalance, _Destroy of the trivially destructible
//   value, allocator::deallocate's null test, --_M_node_count) and the iterator's _M_increment (right subtree's leftmost,
//   else climb while a right child) all inline.
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
    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
private:
    void _M_erase(_Link_type __x);             // 0x0098FD40
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
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last)
{
    if (__first == begin() && __last == end())
        clear();
    else
        while (__first != __last)
            erase(__first++);
}

typedef pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> _PendingValue;
typedef _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, _PendingValue, _Select1st<_PendingValue>,
                 less<CThreadedFileManager::CPendingObjectSortKey>, allocator<_PendingValue> > _PendingTree;
template void _PendingTree::erase(_PendingTree::iterator, _PendingTree::iterator);

} // namespace std
