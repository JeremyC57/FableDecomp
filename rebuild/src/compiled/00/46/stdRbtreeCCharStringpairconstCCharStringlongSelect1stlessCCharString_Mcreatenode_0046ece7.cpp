// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_create_node(const value_type& v) -- STLport: malloc one 0x18-byte map node and copy-construct the (CCharString, long) value into it
// Leaf: ?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@std@@@2@ABU?$pair@$$CBVCCharString@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046ECE7, 46 bytes, `ret 4` (thiscall: the value by reference; the empty allocator `this` is unused).
// Callees: malloc @ 0x00BFEA0E, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Identity: the catalog's "Alloc" is behavioural. Callers: _Rb_tree::_M_copy @ 0x0046EC57 (landed as "CRetailTree_0046ece7::Copy",
//   twice, inlined _M_clone_node) and _Rb_tree::_M_insert @ 0x004759AD (twice, one per branch); 0x004759AD's tree compares
//   CCharString keys (insert_unique 0x004758E3, lower/upper_bound 0x0047547D/0x004754C6). Node = {colour, parent, left,
//   right} + the 8-byte value at +0x10; the mapped word is copied as a plain dword (a long-sized member).
// Model: STLport 4.x `_M_create_node` = allocate(1) + _Construct(&tmp->_M_value_field, x) (the placement-new null test);
//   pair's member-wise copy constructor runs the CCharString copy constructor, then the dword copy. Exceptions off (no
//   unwind frame in retail). CCharString is local (no engine header): one PStringData pointer.
// Shape: `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer. In this size TU pair's copy
//   constructor must be `__forceinline` (plain inline leaves it out of line: 33v46).
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct PStringData;
class CCharString {
public:
    PStringData* PData;                             // +0x00
    CCharString(const CCharString& other);          // 0x0099EC30
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    __forceinline pair(const pair<T1, T2>& other) : first(other.first), second(other.second) { }
};

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> struct allocator { };

struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};
template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class Key, class Value, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    __forceinline _Link_type _M_allocate_node() { return static_cast<_Link_type>(malloc(sizeof(_Rb_tree_node<Value>))); }
protected:
    _Link_type _M_create_node(const Value& x);
};

template <class Key, class Value, class Alloc>
typename _Rb_tree<Key, Value, Alloc>::_Link_type _Rb_tree<Key, Value, Alloc>::_M_create_node(const Value& x)
{
    _Link_type tmp = _M_allocate_node();
    _Construct(&tmp->_M_value_field, x);
    return tmp;
}

typedef pair<const CCharString, long> _MapValue;
template _Rb_tree<CCharString, _MapValue, allocator<_MapValue> >::_Link_type
    _Rb_tree<CCharString, _MapValue, allocator<_MapValue> >::_M_create_node(const _MapValue& x);
}