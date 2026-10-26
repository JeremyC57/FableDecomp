// Behaviour test for 0x00493FF8 std::vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow: grow to
// old_size + max(old_size, n), copy the head, construct n copies of x, copy the tail unless appending, free the old block.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#include <stddef.h>
#include <new>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class C2DVector { public:
    float X;
    float Y;
};

class CSubtitleRenderer {
public:
    class CEmbeddedGraphic { public:                       // Ego_r.pdb, 16 bytes
        long      Graphic;                          // +0x00
        long      CharIndex;                        // +0x04
        C2DVector Offset;                           // +0x08
        CEmbeddedGraphic(const CEmbeddedGraphic& other);   // 0x004935F8
    };
};

namespace std {

struct __false_type { __false_type() { } };

typedef CSubtitleRenderer::CEmbeddedGraphic CEmbeddedGraphic;

CEmbeddedGraphic* __fastcall __uninitialized_copy(const CEmbeddedGraphic* __first, const CEmbeddedGraphic* __last,
                                                CEmbeddedGraphic* __result, const __false_type&)        // 0x00493959 (same-TU stand-in body)
{
    CEmbeddedGraphic* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CEmbeddedGraphic(*__first);
    return __cur;
}
CEmbeddedGraphic* __fastcall __uninitialized_fill_n(CEmbeddedGraphic* __first, size_t __n, const CEmbeddedGraphic& __x,
                                                  const __false_type&)                                // 0x00493984 (same-TU stand-in body)
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CEmbeddedGraphic(__x);
    return __first;
}

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    size_t size() const { return size_t(_M_finish - _M_start); }
    __forceinline void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                                     bool __atend)
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CEmbeddedGraphic>::_M_insert_overflow(CEmbeddedGraphic*, const CEmbeddedGraphic&, const __false_type&,
                                                         size_t, bool);

}

static int g_copies = 0;
CSubtitleRenderer::CEmbeddedGraphic::CEmbeddedGraphic(const CEmbeddedGraphic& other)
    : Graphic(other.Graphic), CharIndex(other.CharIndex), Offset(other.Offset) { ++g_copies; }

static CSubtitleRenderer::CEmbeddedGraphic* MakeOne(long g)
{
    CSubtitleRenderer::CEmbeddedGraphic* p = (CSubtitleRenderer::CEmbeddedGraphic*)malloc(sizeof(CSubtitleRenderer::CEmbeddedGraphic));
    p->Graphic = g; p->CharIndex = g * 10; p->Offset.X = (float)g; p->Offset.Y = -(float)g;
    return p;
}

int main()
{
    typedef CSubtitleRenderer::CEmbeddedGraphic G;
    std::vector<G> v; v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    int ok = 1;
    G* x1 = MakeOne(1); G* x2 = MakeOne(2); G* x5 = MakeOne(5);
    v._M_insert_overflow(v._M_finish, *x1, std::__false_type(), 1, true);     // push_back into an empty vector
    if (v.size() != 1 || v._M_end_of_storage._M_data - v._M_start != 1 || v._M_start[0].Graphic != 1) ok = 0;
    v._M_insert_overflow(v._M_finish, *x2, std::__false_type(), 1, true);
    if (v.size() != 2 || v._M_end_of_storage._M_data - v._M_start != 2 || v._M_start[1].Graphic != 2) ok = 0;
    v._M_insert_overflow(v._M_start + 1, *x5, std::__false_type(), 3, false);  // 1 5 5 5 2, capacity 2 + 3
    long want[5] = { 1, 5, 5, 5, 2 };
    if (v.size() != 5 || v._M_end_of_storage._M_data - v._M_start != 5) ok = 0;
    for (int i = 0; i < 5 && ok; ++i)
        if (v._M_start[i].Graphic != want[i] || v._M_start[i].CharIndex != want[i] * 10 || v._M_start[i].Offset.Y != -(float)want[i]) ok = 0;
    if (g_copies != 1 + 2 + 5) ok = 0;                                            // 1, then 1 + 1, then 1 + 3 + 1
    printf(ok ? "00493ff8_TEST PASS\n" : "00493ff8_TEST FAIL\n");
    return ok ? 0 : 1;
}