// Behaviour test for 0x006E5990 (source body copied from src/006e5990.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

class CScriptConversation {
public:
    unsigned char _data[0x2c];                          // retail STLport layout
    ~CScriptConversation();                             // 0x004AE0E0
    void OnCleanup(bool flag);                          // 0x006E5690
};

namespace std {
template <class _Tp> struct less { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const _Key& __k);                                   // 0x006E6990 (out of line)
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};
template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
};
}

class CScriptConversationManager {
public:
    std::map<long, CScriptConversation> Conversations; // +0x00
    long             NextConversationIndex;             // +0x0c
    bool             AlwaysShowText;                    // +0x10
    void RemoveConversation(long conversation, bool flag);
};

void CScriptConversationManager::RemoveConversation(long conversation, bool flag)
{
    std::map<long, CScriptConversation>::iterator it;
    it = Conversations.find(conversation);
    if (it != Conversations.end()) {
        it->second.OnCleanup(flag);
        Conversations.erase(it);
    }
}


// ---- test harness ----
static int g_dtor = 0, g_cleanup = 0, g_cleanupFlag = -1, g_rebalance = 0;
static void* g_cleanupThis = 0;
CScriptConversation::~CScriptConversation() { ++g_dtor; }
void CScriptConversation::OnCleanup(bool flag) { ++g_cleanup; g_cleanupFlag = flag ? 1 : 0; g_cleanupThis = this; }
namespace std {
template <> _Rb_tree_node_base* __cdecl _Rb_global<bool>::_Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                                              _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost)
{
    ++g_rebalance;
    _Rb_tree_node_base* header = __root->_M_parent;     // the only node is the root: unlink it
    if (__root == __z) { __root = 0; __leftmost = header; __rightmost = header; }
    return __z;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::find(const _Key& __k)
{
    _Rb_tree_node_base* __x = _M_header._M_data->_M_parent;
    while (__x != 0) {
        _Key key = ((_Link_type)__x)->_M_value_field.first;
        if (key == __k) return iterator((_Link_type)__x);
        __x = __k < key ? __x->_M_left : __x->_M_right;
    }
    return end();
}
}
typedef std::_Rb_tree_node<std::pair<const long, CScriptConversation> > TNode;
int main()
{
    static double hraw[sizeof(TNode) / sizeof(double) + 1];
    memset(hraw, 0, sizeof hraw);
    TNode* h = (TNode*)hraw;
    TNode* n = (TNode*)malloc(sizeof(TNode));
    memset(n, 0, sizeof(TNode));
    *(long*)&n->_M_value_field.first = 5;
    h->_M_parent = n; h->_M_left = n; h->_M_right = n; n->_M_parent = h;
    static double mraw[sizeof(CScriptConversationManager) / sizeof(double) + 1];
    memset(mraw, 0, sizeof mraw);
    CScriptConversationManager* m = (CScriptConversationManager*)mraw;
    m->Conversations._M_t._M_header._M_data = h;
    m->Conversations._M_t._M_node_count = 1;
    int ok = 1;
    m->RemoveConversation(7, true);                      // missing: nothing happens
    if (g_cleanup != 0 || g_dtor != 0 || g_rebalance != 0 || m->Conversations._M_t._M_node_count != 1) ok = 0;
    m->RemoveConversation(5, true);
    if (g_cleanup != 1 || g_cleanupFlag != 1 || g_cleanupThis != (void*)&n->_M_value_field.second) ok = 0;
    if (g_rebalance != 1 || g_dtor != 1 || m->Conversations._M_t._M_node_count != 0 || h->_M_parent != 0) ok = 0;
    printf("006e5990_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}