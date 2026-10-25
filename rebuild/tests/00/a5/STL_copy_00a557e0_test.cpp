// Behaviour test for 0x00A557E0 (source body copied from src/00a557e0.cpp; out-of-line callees stubbed below).
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

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

namespace _STL {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary
struct input_iterator_tag { input_iterator_tag() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00433530
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Container>
class back_insert_iterator {
public:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

struct _Rb_tree_node_base {
    char                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

template <class _Dummy>
class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node);
};

template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};

template <class _Tp> struct _Const_traits {
    typedef _Tp        value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp        value_type;
    typedef _Tp&       reference;
    typedef _Tp*       pointer;
};

// STLport 4.6 tree iterator: set<T>::iterator and ::const_iterator are both the _Const_traits instantiation, so its
// copy constructor is the IMPLICIT one (the user constructor below converts from the _Nonconst_traits iterator).
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&, _Distance*)
{
    for ( ; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, input_iterator_tag(), (int*)0);
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}

template back_insert_iterator<vector<CCharString> > __fastcall copy(_Rb_tree_iterator<CCharString, _Const_traits<CCharString> >,
                                                                     _Rb_tree_iterator<CCharString, _Const_traits<CCharString> >,
                                                                     back_insert_iterator<vector<CCharString> >);
}


// ---- test harness ----
static int g_copies = 0, g_overflow = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }
namespace _STL {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
{
    ++g_overflow;
    unsigned int size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int cap = size * 2 + __fill_len;
    _Tp* block = (_Tp*)malloc(cap * sizeof(_Tp));
    if (size) memcpy(block, this->_M_start, size * sizeof(_Tp));
    for (unsigned int i = 0; i < __fill_len; ++i) memcpy(block + size + i, &__x, sizeof(_Tp));
    this->_M_start = block;
    this->_M_finish = block + size + __fill_len;
    this->_M_end_of_storage._M_data = block + cap;
    (void)__position;
}
}
typedef _STL::_Rb_tree_node<CCharString> TNode;
typedef _STL::_Rb_tree_iterator<CCharString, _STL::_Const_traits<CCharString> > TIter;
typedef _STL::vector<CCharString> TVec;
int main()
{
    static double nodeRaw[(4 * sizeof(TNode)) / sizeof(double) + 1];   // 0 = header, 1..3 = A, B, C (in-order)
    memset(nodeRaw, 0, sizeof nodeRaw);
    TNode* nodes = (TNode*)nodeRaw;
    TNode* h = &nodes[0]; TNode* a = &nodes[1]; TNode* b = &nodes[2]; TNode* c = &nodes[3];
    h->_M_parent = b; h->_M_left = a; h->_M_right = c;
    b->_M_parent = h; b->_M_left = a; b->_M_right = c;
    a->_M_parent = b; c->_M_parent = b;
    a->_M_value_field.m_pData = (void*)0x111; b->_M_value_field.m_pData = (void*)0x222; c->_M_value_field.m_pData = (void*)0x333;
    static double storageRaw[(2 * sizeof(CCharString)) / sizeof(double) + 1];
    CCharString* storage = (CCharString*)storageRaw;
    TVec v;
    v._M_start = storage; v._M_finish = storage; v._M_end_of_storage._M_data = storage + 2;
    TIter first, last;
    first._M_node = a; last._M_node = h;
    _STL::back_insert_iterator<TVec> out = _STL::copy(first, last, _STL::back_insert_iterator<TVec>(v));
    int ok = 1;
    if (out.container != &v) ok = 0;
    if (v._M_finish - v._M_start != 3) ok = 0;
    else if (v._M_start[0].m_pData != (void*)0x111 || v._M_start[1].m_pData != (void*)0x222 || v._M_start[2].m_pData != (void*)0x333) ok = 0;
    if (g_copies != 2 || g_overflow != 1) ok = 0;
    TIter e1, e2; e1._M_node = h; e2._M_node = h;     // empty range: nothing appended
    TVec w; w._M_start = w._M_finish = storage; w._M_end_of_storage._M_data = storage + 2;
    _STL::back_insert_iterator<TVec> out2 = _STL::copy(e1, e2, _STL::back_insert_iterator<TVec>(w));
    if (out2.container != &w || w._M_finish != w._M_start || g_copies != 2) ok = 0;
    printf("00a557e0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}