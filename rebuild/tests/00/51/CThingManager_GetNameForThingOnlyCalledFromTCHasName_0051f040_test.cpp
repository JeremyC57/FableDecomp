// Behaviour test for CThingManager::GetNameForThing_OnlyCalledFromTCHasName(unsigned __int64) (0x0051F040): the
// result is a copy of the display name (the pair's second string) stored for the UID in the NPC name map; the map is
// unchanged and the full 64-bit UID selects the entry.
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 TUID;
#else
typedef unsigned long long TUID;
#endif

static int gCopies = 0;

class CCharString {
public:
    const char* Text;
    CCharString(const char* t) : Text(t) { }
};

class CWideString {
public:
    const char* Text;                                   // narrow stand-in for the shared wide text
    CWideString(const char* t) : Text(t) { }
    CWideString(const CWideString& o) : Text(o.Text) { ++gCopies; }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
};

struct _Rb_tree_node_base {
    _Rb_tree_node_base* _M_next;                        // the test keeps the nodes in a list
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
    _Rb_tree_node(const _Value& v) : _M_value_field(v) { }
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};

template <class _Key, class _Value>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base  _M_header_node;
    _Rb_tree_node_base* _M_header;
    _Rb_tree() : _M_header(&_M_header_node) { _M_header_node._M_next = 0; }
    _Rb_tree_node_base* _M_find(const _Key& __k) const
    {
        for (_Rb_tree_node_base* n = _M_header->_M_next; n; n = n->_M_next)
            if (((_Rb_tree_node<_Value>*)n)->_M_value_field.first == __k)
                return n;
        return _M_header;
    }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
};

template <class _Key, class _Tp>
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
};
}

typedef _STL::pair<CCharString, CWideString> TName;
typedef _STL::map<TUID, TName>::value_type TEntry;

class CThingManager {
public:
    _STL::map<TUID, TName> NPCNameMap;
    CWideString GetNameForThing_OnlyCalledFromTCHasName(TUID uid)
    {
        return (*NPCNameMap.find(uid)).second.second;
    }
};

int main()
{
    CThingManager manager;
    TUID lowUid = 1;
    TUID highUid = ((TUID)1 << 32) | 1;     // same low word, different high word
    _STL::_Rb_tree_node<TEntry> a(TEntry(lowUid, TName(CCharString("GUARD_01"), CWideString("Guard"))));
    _STL::_Rb_tree_node<TEntry> b(TEntry(highUid, TName(CCharString("TRADER_02"), CWideString("Trader"))));
    manager.NPCNameMap._M_t._M_header->_M_next = &a;
    a._M_next = &b;
    b._M_next = 0;
    gCopies = 0;
    CWideString n1 = manager.GetNameForThing_OnlyCalledFromTCHasName(highUid);
    CWideString n2 = manager.GetNameForThing_OnlyCalledFromTCHasName(lowUid);
    int fails = 0;
    if (std::strcmp(n1.Text, "Trader") != 0 || std::strcmp(n2.Text, "Guard") != 0) ++fails;
    if (gCopies < 2) ++fails;                              // each call copies the stored name
    if (std::strcmp(b._M_value_field.second.second.Text, "Trader") != 0) ++fails;
    if (std::strcmp(a._M_value_field.second.first.Text, "GUARD_01") != 0) ++fails;
    if (fails) { std::printf("0051f040_TEST FAIL\n"); return 1; }
    std::printf("0051f040_TEST PASS\n");
    return 0;
}