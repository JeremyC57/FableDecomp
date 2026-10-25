// Behaviour test for 0x0048608C std::vector<CDateAndTime>::_M_fill_insert (model copy; growth is stubbed).
#include <cstdio>
#include <cstdlib>
#include <new>

struct CDateAndTime { unsigned long m[7]; };   // 28-byte record (Ego_r.pdb CDateAndTime; fields as words here)

static CDateAndTime* ucopy(CDateAndTime* f, CDateAndTime* l, CDateAndTime* r)
{ for (; f != l; ++f, ++r) new (r) CDateAndTime(*f); return r; }
static CDateAndTime* ufill_n(CDateAndTime* f, unsigned int n, const CDateAndTime& x)
{ for (; n > 0; --n, ++f) new (f) CDateAndTime(x); return f; }
static CDateAndTime* copy_backward(CDateAndTime* f, CDateAndTime* l, CDateAndTime* r)
{ for (int n = int(l - f); n > 0; --n) *--r = *--l; return r; }
static void fill(CDateAndTime* f, CDateAndTime* l, const CDateAndTime& x) { for (; f != l; ++f) *f = x; }

static int g_overflowCalls = 0;
struct VectorModel {
    CDateAndTime* _M_start;
    CDateAndTime* _M_finish;
    CDateAndTime* _M_end_of_storage;
    void _M_insert_overflow(CDateAndTime*, const CDateAndTime&, unsigned int) { ++g_overflowCalls; }
    void _M_fill_insert(CDateAndTime* position, unsigned int n, const CDateAndTime& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                CDateAndTime x_copy = x;
                const unsigned int elems_after = _M_finish - position;
                CDateAndTime* old_finish = _M_finish;
                if (elems_after > n) {
                    ucopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                }
                else {
                    ufill_n(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    ucopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            }
            else
                _M_insert_overflow(position, x, n);
        }
    }
};

static CDateAndTime R(unsigned long v) { CDateAndTime r; for (int i = 0; i < 7; ++i) r.m[i] = v; return r; }

int main()
{
    CDateAndTime buf[10];
    VectorModel v;
    v._M_start = buf;
    for (int i = 0; i < 4; ++i) buf[i] = R(i + 1);              // 1 2 3 4
    v._M_finish = buf + 4;
    v._M_end_of_storage = buf + 10;
    v._M_fill_insert(buf + 1, 2, R(9));                          // elems_after 3 > 2: 1 9 9 2 3 4
    int ok = v._M_finish == buf + 6 && buf[0].m[0] == 1 && buf[1].m[6] == 9 && buf[2].m[0] == 9 && buf[3].m[0] == 2 && buf[5].m[0] == 4;
    v._M_fill_insert(buf + 5, 3, R(7));                          // elems_after 1 <= 3: 1 9 9 2 3 7 7 7 4
    if (v._M_finish != buf + 9 || buf[4].m[0] != 3 || buf[5].m[0] != 7 || buf[7].m[0] != 7 || buf[8].m[0] != 4) ok = 0;
    v._M_fill_insert(buf, 5, R(1));                              // no room: growth path
    v._M_fill_insert(buf, 0, R(1));                              // n == 0: nothing
    if (g_overflowCalls != 1 || v._M_finish != buf + 9) ok = 0;
    if (!ok) { std::printf("0048608c_TEST FAIL\n"); return 1; }
    std::printf("0048608c_TEST PASS\n");
    return 0;
}