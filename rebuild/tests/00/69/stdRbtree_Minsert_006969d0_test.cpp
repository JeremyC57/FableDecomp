// Behaviour test for 0x006969D0: std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// A copy of the source model and logic (compiled separately from src/006969d0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

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

struct CCPPointerInfo {                                     // the shared count block
    long RefCount;                                          // +0x00
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

namespace NCreatureAction { class CActionEventBase; }

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

typedef std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > CActionEventEntry;
template std::_Rb_tree<CCharString, CActionEventEntry, std::_Select1st<CActionEventEntry>,
                       std::less<CCharString> >::iterator
std::_Rb_tree<CCharString, CActionEventEntry, std::_Select1st<CActionEventEntry>, std::less<CCharString> >::_M_insert(
    std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const CActionEventEntry&, std::_Rb_tree_node_base*);

// ---- test stand-ins ----
static int g_stringCopies = 0, g_rebalances = 0;
static std::_Rb_tree_node_base* g_rebalanceNode = 0;
static std::_Rb_tree_node_base** g_rebalanceRoot = 0;
bool CBasicStringChar::operator<(const CBasicStringChar& other) const { return std::strcmp(PData, other.PData) < 0; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
template <class _Dummy>
void __cdecl std::_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{                                                          // test stand-in: records the call, leaves the tree unbalanced
    ++g_rebalances; g_rebalanceNode = __x; g_rebalanceRoot = &__root;
}

typedef std::_Rb_tree<CCharString, CActionEventEntry, std::_Select1st<CActionEventEntry>, std::less<CCharString> > Tree;
typedef std::_Rb_tree_node<CActionEventEntry> Node;
static CCharStringData g_keys[8];
static char g_text[8][4];
static CCPPointerInfo g_info;
static CActionEventEntry* Entry(int k)
{
    CActionEventEntry* v = (CActionEventEntry*)std::malloc(sizeof(CActionEventEntry));
    const_cast<CCharString&>(v->first).PStringData = &g_keys[k];
    v->second.Data = (NCreatureAction::CActionEventBase*)&g_text[k];
    v->second.Info = &g_info;
    return v;
}
static const char* Key(std::_Rb_tree_node_base* n) { return ((Node*)n)->_M_value_field.first.PStringData->Data.PData; }
static Tree::iterator InsertUnique(Tree& tree, int k)
{                                                          // the insert_unique descent, then _M_insert(0, y, v)
    CActionEventEntry* v = Entry(k);
    std::_Rb_tree_node_base* y = tree._M_header._M_data;
    std::_Rb_tree_node_base* x = y->_M_parent;
    while (x != 0) {
        y = x;
        x = (v->first < ((Node*)x)->_M_value_field.first) ? x->_M_left : x->_M_right;
    }
    Tree::iterator it = tree._M_insert(0, y, *v);
    std::free(v);
    return it;
}
static int g_walked = 0;
static const char* g_order[8];
static void InOrder(std::_Rb_tree_node_base* n)
{
    if (!n) return;
    InOrder(n->_M_left); g_order[g_walked++ & 7] = Key(n); InOrder(n->_M_right);
}

static int Fail(int line) { std::printf("006969d0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    const char* words = "mcxazeqb";
    for (int i = 0; i < 8; ++i) { g_text[i][0] = words[i]; g_text[i][1] = 0; g_keys[i].Data.PData = g_text[i]; }
    g_info.RefCount = 1;
    Tree tree;
    tree._M_header._M_data = (Node*)std::malloc(sizeof(Node));
    tree._M_header._M_data->_M_parent = 0;
    tree._M_header._M_data->_M_left = tree._M_header._M_data->_M_right = tree._M_header._M_data;
    tree._M_node_count = 0;
    Tree::iterator first = InsertUnique(tree, 0);              // "m" becomes root, leftmost and rightmost
    Node* header = tree._M_header._M_data;
    CHECK(header->_M_parent == first._M_node && header->_M_left == first._M_node && header->_M_right == first._M_node);
    CHECK(first._M_node->_M_parent == header && g_rebalances == 1 && g_rebalanceNode == first._M_node);
    CHECK(g_rebalanceRoot == &header->_M_parent && tree._M_node_count == 1);
    for (int k = 1; k < 8; ++k) InsertUnique(tree, k);
    CHECK(tree._M_node_count == 8 && g_rebalances == 8 && g_stringCopies == 8 && g_info.RefCount == 9);
    CHECK(Key(header->_M_left)[0] == 'a' && Key(header->_M_right)[0] == 'z');   // leftmost / rightmost kept
    InOrder(header->_M_parent);
    CHECK(g_walked == 8);
    for (int i = 1; i < 8; ++i) CHECK(std::strcmp(g_order[i - 1], g_order[i]) < 0);
    for (std::_Rb_tree_node_base* n = header->_M_left; n; n = n->_M_left) CHECK(n->_M_left == 0 || n->_M_left->_M_parent == n);
    // x != 0 forces a left link even for a larger key; w != 0 forces a right link
    std::_Rb_tree_node_base* a = header->_M_left;              // "a", no children
    CActionEventEntry* v = Entry(1);                           // "c" > "a"
    Tree::iterator left = tree._M_insert(a, a, *v);
    CHECK(a->_M_left == left._M_node && left._M_node->_M_parent == a && header->_M_left == left._M_node);
    Tree::iterator right = tree._M_insert(0, left._M_node, *v, left._M_node);
    CHECK(left._M_node->_M_right == right._M_node && right._M_node->_M_left == 0 && right._M_node->_M_right == 0);
    CHECK(tree._M_node_count == 10 && header->_M_right != right._M_node);
    std::free(v);
    std::printf("006969d0_TEST PASS\n");
    return 0;
}