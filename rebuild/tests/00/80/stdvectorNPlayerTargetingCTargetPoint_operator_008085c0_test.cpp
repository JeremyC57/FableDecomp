// std::vector<CTargetPoint>::operator=(const vector& x) -- STLport copy assignment of a player-targeting candidate vector (28-byte {PThing, Index, TargetingPosition, FrameRejected} entries): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCTargetPoint@NPlayerTargeting@@V?$allocator@VCTargetPoint@NPlayerTargeting@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008085C0, 325 bytes, thiscall, `ret 4`.
// Identity (sweep2, 2026-10-25): 28-byte stride (`imul 0x92492493`, /0x1C); the element helpers copy a
//   CIntelligentPointer<CThing> at +0 (vptr 0x01238C6C + pointer, 0x00A01B10/0x00A01B50/0x00A01B90) and five dwords
//   +8..+0x18 = Ego_r.pdb NPlayerTargeting::CTargetPoint {CIntelligentPointer<CThing> PThing, long Index, C3DVector
//   TargetingPosition, long FrameRejected}, 28 bytes; all four callers are in the player-targeting update 0x00804020.
//   Ego_r.pdb name set: ??4?$vector@VCTargetPoint@NPlayerTargeting@@V?$allocator@VCTargetPoint@NPlayerTargeting@@@std@@@std@@QAEAAV01@ABV01@@Z.
//   The landed name CCombatWheel_AssignFrom was propagated; the model was a nested VectorAssignment_008085c0::std over
//   `unsigned[7]` (owner 2026-10-25: top-level std).
// Callees: __uninitialized_copy @ 0x00807980, __copy @ 0x00806EA0, _Destroy @ 0x004DFBC6, _M_clear @ 0x00808580, malloc @ 0x00BFEA0E.
// Shape: STLport 4.6 vector::operator= over a class element (__type_traits all __false_type): _M_allocate_and_copy
//   inlined (allocate + __uninitialized_copy), __copy_ptrs inlined to its random-access __copy call. __copy and
//   __uninitialized_copy carry same-TU __declspec(noinline) stand-in bodies (retail's are 0x00806EA0 / 0x00807980):
//   with declarations only, VC7.1 gives the iterator tag its own frame byte where retail reuses x's dead argument slot.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E

class CThing;
class C3DVector {
public:
    float X, Y, Z;
};
template <class T> class CIntelligentPointer {
public:
    void* m_Vtable;                                         // +0x0 (vptr 0x01238C6C in retail)
    T*    m_Pointer;                                        // +0x4
    CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer& other);
    ~CIntelligentPointer();
    CIntelligentPointer& operator=(const CIntelligentPointer& other);
};

namespace NPlayerTargeting {
class CTargetPoint {                                        // Ego_r.pdb, 28 bytes
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long      Index;                                        // +0x08
    C3DVector TargetingPosition;                            // +0x0C
    long      FrameRejected;                                // +0x18
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

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)   // same-TU stand-in body
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
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

template vector<NPlayerTargeting::CTargetPoint>& vector<NPlayerTargeting::CTargetPoint>::operator=(const vector<NPlayerTargeting::CTargetPoint>&);

}   // namespace std

// ---- test harness: out-of-line helper bodies + behaviour test ----
#include <stdio.h>
#include <stdlib.h>
#include <new>

template <class T> CIntelligentPointer<T>::CIntelligentPointer() : m_Vtable(0), m_Pointer(0) { }
template <class T> CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer& o) : m_Vtable(0), m_Pointer(o.m_Pointer) { }
template <class T> CIntelligentPointer<T>::~CIntelligentPointer() { }
template <class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& o) { m_Pointer = o.m_Pointer; return *this; }
static int g_things[16];
static NPlayerTargeting::CTargetPoint make(int i)
{
    NPlayerTargeting::CTargetPoint p;
    p.PThing.m_Pointer = (CThing*)&g_things[i]; p.Index = i; p.TargetingPosition.X = 1.0f * i; p.TargetingPosition.Y = 2.0f * i;
    p.TargetingPosition.Z = 3.0f * i; p.FrameRejected = 50 + i;
    return p;
}
static bool same(const NPlayerTargeting::CTargetPoint& a, const NPlayerTargeting::CTargetPoint& b)
{
    return a.PThing.m_Pointer == b.PThing.m_Pointer && a.Index == b.Index && a.TargetingPosition.X == b.TargetingPosition.X &&
           a.TargetingPosition.Y == b.TargetingPosition.Y && a.TargetingPosition.Z == b.TargetingPosition.Z && a.FrameRejected == b.FrameRejected;
}

namespace std {
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last) { for ( ; __first != __last; ++__first) __first->~CTargetPoint(); }
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    free(this->_M_start);
}
}
struct VecView { NPlayerTargeting::CTargetPoint* s; NPlayerTargeting::CTargetPoint* f; NPlayerTargeting::CTargetPoint* e; };
static VecView& W(std::vector<NPlayerTargeting::CTargetPoint>& v) { return *(VecView*)&v; }
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { printf("fail line %d\n", line); g_fail = 1; } }
static bool same_range(const NPlayerTargeting::CTargetPoint* a, const NPlayerTargeting::CTargetPoint* b, int n) { for (int i = 0; i < n; ++i) if (!same(a[i], b[i])) return false; return true; }
int main()
{
    NPlayerTargeting::CTargetPoint data[5];
    for (int i = 0; i < 5; ++i) data[i] = make(i);
    std::vector<NPlayerTargeting::CTargetPoint> a, b;
    W(a).s = W(a).f = W(a).e = 0;
    W(b).s = data; W(b).f = data + 3; W(b).e = data + 5;
    a = b;                                              // grow: allocate + copy-construct
    check(W(a).f - W(a).s == 3 && W(a).e - W(a).s == 3 && same_range(W(a).s, data, 3), __LINE__);
    NPlayerTargeting::CTargetPoint* storage = W(a).s;
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
    if (!g_fail) puts("008085c0_TEST PASS");
    return g_fail;
}
