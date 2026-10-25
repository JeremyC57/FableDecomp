// std::vector<pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > >::operator=(const vector& x) -- STLport copy assignment of the particle update-parameter map's vector (16-byte {key, CParticleUpdateParamDesc} entries): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@U?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@V?$allocator@U?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E22F0, 255 bytes, thiscall, `ret 4`.
// Identity (sweep2, 2026-10-25): the landed NParticleEngine::CParticleUpdateParameters::operator= 0x006E2570 assigns
//   UpdateParams (+0x14, Ego_r CVectorMap<unsigned long, CParticleUpdateParamDesc<unsigned long,unsigned long>, ...>)
//   through this call; CParticleEmitter::SetUpdateParameters 0x00AF0610 and CParticleSystem::Update 0x00AF59A0 call
//   it the same way. The element helpers copy a key dword, store the CParticleUpdateParamDesc vptr 0x0125FEE4 and
//   copy Callback/Index (__uninitialized_copy 0x006E1F80), and destroy through ~CBaseClass 0x0099A2E0 (_Destroy
//   0x006E2200): Ego_r.pdb CParticleUpdateParamDesc<ulong,ulong> = {vptr, Callback*, ulong Index}, 12 bytes. Ego_r.pdb
//   name set: ??4?$vector@U?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@V?$allocator@...@std@@QAEAAV01@ABV01@@Z.
//   The landed name CCombatWheel_InsertAnimStates was propagated; the model was a nested VectorAssignment_006e22f0::std
//   over `unsigned[4]` (owner 2026-10-25: top-level std).
// Callees: __uninitialized_copy @ 0x006E1F80, __copy_ptrs @ 0x006E1F40, _Destroy @ 0x006E2200, _M_clear @ 0x006E2230, malloc @ 0x00BFEA0E.
// Shape: STLport 4.6 vector::operator= over a class element (__type_traits all __false_type): _M_allocate_and_copy
//   inlined (allocate + __uninitialized_copy); __copy_ptrs is out of line in this TU. __copy_ptrs and
//   __uninitialized_copy carry same-TU __declspec(noinline) stand-in bodies (retail's are the callees above).
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E

class CBaseClass {
public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};

namespace NParticleEngine {
template <class _Key, class _Value> class CParticleUpdateParamCallback;
template <class _Key, class _Value> class CParticleUpdateParamDesc : public CBaseClass {   // Ego_r.pdb, 12 bytes
public:
    CParticleUpdateParamCallback<_Key, _Value>* Callback;   // +0x4
    unsigned long Index;                                    // +0x8
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)   // same-TU stand-in body
{
    for (int __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)   // same-TU stand-in body
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                    // out of line

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear();                                                                           // out of line
    vector& operator=(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >& vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >::operator=(const vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >&);

}   // namespace std

// ---- test harness: out-of-line helper bodies + behaviour test ----
#include <stdio.h>
#include <stdlib.h>
#include <new>

CBaseClass::~CBaseClass() { }
static NParticleEngine::CParticleUpdateParamCallback<unsigned long, unsigned long>* cb(int i) { return (NParticleEngine::CParticleUpdateParamCallback<unsigned long, unsigned long>*)(0x1000 + 16 * i); }
static std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > make(int i)
{
    std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > p;
    p.first = 10 + i; p.second.Callback = cb(i); p.second.Index = 200 + i;
    return p;
}
static bool same(const std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >& a,
                 const std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >& b)
{
    return a.first == b.first && a.second.Callback == b.second.Callback && a.second.Index == b.second.Index;
}

namespace std {
template <class _Tp> void _DestroyOne(_Tp* __p) { __p->~_Tp(); }
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last) { for ( ; __first != __last; ++__first) _DestroyOne(__first); }
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    free(this->_M_start);
}
}
using namespace std;
struct VecView { pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * s; pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * f; pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * e; };
static VecView& W(std::vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >& v) { return *(VecView*)&v; }
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { printf("fail line %d\n", line); g_fail = 1; } }
static bool same_range(const pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * a, const pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * b, int n) { for (int i = 0; i < n; ++i) if (!same(a[i], b[i])) return false; return true; }
int main()
{
    pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >  data[5];
    for (int i = 0; i < 5; ++i) data[i] = make(i);
    std::vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > > a, b;
    W(a).s = W(a).f = W(a).e = 0;
    W(b).s = data; W(b).f = data + 3; W(b).e = data + 5;
    a = b;                                              // grow: allocate + copy-construct
    check(W(a).f - W(a).s == 3 && W(a).e - W(a).s == 3 && same_range(W(a).s, data, 3), __LINE__);
    pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > * storage = W(a).s;
    a = a;                                              // self-assignment: no-op
    check(W(a).s == storage && W(a).f == storage + 3, __LINE__);
    W(b).f = data + 1; a = b;                           // shrink: assign prefix, destroy the rest
    check(W(a).s == storage && W(a).f == storage + 1 && same_range(W(a).s, data, 1), __LINE__);
    data[1] = make(7); data[2] = make(8); W(b).f = data + 3; a = b;   // within capacity: assign + construct tail
    check(W(a).s == storage && W(a).f == storage + 3 && same_range(W(a).s, data, 3), __LINE__);
    W(b).f = data + 5; a = b;                           // reallocate
    check(W(a).f - W(a).s == 5 && W(a).e - W(a).s == 5 && same_range(W(a).s, data, 5), __LINE__);
    W(b).f = data; a = b;
    check(W(a).f == W(a).s, __LINE__);
    if (!g_fail) puts("006e22f0_TEST PASS");
    return g_fail;
}
