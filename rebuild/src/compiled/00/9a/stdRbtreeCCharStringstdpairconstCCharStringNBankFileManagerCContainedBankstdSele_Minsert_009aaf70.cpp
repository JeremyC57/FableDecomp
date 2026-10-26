// std::_Rb_tree<CCharString, std::pair<const CCharString, NBankFileManager::CContainedBank>, std::_Select1st<..>, std::less<CCharString>, std::allocator<..> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- NBankFileManager::CRetailBank::ContainedBanks (map<CCharString, CContainedBank>) node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AAF70, 173 bytes, thiscall `ret 14h` (the hidden iterator result pointer first).
// Identity (sweep3, family pass 1 "invented SRecord20"): _M_create_node 0x009AA9E0 copies the value as a CCharString
//   copy + five dwords: Ego_r.pdb's NBankFileManager::CContainedBank {DataVersion, EntryCount, InfoOffset, InfoSize,
//   Alignment} (0x14 bytes, all ulong), and CRetailBank::ContainedBanks is the only Ego_r member typed
//   map<CCharString, NBankFileManager::CContainedBank>; the unreferenced STLport _Construct 0x009AA520 beside it
//   copies the same pair. The old SRecord20 was an invented stand-in for CContainedBank.
// Callers: nine call sites 0x009AB096..0x009ABA73 (insert_unique and the hinted insert_unique of the same tree).
// Callees: CBasicString<char>::operator< @ 0x00429950 (CCharString::operator< inlined), _M_create_node @ 0x009AA9E0
//   (out of line: malloc(0x28) + the pair copy; RENAMES row this pass), _Rb_global<bool>::_Rebalance @ 0x0042971D.
// Shape: STLport 4.6 `_tree.c` `_M_insert` verbatim (right-insert arm first, left-insert arm second; with _M_create_node
//   out of line the zero links are immediate stores); access per STLport 4.6 `_tree.h` (sweep2 decision 1d).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBasicStringChar {                                    // CBasicString<char> (13 bytes packed; only compared here)
public:
    char* PData;
    bool operator<(const CBasicStringChar& other) const;    // 0x00429950
};
class CCharStringData { public:                                    // shared string block (Data, then NoRefs at +0x0D)
    CBasicStringChar Data;                                  // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
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
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class _Tp> struct _Nonconst_traits { };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
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
protected:
    _STLP_alloc_proxy<_Link_type, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
    _Link_type _M_create_node(const value_type& __x);       // 0x009AA9E0 for this instantiation (out of line)
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
};

// STLport 4.6 _tree.c, verbatim.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                                 const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && // If w != 0, the remainder fails to false
         (__x != 0 ||     // If x != 0, the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;               // also makes _M_leftmost() = __z
                                          //    when __y == _M_header
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;   // maintain _M_leftmost() pointing to min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;  // maintain _M_rightmost() pointing to max node
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

namespace NBankFileManager {
class CContainedBank {                                      // 0x14 bytes (Ego_r.pdb NBankFileManager::CContainedBank), copied as a block
public:
    unsigned long DataVersion;                              // +0x00
    unsigned long EntryCount;                               // +0x04
    unsigned long InfoOffset;                               // +0x08
    unsigned long InfoSize;                                 // +0x0C
    unsigned long Alignment;                                // +0x10
};
}

typedef std::pair<const CCharString, NBankFileManager::CContainedBank> _TreeValue;
typedef std::_Rb_tree<CCharString, _TreeValue, std::_Select1st<_TreeValue>, std::less<CCharString>, std::allocator<_TreeValue> > _Tree;
template _Tree::iterator _Tree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const _TreeValue&, std::_Rb_tree_node_base*);
