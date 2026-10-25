// Test for 0x0062ED40 vector<CIntelligentPointer<CThing> >::vector(const vector&): the copy gets x.size() fresh
// elements, each copy-constructed from x's element in order, and finish = start + size.
#include <stdio.h>
#include <stdlib.h>
inline void* operator new(size_t, void* p) { return p; }
class CBaseObject;
class CBaseObjectPointer;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

namespace std {

template <class _Tp> void __fastcall _Construct(_Tp* __p, const _Tp& __val);                  // 0x004AA030

struct __false_type { __false_type() { } };

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a);  // 0x0062ED90
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    vector(const vector& __x);
};

}
static int g_copies, g_base;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
typedef CIntelligentPointer<CThing> P;
namespace std {
template <> void __fastcall _Construct(P* __p, const P& __val) { __p->PData = __val.PData; ++g_copies; }
template <>
_Vector_base<P, allocator<P> >::_Vector_base(unsigned int __n, const allocator<P>& __a)
    : _M_end_of_storage(__a, 0)
{
    ++g_base;
    _M_start = (P*)malloc(__n * sizeof(P) + 1);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}
}
int main()
{
    P src[3];
    for (int k = 0; k < 3; ++k) src[k].PData = (CBaseObjectPointer*)(0x40 + k);
    std::vector<P>* x = (std::vector<P>*)malloc(sizeof(std::vector<P>));
    x->_M_start = src; x->_M_finish = src + 3; x->_M_end_of_storage._M_data = src + 3;
    std::vector<P>* y = (std::vector<P>*)malloc(sizeof(std::vector<P>));
    new (y) std::vector<P>(*x);
    if (g_base != 1 || g_copies != 3) return 1;
    if (y->_M_finish != y->_M_start + 3 || y->_M_end_of_storage._M_data != y->_M_start + 3) return 2;
    for (int k = 0; k < 3; ++k) if (y->_M_start[k].PData != (CBaseObjectPointer*)(0x40 + k)) return 3;
    x->_M_finish = src;
    new (y) std::vector<P>(*x);
    if (g_base != 2 || g_copies != 3 || y->_M_finish != y->_M_start) return 4;
    puts("0062ed40_TEST PASS");
    return 0;
}
