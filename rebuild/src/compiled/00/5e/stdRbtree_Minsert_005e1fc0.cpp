// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CCharString, CIVCountedPointer<CAnimComponentBase> > node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1FC0, 256 bytes (0xFC + int3 padding), thiscall `ret 14h` (the hidden
//   iterator result pointer first, then x, y, &v, w).
// Identity: the tree at global 0x013B8C1C that CAnimationEntry::StatRegisterClass (0x005DD670) fills through
//   insert_unique 0x005E2580 (its only two call sites, 0x005E25F6 / 0x005E264F): every registration builds a
//   12-byte counted component object (vtable +0, count +4 = 1) and inserts (CCharString name, counted pointer) --
//   Ego_r.pdb map<CCharString, CIVCountedPointer<CAnimComponentBase> > (node 0x18: base 0x10 + the 8-byte pair).
//   The catalog label "std_set_InsertStringKey" is descriptive.
// Callees: CBasicString<char>::operator< 0x00429950 (CCharString::operator< inlined), malloc 0x00BFEA0E (inline
//   allocate), CCharString copy constructor 0x0099EC30, _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Shape: STLport 4.6 `_M_insert` verbatim (the landed 0x009AB120 model); here the pair copy constructor is the
//   implicit one, so _M_create_node inlines the null-checked placement copy: the out-of-line key copy, then the
//   intrusive pointer copy (`if (P) ++P->RefCount`, count at +4) re-reading &v from its argument slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBasicStringChar {                                    // CBasicString<char> (only compared here)
public:
    char* PData;                                            // +0x00
    bool operator<(const CBasicStringChar& other) const;    // 0x00429950
};
struct CCharStringData {                                    // shared string block (Data first)
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

class CIVCountedPointeeBase {                               // vptr +0, reference count +4
public:
    virtual ~CIVCountedPointeeBase();
    long RefCount;                                          // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase { };

template <class T> class CIVCountedPointer {
public:
    T* Data;                                                // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Data(other.Data) { if (Data) ++Data->RefCount; }
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
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

typedef std::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > CAnimComponentEntry;
template std::_Rb_tree<CCharString, CAnimComponentEntry, std::_Select1st<CAnimComponentEntry>,
                        std::less<CCharString> >::iterator
std::_Rb_tree<CCharString, CAnimComponentEntry, std::_Select1st<CAnimComponentEntry>, std::less<CCharString> >::_M_insert(
    std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const CAnimComponentEntry&, std::_Rb_tree_node_base*);