// Behaviour test for std::_Uninit_fill_n<CHandleInfo*, unsigned int, CHandleInfo, allocator<CHandleInfo> > @ 0x00C294B0.
#include <cstdio>
#include <new>

struct CHeader;
struct CHandleInfo {
    union { CHeader* Header; long NextFreeHandle; };
};

namespace model {
template<class T> struct allocator {
    void construct(T* p, const T& v) { new ((void*)p) T(v); }
    void destroy(T* p) { p->~T(); }
};

template<class FwdIt, class Diff, class Tval, class Alloc>
void uninit_fill_n(FwdIt first, Diff count, const Tval& val, Alloc& al)
{
    FwdIt next = first;
    try {
        for (; 0 < count; --count, ++first)
            al.construct(first, val);
    } catch (...) {
        for (; next != first; ++next)
            al.destroy(next);
        throw;
    }
}
}

int main()
{
    CHandleInfo slots[8];
    for (int i = 0; i < 8; ++i) slots[i].NextFreeHandle = -7;
    CHandleInfo value;
    value.NextFreeHandle = 1234;
    model::allocator<CHandleInfo> al;
    model::uninit_fill_n(slots + 1, 5u, value, al);
    bool ok = slots[0].NextFreeHandle == -7 && slots[6].NextFreeHandle == -7 && slots[7].NextFreeHandle == -7;
    for (int i = 1; i < 6; ++i) if (slots[i].NextFreeHandle != 1234) ok = false;
    model::uninit_fill_n(slots, 0u, value, al);             // a zero count writes nothing
    if (slots[0].NextFreeHandle != -7) ok = false;
    if (!ok) { std::printf("00c294b0_TEST FAIL\n"); return 1; }
    std::printf("00c294b0_TEST PASS\n");
    return 0;
}