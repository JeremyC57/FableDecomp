// Behaviour test for 0x00A608F0 _STL::_Rb_tree<CWideString, pair<const CWideString, CEntry>, ...>::_M_find(const CWideString&) const
#include <cstdio>

class CWideString {
public:
    const wchar_t* PStringData;
    bool operator<(const CWideString& other) const          // 0x0099AF70 stand-in: ordinal wide compare
    {
        const wchar_t* a = PStringData;
        const wchar_t* b = other.PStringData;
        while (*a != 0 && *a == *b) { ++a; ++b; }
        return *a < *b;
    }
};
class CChecksumCacheFile {
public:
    class CEntry {
    public:
        unsigned long Data[8];
    };
};

namespace _STL {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};
template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return __x->_M_value_field.first; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_find(const _Key& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
};
}   // namespace _STL

typedef _STL::pair<CWideString, CChecksumCacheFile::CEntry> CValue;
typedef _STL::_Rb_tree_node<CValue> CNode;
typedef _STL::_Rb_tree<CWideString, CValue, _STL::less<CWideString> > CTree;

static void Set(CNode& n, const wchar_t* key, CNode* parent, CNode* left, CNode* right)
{
    n._M_value_field.first.PStringData = key;
    n._M_parent = parent;
    n._M_left = left;
    n._M_right = right;
}
static CWideString Key(const wchar_t* text) { CWideString k; k.PStringData = text; return k; }

// a fixed five-key search tree: dog (root) / cat (ant) / emu (fox), header left = ant, right = fox
static CNode g_header, g_dog, g_cat, g_emu, g_ant, g_fox;
static void Build(CTree& t)
{
    Set(g_header, L"", &g_dog, &g_ant, &g_fox);
    Set(g_dog, L"dog", &g_header, &g_cat, &g_emu);
    Set(g_cat, L"cat", &g_dog, &g_ant, 0);
    Set(g_emu, L"emu", &g_dog, 0, &g_fox);
    Set(g_ant, L"ant", &g_cat, 0, 0);
    Set(g_fox, L"fox", &g_emu, 0, 0);
    t._M_header._M_data = &g_header;
    t._M_node_count = 5;
}

int main()
{
    CTree t;
    Build(t);
    bool ok = t._M_find(Key(L"dog")) == &g_dog && t._M_find(Key(L"ant")) == &g_ant && t._M_find(Key(L"fox")) == &g_fox &&
              t._M_find(Key(L"cat")) == &g_cat && t._M_find(Key(L"cow")) == &g_header &&      // missing keys: end()
              t._M_find(Key(L"zebra")) == &g_header && t._M_find(Key(L"a")) == &g_header;
    if (!ok) { std::printf("00a608f0_TEST FAIL\n"); return 1; }
    std::printf("00a608f0_TEST PASS\n");
    return 0;
}