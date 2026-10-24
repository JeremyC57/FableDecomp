// Behaviour test for _STL::__uninitialized_fill_n over {counted pointer, long} records (0x009CA500).
#include <cstdio>
#include <new>

struct __false_type { __false_type() {} };

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter UninitializedFillN(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) _Tp(x);
    return cur;
}

struct SRefCounted { void* vtable; long RefCount; };
struct SIVCountedPointer
{
    SRefCounted* P;
    SIVCountedPointer(SRefCounted* p) : P(p) {}
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
struct SCountedPointerLong
{
    SIVCountedPointer m_pointer;
    long              m_value;
    SCountedPointerLong(SRefCounted* p, long v) : m_pointer(p), m_value(v) {}
};

int main()
{
    SRefCounted target = { 0, 1 };
    SCountedPointerLong value(&target, 42);
    union { double align; unsigned char raw[sizeof(SCountedPointerLong) * 4]; } storage;
    SCountedPointerLong* first = (SCountedPointerLong*)storage.raw;
    SCountedPointerLong* end = UninitializedFillN(first, 3u, value, __false_type());
    if (end != first + 3 || target.RefCount != 4) { std::printf("009ca500_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (first[i].m_pointer.P != &target || first[i].m_value != 42) { std::printf("009ca500_TEST FAIL\n"); return 1; }
    if (UninitializedFillN(first, 0u, value, __false_type()) != first || target.RefCount != 4) { std::printf("009ca500_TEST FAIL\n"); return 1; }
    SCountedPointerLong empty(0, 7);
    UninitializedFillN(first, 1u, empty, __false_type());
    if (first[0].m_pointer.P != 0 || first[0].m_value != 7) { std::printf("009ca500_TEST FAIL\n"); return 1; }
    std::printf("009ca500_TEST PASS\n");
    return 0;
}