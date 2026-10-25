// std::_Rb_tree::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert for CThreadedFileManager's pending-object map (map<CPendingObjectSortKey, CFileObject*>): begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Select1st@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@U?$less@VCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00990DC0, 624 bytes (+ int3 padding), thiscall `ret 0Ch` (hidden iterator result,
//   position, v).
// Identity: the catalog's "map_operator_square_bracket" is descriptive: this is the hinted insert that the map's
//   operator[] (landed STLmap_operator_00991350: callers 0x00991304 / 0x009913B6) calls. FableWin.pdb
//   CThreadedFileManager::CPendingObjectMap::Objects = map<CPendingObjectSortKey, CFileObject*>.
// Callees (all this instantiation): less<CPendingObjectSortKey>::operator() 0x0098F570 (landed, out of line here:
//   thiscall on _M_key_compare +8), iterator operator++ 0x0098F9D0 / operator-- 0x0098FDD0 (landed, out of line,
//   thiscall on the iterator), _M_insert 0x00990830 (landed), insert_unique(v) 0x00990900 (this chunk).
// Model: STLport 4.6 `_tree.c` insert_unique(position, v) verbatim, the landed map<unsigned long,float> twin 0x00AD5030;
//   `__before` / `__after` live in the dead `position` / hidden slots and `__comp_v_pos` in the dead `v` slot.
//   operator++ / operator-- and the key compare carry same-TU `__declspec(noinline)` bodies standing in for the retail
//   copies: with the compare declaration-only VC7.1 keeps a second copy of the `insert_unique(v).first` tail and swaps
//   ebx/ebp (v / the node key); with the same-TU body (the less<CPendingObjectSortKey> order of the landed 0x0098F570:
//   physical sort key, file position, object address) the tails merge as in retail.
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> struct less {
    __declspec(noinline) bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }   // 0x0098F570
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

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

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    __declspec(noinline) _Self& operator++() { _M_increment(); return *this; }   // 0x0098F9D0
    __declspec(noinline) _Self& operator--()                                       // 0x0098FDD0
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
        return *this;
    }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00990830
    pair<iterator, bool> insert_unique(const value_type& __x);                                     // 0x00990900
    iterator insert_unique(iterator __position, const value_type& __x);
    unsigned int size() const { return _M_node_count; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v)
{
    if (__position._M_node == _M_header._M_data->_M_left) {   // begin()
        if (size() <= 0)
            return insert_unique(__v).first;
        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if (__comp_pos_v == false)
                return __position;
            iterator __after = __position;
            ++__after;
            if (__after._M_node == _M_header._M_data)
                return _M_insert(0, __position._M_node, __v, __position._M_node);
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else
                return insert_unique(__v).first;
        }
    }
    else if (__position._M_node == _M_header._M_data) {   // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, __position._M_node);
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;
        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));
        if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        }
        else {
            iterator __after = __position;
            ++__after;
            bool __comp_pos_v = !__comp_v_pos;
            if (!__comp_v_pos)
                __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if ((!__comp_v_pos) && __comp_pos_v &&
                (__after._M_node == _M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}
}   // namespace std

typedef std::pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> TPendingValue;
typedef std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, TPendingValue, std::_Select1st<TPendingValue>,
                      std::less<CThreadedFileManager::CPendingObjectSortKey>, std::allocator<TPendingValue> > TPendingTree;
template TPendingTree::iterator TPendingTree::insert_unique(TPendingTree::iterator, const TPendingValue&);