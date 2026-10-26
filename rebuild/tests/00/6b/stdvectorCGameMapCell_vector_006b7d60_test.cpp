// Behaviour test for 0x006B7D60 std::vector<CGameMapCell>::vector(unsigned int n).
#include <cstdio>
#include <cstdlib>
#include <new>

class CGameMapCell { public:
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;
    unsigned char Music;
    CGameMapCell() { }
};

static int g_filled = 0;

template <class ForwardIter, class Size, class T>
ForwardIter UninitFillN(ForwardIter first, Size n, const T& x)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur) { new (cur) T(x); ++g_filled; }
    return cur;
}

template <class T> struct Allocator {
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};

template <class T> struct CellVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    explicit CellVector(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        Allocator<T> a;
        _M_start = a.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = UninitFillN(_M_start, n, T());
    }
};

int main()
{
    CellVector<CGameMapCell> v(5);
    int ok = v._M_start != 0 && v._M_finish == v._M_start + 5 && v._M_end_of_storage == v._M_start + 5 && g_filled == 5;
    if ((char*)v._M_finish - (char*)v._M_start != 35) ok = 0;   // 7-byte cells
    std::free(v._M_start);
    CellVector<CGameMapCell> e(0);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0 || g_filled != 5) ok = 0;
    if (!ok) { std::printf("006b7d60_TEST FAIL\n"); return 1; }
    std::printf("006b7d60_TEST PASS\n");
    return 0;
}