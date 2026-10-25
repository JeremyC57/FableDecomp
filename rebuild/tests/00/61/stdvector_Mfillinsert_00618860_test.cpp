// Behaviour test for 0x00618860: std::vector::_M_fill_insert(CTCInventoryMap::CUIMiniMapMarker* position, unsigned int n, const CUIMiniMapMarker& x) -- STLport insert of n copies of a mini-map marker: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// A copy of the source model and logic (compiled separately from src/00618860.cpp) with test stand-ins for the callees.
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

class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                                       // 0x0099EC30
    ~CCharString();                                                              // 0x0099EAE0
    CCharString& operator=(const CCharString& other);
};

struct C2DVector {
    float X, Y;
};

class CTCInventoryMap {
public:
    class CUIMiniMapMarker {
    public:
        long        Region;                                                      // +0x00
        CCharString UIDefName;                                                   // +0x04
        C2DVector   Position;                                                    // +0x08
        long        DefNameLength;                                               // +0x10
        long        GraphicIndex;                                                // +0x14
        float       Angle;                                                       // +0x18
        CUIMiniMapMarker(const CUIMiniMapMarker& other);                         // 0x00612980
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00613DD0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x00613E30
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x00613390
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x00613180
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x006182F0
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CTCInventoryMap::CUIMiniMapMarker>::_M_fill_insert(CTCInventoryMap::CUIMiniMapMarker*, unsigned int, const CTCInventoryMap::CUIMiniMapMarker&);
}

// ---- test stand-ins ----
static int g_live = 0, g_assigns = 0;
CCharString::CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& other) { m_pStringData = other.m_pStringData; ++g_assigns; return *this; }
CTCInventoryMap::CUIMiniMapMarker::CUIMiniMapMarker(const CUIMiniMapMarker& other)
    : Region(other.Region), UIDefName(other.UIDefName), Position(other.Position), DefNameLength(other.DefNameLength),
      GraphicIndex(other.GraphicIndex), Angle(other.Angle) { }
typedef CTCInventoryMap::CUIMiniMapMarker Elem;
static char g_names[128];
static void Set(Elem& m, long k)
{
    m.Region = k; m.UIDefName.m_pStringData = &g_names[k]; m.Position.X = (float)k; m.Position.Y = -(float)k;
    m.DefNameLength = k + 1; m.GraphicIndex = k * 2; m.Angle = 0.5f * (float)k;
}
static bool Is(const Elem& m, long k)
{
    return m.Region == k && m.UIDefName.m_pStringData == &g_names[k] && m.Position.X == (float)k && m.Position.Y == -(float)k &&
           m.DefNameLength == k + 1 && m.GraphicIndex == k * 2 && m.Angle == 0.5f * (float)k;
}

static int g_overflowCalls = 0;
static unsigned int g_overflowN = 0;
static void* g_overflowPos = 0;
namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp&, const __false_type&, size_type __fill_len, bool)
{
    ++g_overflowCalls; g_overflowPos = (void*)__position; g_overflowN = __fill_len;
}
}

struct TestVector : public std::vector<Elem> {
    void Init(Elem* storage, unsigned int count, unsigned int capacity)
    {
        this->_M_start = storage; this->_M_finish = storage + count; this->_M_end_of_storage._M_data = storage + capacity;
    }
    Elem* Begin() { return this->_M_start; }
    unsigned int Size() { return (unsigned int)(this->_M_finish - this->_M_start); }
};

static int Fail() { std::printf("00618860_TEST FAIL\n"); return 1; }
static bool Same(TestVector& v, const long* want, unsigned int n)
{
    if (v.Size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (!Is(v.Begin()[i], want[i])) return false;
    return true;
}

int main()
{
    Elem* storage = (Elem*)std::malloc(16 * sizeof(Elem));
    for (long k = 0; k < 6; ++k) Set(storage[k], k);
    Elem* x = (Elem*)std::malloc(sizeof(Elem));
    Set(*x, 99);
    TestVector v; v.Init(storage, 6, 14);
    g_live = 6;
    v._M_fill_insert(v.Begin() + 2, 3, *x);                 // elems_after 4 > 3: copy the tail up, copy_backward, fill
    const long k1[] = { 0, 1, 99, 99, 99, 2, 3, 4, 5 };
    if (!Same(v, k1, 9)) return Fail();
    Set(*x, 50);
    v._M_fill_insert(v.Begin() + 7, 4, *x);                 // elems_after 2 <= 4: fill_n the excess, copy, fill
    const long k2[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5 };
    if (!Same(v, k2, 13)) return Fail();
    v._M_fill_insert(v.Begin(), 0, *x);                     // n == 0
    if (!Same(v, k2, 13) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 13, 1, *x);                // exactly fills the capacity
    const long k3[] = { 0, 1, 99, 99, 99, 2, 3, 50, 50, 50, 50, 4, 5, 50 };
    if (!Same(v, k3, 14) || g_overflowCalls != 0) return Fail();
    v._M_fill_insert(v.Begin() + 1, 2, *x);                 // no spare room: _M_insert_overflow(pos, x, tag, n)
    if (g_overflowCalls != 1 || g_overflowN != 2 || g_overflowPos != (void*)(v.Begin() + 1) || !Same(v, k3, 14)) return Fail();
    if (g_live != 14 || g_assigns == 0) return Fail();
    std::free(x); std::free(storage);
    std::printf("00618860_TEST PASS\n");
    return 0;
}
