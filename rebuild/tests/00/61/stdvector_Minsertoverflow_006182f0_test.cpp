// Behaviour test for 0x006182F0: std::vector::_M_insert_overflow(CTCInventoryMap::CUIMiniMapMarker* position, const CUIMiniMapMarker& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for mini-map markers: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, destroy and free the old block
// A copy of the source model and logic (compiled separately from src/006182f0.cpp) with test stand-ins for the callees.
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
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

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
#pragma auto_inline(on)

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CTCInventoryMap::CUIMiniMapMarker>::_M_insert_overflow(CTCInventoryMap::CUIMiniMapMarker*, const CTCInventoryMap::CUIMiniMapMarker&, const __false_type&, unsigned int, bool);
}

// ---- test stand-ins ----
static int g_live = 0;
CCharString::CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }

typedef CTCInventoryMap::CUIMiniMapMarker Marker;
struct TestVector : public std::vector<Marker> {
    Marker* Begin() { return this->_M_start; }
    unsigned int Capacity() { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    void Init(Marker* s, unsigned int n, unsigned int cap) { this->_M_set(s, s + n, s + cap); }
};

static char g_names[16];
static int Fail() { std::printf("006182f0_TEST FAIL\n"); return 1; }
static void Set(Marker& m, long k)
{
    m.Region = k; m.UIDefName.m_pStringData = &g_names[k % 16]; m.Position.X = (float)k; m.Position.Y = -(float)k;
    m.DefNameLength = k + 1; m.GraphicIndex = k * 2; m.Angle = 0.5f * (float)k;
}
static bool Is(Marker& m, long k)
{
    return m.Region == k && m.UIDefName.m_pStringData == &g_names[k % 16] && m.Position.X == (float)k && m.Position.Y == -(float)k &&
           m.DefNameLength == k + 1 && m.GraphicIndex == k * 2 && m.Angle == 0.5f * (float)k;
}

int main()
{
    Marker* s = (Marker*)std::malloc(3 * sizeof(Marker));
    for (long k = 0; k < 3; ++k) Set(s[k], k);
    g_live = 3;
    Marker* x = (Marker*)std::malloc(sizeof(Marker));
    Set(*x, 9);
    TestVector v; v.Init(s, 3, 3);
    v._M_insert_overflow(v.Begin() + 1, *x, std::__false_type(), 1);       // grow 3 -> 6, x in the middle
    if (v.size() != 4 || v.Capacity() != 6 || g_live != 4) return Fail();
    if (!Is(v.Begin()[0], 0) || !Is(v.Begin()[1], 9) || !Is(v.Begin()[2], 1) || !Is(v.Begin()[3], 2)) return Fail();
    Set(*x, 7);
    Marker* end = v.Begin() + v.size();
    v._M_insert_overflow(end, *x, std::__false_type(), 5, true);            // grow 4 -> 9 at the end, five copies
    if (v.size() != 9 || v.Capacity() != 9 || g_live != 9) return Fail();
    if (!Is(v.Begin()[3], 2) || !Is(v.Begin()[4], 7) || !Is(v.Begin()[8], 7)) return Fail();
    v._M_insert_overflow(v.Begin(), *x, std::__false_type(), 2);            // grow 9 -> 18 at the front
    if (v.size() != 11 || v.Capacity() != 18 || g_live != 11) return Fail();
    if (!Is(v.Begin()[0], 7) || !Is(v.Begin()[1], 7) || !Is(v.Begin()[2], 0) || !Is(v.Begin()[3], 9) || !Is(v.Begin()[10], 7)) return Fail();
    v._M_clear();
    if (g_live != 0) return Fail();
    std::free(x);
    std::printf("006182f0_TEST PASS\n");
    return 0;
}