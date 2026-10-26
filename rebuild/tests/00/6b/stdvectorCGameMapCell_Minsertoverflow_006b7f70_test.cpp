// Behaviour test for 0x006B7F70 std::vector<CGameMapCell>::_M_insert_overflow(position, x, __false_type, fill_len, atend).
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
static void SetCell(CGameMapCell& c, int k) { c.AtmosThemes[0] = (unsigned char)k; c.AtmosThemes[1] = 0; c.AtmosThemes[2] = 0; c.AtmosBlends[0] = 0; c.AtmosBlends[1] = 0; c.SoundTheme = (signed char)k; c.Music = (unsigned char)k; }
static int Key(const CGameMapCell& c) { return c.AtmosThemes[0]; }

struct FalseType { FalseType() { } };

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

template <class InputIter, class ForwardIter>
ForwardIter UninitCopy(InputIter first, InputIter last, ForwardIter result, const FalseType&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur) new (cur) CGameMapCell(*first);
    return cur;
}
template <class ForwardIter, class Size, class T>
ForwardIter UninitFillN(ForwardIter first, Size n, const T& x, const FalseType&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur) new (cur) T(x);
    return cur;
}

struct CellVector {
    CGameMapCell* _M_start;
    CGameMapCell* _M_finish;
    CGameMapCell* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void InsertOverflow(CGameMapCell* position, const CGameMapCell& x, const FalseType&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + Max(old_size, fill_len);
        CGameMapCell* new_start = len != 0 ? (CGameMapCell*)std::malloc(len * sizeof(CGameMapCell)) : 0;
        CGameMapCell* new_finish = UninitCopy((const CGameMapCell*)_M_start, (const CGameMapCell*)position, new_start, FalseType());
        if (fill_len == 1) {
            new (new_finish) CGameMapCell(x);
            ++new_finish;
        } else
            new_finish = UninitFillN(new_finish, fill_len, x, FalseType());
        if (!atend)
            new_finish = UninitCopy((const CGameMapCell*)position, (const CGameMapCell*)_M_finish, new_finish, FalseType());
        if (_M_start != 0) std::free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    CellVector v;
    v._M_start = (CGameMapCell*)std::malloc(3 * sizeof(CGameMapCell));
    for (int i = 0; i < 3; ++i) SetCell(v._M_start[i], i + 1);
    v._M_finish = v._M_end_of_storage = v._M_start + 3;
    CGameMapCell x; SetCell(x, 9);
    // insert two copies in the middle: len = 3 + max(3, 2) = 6
    v.InsertOverflow(v._M_start + 1, x, FalseType(), 2, false);
    int ok = v.size() == 5 && v._M_end_of_storage == v._M_start + 6 && Key(v._M_start[0]) == 1 && Key(v._M_start[1]) == 9 &&
             Key(v._M_start[2]) == 9 && Key(v._M_start[3]) == 2 && Key(v._M_start[4]) == 3;
    // push_back path: one copy at the end, atend = true, len = 5 + 5 = 10
    SetCell(x, 7);
    v.InsertOverflow(v._M_finish, x, FalseType(), 1, true);
    if (v.size() != 6 || v._M_end_of_storage != v._M_start + 10 || Key(v._M_start[5]) != 7 || Key(v._M_start[0]) != 1) ok = 0;
    // fill_len larger than the old size: len = 6 + 8 = 14
    v.InsertOverflow(v._M_start, x, FalseType(), 8, false);
    if (v.size() != 14 || v._M_end_of_storage != v._M_start + 14 || Key(v._M_start[7]) != 7 || Key(v._M_start[8]) != 1) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("006b7f70_TEST FAIL\n"); return 1; }
    std::printf("006b7f70_TEST PASS\n");
    return 0;
}