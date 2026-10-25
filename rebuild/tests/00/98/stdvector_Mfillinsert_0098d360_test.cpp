// Behaviour test for 0x0098D360 std::vector<C4DVector>::_M_fill_insert (STLport in-place / overflow insert).
#include <cstdio>
#include <cstdlib>
#include <new>

class C4DVector {
public:
    float X, Y, Z, W;
};

static int g_overflowCalls = 0;

struct CellVector {
    C4DVector* _M_start;
    C4DVector* _M_finish;
    C4DVector* _M_end_of_storage;
    void _M_insert_overflow(C4DVector*, const C4DVector&, unsigned int) { ++g_overflowCalls; }
    void _M_fill_insert(C4DVector* pos, unsigned int n, const C4DVector& x);
};

static C4DVector* UninitCopy(C4DVector* f, C4DVector* l, C4DVector* r)
{ for (; f != l; ++f, ++r) new (r) C4DVector(*f); return r; }
static C4DVector* CopyBackward(C4DVector* f, C4DVector* l, C4DVector* r)
{ for (long n = (long)(l - f); n > 0; --n) *--r = *--l; return r; }
static void Fill(C4DVector* f, C4DVector* l, const C4DVector& v) { for (; f != l; ++f) *f = v; }
static C4DVector* UninitFillN(C4DVector* f, unsigned int n, const C4DVector& v)
{ for (; n > 0; --n, ++f) new (f) C4DVector(v); return f; }

void CellVector::_M_fill_insert(C4DVector* pos, unsigned int n, const C4DVector& x)
{
    if (n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            C4DVector xc = x;
            const unsigned int after = (unsigned int)(_M_finish - pos);
            C4DVector* oldFinish = _M_finish;
            if (after > n) {
                UninitCopy(_M_finish - n, _M_finish, _M_finish);
                _M_finish += n;
                CopyBackward(pos, oldFinish - n, oldFinish);
                Fill(pos, pos + n, xc);
            } else {
                UninitFillN(_M_finish, n - after, xc);
                _M_finish += n - after;
                UninitCopy(pos, oldFinish, _M_finish);
                _M_finish += after;
                Fill(pos, oldFinish, xc);
            }
        } else
            _M_insert_overflow(pos, x, n);
    }
}

static C4DVector Cell(float h) { C4DVector c; c.X = h; c.Y = 0; c.Z = 0; c.W = h * 2; return c; }

static bool Check(CellVector& v, const float* want, int count)
{
    if (v._M_finish - v._M_start != count) return false;
    for (int i = 0; i < count; ++i)
        if (v._M_start[i].X != want[i] || v._M_start[i].W != want[i] * 2) return false;
    return true;
}

int main()
{
    C4DVector buf[10];
    CellVector v;
    v._M_start = buf; v._M_finish = buf; v._M_end_of_storage = buf + 10;
    for (int i = 0; i < 5; ++i) { buf[i] = Cell((float)(i + 1)); ++v._M_finish; }
    // elems_after (4) > n (2): insert 2 x 9 before index 1
    v._M_fill_insert(buf + 1, 2, Cell(9.0f));
    const float a[] = { 1, 9, 9, 2, 3, 4, 5 };
    if (!Check(v, a, 7)) { std::printf("0098d360_TEST FAIL\n"); return 1; }
    // elems_after (1) <= n (2): insert 2 x 7 before the last element
    v._M_fill_insert(buf + 6, 2, Cell(7.0f));
    const float b[] = { 1, 9, 9, 2, 3, 4, 7, 7, 5 };
    if (!Check(v, b, 9)) { std::printf("0098d360_TEST FAIL\n"); return 1; }
    // n == 0: no change; no room: overflow path
    v._M_fill_insert(buf, 0, Cell(1.0f));
    v._M_fill_insert(buf, 3, Cell(1.0f));
    if (!Check(v, b, 9) || g_overflowCalls != 1) { std::printf("0098d360_TEST FAIL\n"); return 1; }
    std::printf("0098d360_TEST PASS\n");
    return 0;
}