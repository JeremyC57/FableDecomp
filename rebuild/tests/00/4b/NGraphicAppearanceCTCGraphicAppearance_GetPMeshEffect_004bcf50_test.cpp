// Behaviour test for 0x004BCF50 (source body copied from src/004bcf50.cpp; out-of-line callees stubbed below).
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
class CEngineMeshEffect {
public:
    virtual void v000();
    long RefCount;                                          // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline CIVCountedPointer() : Object(0) {}
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() { } };
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
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
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
    _Rb_tree_iterator& operator++();                        // 0x004C2560 (out of line in this TU)
    _Rb_tree_iterator operator++(int)
    {
        _Rb_tree_iterator __tmp = *this;
        _M_node = _Rb_global_inst::_M_increment(_M_node);
        return __tmp;
    }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    _Node* _M_leftmost() const { return (_Node*)_M_header._M_data->_M_left; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }
};
template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    struct CMeshEffectPriorityEntry {
        CIVCountedPointer<CEngineMeshEffect> PEffect;      // +0x0
        long          Priority;                             // +0x4 EAppearanceMeshEffectPriority
        bool          EffectsCarriedItems;                  // +0x8
        bool          EffectsWeapons;                       // +0x9
        bool          InheritedFromCarrier;                 // +0xa
    };
    typedef _STL::map<void*, CMeshEffectPriorityEntry> CMeshEffectMap;

    void*          __vftable;                               // +0x00 (CTCBase)
    void*          ParentThing;                             // +0x04
    unsigned char  _pad08[0x0c - 0x08];
    CMeshEffectMap MeshEffects;                             // +0x0c
    CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect();
};

CIVCountedPointer<CEngineMeshEffect> CTCGraphicAppearance::GetPMeshEffect()
{
    CMeshEffectMap::iterator it = MeshEffects.begin();
    CMeshEffectMap::iterator last = MeshEffects.end();
    if (it == last)
        return CIVCountedPointer<CEngineMeshEffect>();
    CMeshEffectPriorityEntry* best = &it->second;
    ++it;
    for (CMeshEffectMap::iterator i = it; i != last; i++)
        if (i->second.Priority > best->Priority)
            best = &i->second;
    return best->PEffect;
}

}


// ---- test harness ----
void CEngineMeshEffect::v000() { }
namespace _STL {
template <class _Value, class _Traits>
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator++()
{
    _M_node = _Rb_global_inst::_M_increment(_M_node);
    return *this;
}
}
using NGraphicAppearance::CTCGraphicAppearance;
typedef _STL::_Rb_tree_node<CTCGraphicAppearance::CMeshEffectMap::value_type> TNode;
static TNode* Node(double* raw) { return (TNode*)raw; }
int main()
{
    static double r0[sizeof(TNode) / sizeof(double) + 1], r1[sizeof(TNode) / sizeof(double) + 1],
                  r2[sizeof(TNode) / sizeof(double) + 1], r3[sizeof(TNode) / sizeof(double) + 1];
    TNode* h = Node(r0); TNode* a = Node(r1); TNode* b = Node(r2); TNode* c = Node(r3);
    memset(r0, 0, sizeof r0); memset(r1, 0, sizeof r1); memset(r2, 0, sizeof r2); memset(r3, 0, sizeof r3);
    h->_M_parent = b; h->_M_left = a; h->_M_right = c;          // in-order a, b, c
    b->_M_parent = h; b->_M_left = a; b->_M_right = c;
    a->_M_parent = b; c->_M_parent = b;
    CEngineMeshEffect ea, eb, ec;
    ea.RefCount = 1; eb.RefCount = 1; ec.RefCount = 1;
    a->_M_value_field.second.PEffect.Object = &ea; a->_M_value_field.second.Priority = 2;
    b->_M_value_field.second.PEffect.Object = &eb; b->_M_value_field.second.Priority = 5;
    c->_M_value_field.second.PEffect.Object = &ec; c->_M_value_field.second.Priority = 5;   // tie: the earlier entry stays
    CTCGraphicAppearance app;
    app.MeshEffects._M_t._M_header._M_data = h;
    int ok = 1;
    CIVCountedPointer<CEngineMeshEffect> r = app.GetPMeshEffect();
    if (r.Object != &eb || eb.RefCount != 2 || ea.RefCount != 1 || ec.RefCount != 1) ok = 0;
    c->_M_value_field.second.Priority = 9;
    CIVCountedPointer<CEngineMeshEffect> r2b = app.GetPMeshEffect();
    if (r2b.Object != &ec || ec.RefCount != 2) ok = 0;
    b->_M_value_field.second.PEffect.Object = 0; b->_M_value_field.second.Priority = 99;   // null best: no count
    CIVCountedPointer<CEngineMeshEffect> r3b = app.GetPMeshEffect();
    if (r3b.Object != 0) ok = 0;
    h->_M_left = h; h->_M_parent = 0; h->_M_right = h;            // empty map
    CIVCountedPointer<CEngineMeshEffect> r4 = app.GetPMeshEffect();
    if (r4.Object != 0) ok = 0;
    printf("004bcf50_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}