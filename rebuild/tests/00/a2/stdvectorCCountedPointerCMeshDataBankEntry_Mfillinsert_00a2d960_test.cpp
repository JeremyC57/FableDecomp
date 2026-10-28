// Behaviour test for 0x00A2D960: STLport vector<CCountedPointer<CMeshDataBankEntry> >::_M_fill_insert -- n copies of x at
// position, in place when the capacity allows (both the elems_after > n and <= n arms), reference counts balanced.
#include <cstdio>
#include <stdlib.h>
#include <new>

class CCPPointerInfo { public: unsigned long RefCount; };
struct CCountedPointer {
    int Data;
    CCPPointerInfo* Info;
    CCountedPointer(int data, CCPPointerInfo* info) : Data(data), Info(info) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) --Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Info != o.Info) { if (Info) --Info->RefCount; Info = o.Info; if (Info) ++Info->RefCount; }
        Data = o.Data;
        return *this;
    }
};
typedef CCountedPointer T;

struct Vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    static T* Copy(T* f, T* l, T* r) { for (; f != l; ++f, ++r) new (r) T(*f); return r; }
    static void FillN(T* f, unsigned int n, const T& x) { for (; n > 0; --n, ++f) new (f) T(x); }
    static void Fill(T* f, T* l, const T& x) { for (; f != l; ++f) *f = x; }
    static void CopyBackward(T* f, T* l, T* r) { while (l != f) *--r = *--l; }
    void _M_fill_insert(T* position, unsigned int n, const T& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                T x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    Copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    CopyBackward(position, old_finish - n, old_finish);
                    Fill(position, position + n, x_copy);
                } else {
                    FillN(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    Copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    Fill(position, old_finish, x_copy);
                }
            }
            // (the overflow path is the out-of-line _M_insert_overflow 0x00A2D4D0)
        }
    }
};

int main()
{
    CCPPointerInfo a = { 0 }, b = { 0 };
    T* block = (T*)malloc(10 * sizeof(T));
    Vec v = { block, block, block + 10 };
    for (int i = 0; i < 4; ++i) { new (v._M_finish) T(i, &a); ++v._M_finish; }        // a a a a
    { T x(9, &b); v._M_fill_insert(v._M_start + 1, 2, x); }                           // elems_after 3 > 2
    const int want1[] = { 0, 9, 9, 1, 2, 3 };
    for (int i = 0; i < 6; ++i) if (v._M_start[i].Data != want1[i]) { std::printf("00a2d960_TEST FAIL\n"); return 1; }
    if (v._M_finish - v._M_start != 6 || a.RefCount != 4 || b.RefCount != 2) { std::printf("00a2d960_TEST FAIL\n"); return 1; }
    { T y(7, &b); v._M_fill_insert(v._M_start + 5, 3, y); }                           // elems_after 1 <= 3
    const int want2[] = { 0, 9, 9, 1, 2, 7, 7, 7, 3 };
    for (int i = 0; i < 9; ++i) if (v._M_start[i].Data != want2[i]) { std::printf("00a2d960_TEST FAIL\n"); return 1; }
    if (v._M_finish - v._M_start != 9 || a.RefCount != 4 || b.RefCount != 5) { std::printf("00a2d960_TEST FAIL\n"); return 1; }
    for (T* p = v._M_start; p != v._M_finish; ++p) p->~T();
    free(block);
    if (a.RefCount != 0 || b.RefCount != 0) { std::printf("00a2d960_TEST FAIL\n"); return 1; }
    std::printf("00a2d960_TEST PASS\n");
    return 0;
}