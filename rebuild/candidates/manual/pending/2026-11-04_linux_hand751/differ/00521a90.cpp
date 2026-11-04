// CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name) -- NPC name map: create the entry with an empty script name when the uid is new, then store the display name
// Grade: DIFFER(78v78) -- dl=7, first operator[] key lea in ecx (retail eax), second's name/key in edx/eax (retail ecx/edx); reference local, CCharString temp, const operator[], same-TU operator[] stand-in, != form unchanged or worse
// Leaf: ?SetNameForThing@CThingManager@@QAEX_KABVCWideString@@@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};

template <class _Pair> struct _Select1st { };  // STLport 4.6 _function.h (KeyOfValue of map's tree)
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int        _M_node_count;                      // +0x04
    _Compare            _M_key_compare;                     // +0x08

private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;     // 0x00527E00 for the NPC name map
public:
    template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    iterator end() { return _M_t.end(); }
    _Tp& operator[](const _Key& __k);                     // 0x0052B8B0
};
}

class CThingManager {
public:
    void SetNameForThing(unsigned __int64 uid, const CWideString& name);
    unsigned char _Before[0xCC];                            // +0x00 .. +0xCB
    std::map<unsigned __int64, std::pair<CCharString, CWideString> > NPCNameMap;   // +0xCC
};

void CThingManager::SetNameForThing(unsigned __int64 uid, const CWideString& name)
{
    std::map<unsigned __int64, std::pair<CCharString, CWideString> >::iterator it = NPCNameMap.find(uid);
    if (it == NPCNameMap.end())
        NPCNameMap[uid].first = "";
    NPCNameMap[uid].second = name;
}
