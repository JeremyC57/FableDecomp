// Behaviour test for 0x006B7E20 vector<CGameMapCell>::operator= (model copy of the STLport algorithm).
#include <cstdio>
#include <cstdlib>
#include <new>

struct CGameMapCell {
    unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music;
};

struct Vec {
    CGameMapCell* _M_start; CGameMapCell* _M_finish; CGameMapCell* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static CGameMapCell* ucopy(const CGameMapCell* f, const CGameMapCell* l, CGameMapCell* r)
    { for (; f != l; ++f, ++r) new (r) CGameMapCell(*f); return r; }
    static CGameMapCell* copy(const CGameMapCell* f, const CGameMapCell* l, CGameMapCell* r)
    { for (int n = (int)(l - f); n > 0; --n) { *r = *f; ++f; ++r; } return r; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CGameMapCell* tmp = xlen != 0 ? (CGameMapCell*)std::malloc(xlen * sizeof(CGameMapCell)) : 0;
                ucopy(x._M_start, x._M_finish, tmp);
                if (_M_start) std::free(_M_start);
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                copy(x._M_start, x._M_finish, _M_start);
            } else {
                copy(x._M_start, x._M_start + size(), _M_start);
                ucopy(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};

static void fill(Vec& v, unsigned int n, unsigned int cap, int base)
{
    v._M_start = (CGameMapCell*)std::malloc(cap * sizeof(CGameMapCell));
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
    for (unsigned int i = 0; i < n; ++i) { v._M_start[i].Music = (unsigned char)(base + i); v._M_start[i].SoundTheme = -1; }
}

int main()
{
    Vec a, b, c;
    fill(a, 5, 5, 10); fill(b, 2, 3, 50); fill(c, 1, 8, 90);
    int ok = 1;
    b = a;                                                       // grows: 5 > capacity 3
    ok = ok && b.size() == 5 && b.capacity() == 5 && b._M_start[4].Music == 14 && b._M_start != a._M_start;
    fill(a, 2, 2, 20);
    c = a;                                                       // size 1 < 2 <= capacity 8: copy 1, construct 1
    ok = ok && c.size() == 2 && c.capacity() == 8 && c._M_start[0].Music == 20 && c._M_start[1].Music == 21;
    fill(a, 1, 1, 30);
    c = a;                                                       // shrink: size 2 >= 1
    ok = ok && c.size() == 1 && c.capacity() == 8 && c._M_start[0].Music == 30 && c._M_start[0].SoundTheme == -1;
    Vec& r = (c = c);
    ok = ok && &r == &c && c.size() == 1;
    if (!ok) { std::printf("006b7e20_TEST FAIL\n"); return 1; }
    std::printf("006b7e20_TEST PASS\n");
    return 0;
}