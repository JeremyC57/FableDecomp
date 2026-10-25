// std::_Rb_tree<CCharString, pair<const CCharString, SBankRecord>, _Select1st<...>, less<CCharString> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CCharString, 20-byte record> node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AB120, 206 bytes, thiscall `ret 14h` (the hidden iterator result pointer first).
// Catalog name "RBTree_Insert" (descriptive). Callers: insert_unique 0x009AB1F0 (0x009AB266 / 0x009AB2BF) and the hinted
// insert_unique sites 0x009ABB7F..0x009ABD13.
// Callees: CBasicString<char>::operator< @ 0x00429950 (CCharString::operator< inlined), malloc @ 0x00BFEA0E +
// pair<const CCharString, SBankRecord>::pair(const pair&) @ 0x009A9AD0 (thiscall, out of line: the name, four
// CWideString copies and a word) -- _M_create_node inlined with VC7.1's placement null check -- and
// _Rb_global<bool>::_Rebalance @ 0x0042971D.
// Element: pair<const CCharString, 20-byte record of four CWideStrings and a word> (node 0x28; field names not recoverable).
// Shape: STLport 4.6 `_M_insert` verbatim (same source as 0x009AA900 / 0x009AAF70, this chunk).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBasicStringChar {                                    // CBasicString<char> (13 bytes packed; only compared here)
public:
    char* PData;                                            // +0x00
    bool operator<(const CBasicStringChar& other) const;    // 0x00429950
};
struct CCharStringData {                                    // shared string block (Data, then NoRefs at +0x0D)
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
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair<_T1, _T2>& __o);                        // 0x009A9AD0 for this instantiation (out of line)
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

template <class _Value, class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v, _Rb_tree_node_base* __w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                         const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}

}

class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString(const CWideString& other);                  // 0x0099B720
};
struct SBankRecord {                                        // 20-byte mapped value: four wide strings and a word
    CWideString Strings[4];                                 // +0x00
    long        Value;                                      // +0x10
};
typedef std::pair<const CCharString, SBankRecord> SBankEntry;
template std::_Rb_tree<CCharString, SBankEntry, std::_Select1st<SBankEntry>, std::less<CCharString> >::iterator
std::_Rb_tree<CCharString, SBankEntry, std::_Select1st<SBankEntry>, std::less<CCharString> >::_M_insert(
    std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const SBankEntry&, std::_Rb_tree_node_base*);