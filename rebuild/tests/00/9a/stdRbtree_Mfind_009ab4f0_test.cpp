// behaviour test for 0x009AB4F0 std::_Rb_tree::_M_find (map<CCharString, NBankFileManager::CContainedBank>): every stored key is found as its own node, missing keys (before, between, after) and an empty tree give the header, a null-data key sorts first, an equal data pointer short-cuts the string compare, equal text in a different buffer still matches
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
inline void* operator new(size_t, void* p, int) { return p; }
template <class T> class CBasicString {
public:
    T* m_pData;
    long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
struct CCharStringData {
    CBasicString<char> Data;
};
class CCharString {
public:
    CCharStringData* PStringData;
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
namespace stlp_test {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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
template <class _Key, class _Value, class _KeyOfValue, class _Compare>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Link_type _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)_M_header->_M_parent; }
    _Link_type _M_find(const _Key& __k) const
    {
        _Link_type __y = _M_header;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == _M_header || _M_key_compare(__k, _S_key(__y)))
            __y = _M_header;
        return __y;
    }
};
}
static char gText[8][8] = { "apple", "banana", "cherry", "date", "fig", "carrot", "zebra", "aaa" };
static CCharStringData gData[8];
static CCharStringData gFig2;                  // "fig" text in a second buffer
typedef stlp_test::pair<const CCharString, int> TValue;       // stand-in mapped value (`NBankFileManager::CContainedBank` in retail): _M_find reads only the key
typedef stlp_test::_Select1st<TValue> TKeyOfValue;
static TValue MakeValue(const CCharString& k, int tag) { return TValue(k, tag); }
static int Tag(const stlp_test::_Rb_tree_node<TValue>* n) { return n->_M_value_field.second; }

typedef stlp_test::_Rb_tree_node<TValue> TNode;
typedef stlp_test::_Rb_tree<CCharString, TValue, TKeyOfValue, stlp_test::less<CCharString> > TTree;
static CCharString Key(int i)
{
    CCharString s;
    s.PStringData = i < 0 ? 0 : &gData[i];
    return s;
}
static TNode* MkNode(const CCharString& k, int tag)
{
    TNode* n = (TNode*)std::malloc(sizeof(TNode));
    n->_M_color = false;
    n->_M_parent = n->_M_left = n->_M_right = 0;
    new (&n->_M_value_field, 0) TValue(MakeValue(k, tag));
    return n;
}
static void Link(TNode* parent, TNode* left, TNode* right)
{
    parent->_M_left = left;
    parent->_M_right = right;
    if (left) left->_M_parent = parent;
    if (right) right->_M_parent = parent;
}
static int Check()
{
    for (int i = 0; i < 8; ++i) gData[i].Data.m_pData = gText[i];
    static char figCopy[4] = { 'f', 'i', 'g', 0 };
    gFig2.Data.m_pData = figCopy;
    TTree t;
    t._M_header = MkNode(Key(0), -1);
    t._M_header->_M_parent = 0;
    t._M_node_count = 0;
    if (t._M_find(Key(3)) != t._M_header) return 1;                  // empty tree: end()
    // in order: apple banana cherry date fig;  date at the root
    TNode* apple = MkNode(Key(0), 0);
    TNode* banana = MkNode(Key(1), 1);
    TNode* cherry = MkNode(Key(2), 2);
    TNode* date = MkNode(Key(3), 3);
    TNode* fig = MkNode(Key(4), 4);
    Link(date, banana, fig);
    Link(banana, apple, cherry);
    t._M_header->_M_parent = date;
    date->_M_parent = t._M_header;
    t._M_node_count = 5;
    TNode* want[5] = { apple, banana, cherry, date, fig };
    for (int i = 0; i < 5; ++i) {
        TNode* n = t._M_find(Key(i));
        if (n != want[i] || Tag(n) != i) return 2;
    }
    if (t._M_find(Key(5)) != t._M_header) return 3;                  // "carrot": between banana and cherry
    if (t._M_find(Key(6)) != t._M_header) return 4;                  // "zebra": after every key
    if (t._M_find(Key(7)) != t._M_header) return 5;                  // "aaa": before every key
    if (t._M_find(Key(-1)) != t._M_header) return 6;                 // null data sorts first, not stored yet
    CCharString figOther;
    figOther.PStringData = &gFig2;                                    // same text, another buffer
    if (t._M_find(figOther) != fig) return 7;
    TNode* nul = MkNode(Key(-1), 9);                                  // null-data key as the leftmost node
    Link(apple, nul, 0);
    if (t._M_find(Key(-1)) != nul || Tag(nul) != 9) return 8;
    if (t._M_find(Key(0)) != apple) return 9;

    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("009ab4f0_TEST FAIL %d\n", r); return 1; }
    std::printf("009ab4f0_TEST PASS\n");
    return 0;
}