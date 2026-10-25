// behaviour test for 0x00A8DDC0 std::vector<CCountedPointer<...> >::_M_insert_overflow: growth at the end (fill_len 1, atend), growth in the middle with fill_len 3, reference counts of the copied pointers and release of the old block
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
class CUnprovenPointee_00a8ddc0;
struct CCPPointerInfo {
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static int gDeleted = 0;
static void CountDelete(void*) { ++gDeleted; }
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); }
        Data = 0; Info = 0;
    }
};
typedef CCountedPointer<CUnprovenPointee_00a8ddc0 > E;
struct Vec {
    E* _M_start; E* _M_finish; E* _M_end_of_storage;
    void _M_insert_overflow(E* pos, const E& x, unsigned int fill_len, bool atend)
    {
        unsigned int old = (unsigned int)(_M_finish - _M_start);
        unsigned int len = old + (old < fill_len ? fill_len : old);
        E* s = len ? (E*)std::malloc(len * sizeof(E)) : 0;
        E* f = s;
        for (E* p = _M_start; p != pos; ++p, ++f) new (f) E(*p);
        if (fill_len == 1) { new (f) E(x); ++f; }
        else for (unsigned int n = fill_len; n > 0; --n, ++f) new (f) E(x);
        if (!atend) for (E* p = pos; p != _M_finish; ++p, ++f) new (f) E(*p);
        for (E* p = _M_start; p != _M_finish; ++p) p->~E();
        if (_M_start) std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
};
int main()
{
    CCPPointerInfo a = { 0, CountDelete, 0 }, b = { 0, CountDelete, 0 }, c = { 0, CountDelete, 0 };
    int fails = 0;
    {
        E ea((CUnprovenPointee_00a8ddc0*)(size_t)0x10, &a), eb((CUnprovenPointee_00a8ddc0*)(size_t)0x20, &b), ec((CUnprovenPointee_00a8ddc0*)(size_t)0x30, &c);
        Vec v = { 0, 0, 0 };
        v._M_insert_overflow(v._M_finish, ea, 1, true);               // empty -> 1 slot
        if (v._M_finish - v._M_start != 1 || v._M_end_of_storage - v._M_start != 1 || a.RefCount != 2) fails |= 1;
        v._M_insert_overflow(v._M_finish, eb, 1, true);               // grow to 2
        if (v._M_finish - v._M_start != 2 || v._M_start[1].Info != &b || a.RefCount != 2 || b.RefCount != 2) fails |= 2;
        v._M_insert_overflow(v._M_start + 1, ec, 3, false);           // a, c, c, c, b in 2 + max(2, 3) = 5 slots
        if (v._M_finish - v._M_start != 5 || v._M_end_of_storage - v._M_start != 5) fails |= 4;
        if (v._M_start[0].Info != &a || v._M_start[1].Info != &c || v._M_start[3].Data != (CUnprovenPointee_00a8ddc0*)(size_t)0x30 || v._M_start[4].Info != &b) fails |= 8;
        if (a.RefCount != 2 || b.RefCount != 2 || c.RefCount != 4 || gDeleted != 0) fails |= 16;
        for (E* p = v._M_start; p != v._M_finish; ++p) p->~E();
        std::free(v._M_start);
        if (a.RefCount != 1 || b.RefCount != 1 || c.RefCount != 1) fails |= 32;
    }
    if (gDeleted != 3) fails |= 64;
    if (fails) { std::printf("00a8ddc0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a8ddc0_TEST PASS\n");
    return 0;
}