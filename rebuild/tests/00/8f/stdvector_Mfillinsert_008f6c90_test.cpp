// Behaviour test for 0x008F6C90 std::vector<CHeightMapCell>::_M_fill_insert (STLport in-place / overflow insert).
#include <cstdio>
#include <cstdlib>
#include <new>

class CHeightMapCell { public:
    float         Height;
    unsigned char EngineThemePaletteIndex[3];
    unsigned char EngineBlend[2];
    unsigned char Changed : 1;
    unsigned char Passable : 1;
    unsigned char ShorePoint : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavPreferred : 1;
    CHeightMapCell() { }
};

static int g_overflowCalls = 0;

struct CellVector {
    CHeightMapCell* _M_start;
    CHeightMapCell* _M_finish;
    CHeightMapCell* _M_end_of_storage;
    void _M_insert_overflow(CHeightMapCell*, const CHeightMapCell&, unsigned int) { ++g_overflowCalls; }
    void _M_fill_insert(CHeightMapCell* pos, unsigned int n, const CHeightMapCell& x);
};

static CHeightMapCell* UninitCopy(CHeightMapCell* f, CHeightMapCell* l, CHeightMapCell* r)
{ for (; f != l; ++f, ++r) new (r) CHeightMapCell(*f); return r; }
static CHeightMapCell* CopyBackward(CHeightMapCell* f, CHeightMapCell* l, CHeightMapCell* r)
{ for (long n = (long)(l - f); n > 0; --n) *--r = *--l; return r; }
static void Fill(CHeightMapCell* f, CHeightMapCell* l, const CHeightMapCell& v) { for (; f != l; ++f) *f = v; }
static CHeightMapCell* UninitFillN(CHeightMapCell* f, unsigned int n, const CHeightMapCell& v)
{ for (; n > 0; --n, ++f) new (f) CHeightMapCell(v); return f; }

void CellVector::_M_fill_insert(CHeightMapCell* pos, unsigned int n, const CHeightMapCell& x)
{
    if (n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            CHeightMapCell xc = x;
            const unsigned int after = (unsigned int)(_M_finish - pos);
            CHeightMapCell* oldFinish = _M_finish;
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

static CHeightMapCell Cell(float h) { CHeightMapCell c; c.Height = h; c.EngineThemePaletteIndex[0] = (unsigned char)h; return c; }

static bool Check(CellVector& v, const float* want, int count)
{
    if (v._M_finish - v._M_start != count) return false;
    for (int i = 0; i < count; ++i)
        if (v._M_start[i].Height != want[i] || v._M_start[i].EngineThemePaletteIndex[0] != (unsigned char)want[i]) return false;
    return true;
}

int main()
{
    CHeightMapCell buf[10];
    CellVector v;
    v._M_start = buf; v._M_finish = buf; v._M_end_of_storage = buf + 10;
    for (int i = 0; i < 5; ++i) { buf[i] = Cell((float)(i + 1)); ++v._M_finish; }
    // elems_after (4) > n (2): insert 2 x 9 before index 1
    v._M_fill_insert(buf + 1, 2, Cell(9.0f));
    const float a[] = { 1, 9, 9, 2, 3, 4, 5 };
    if (!Check(v, a, 7)) { std::printf("008f6c90_TEST FAIL\n"); return 1; }
    // elems_after (1) <= n (2): insert 2 x 7 before the last element
    v._M_fill_insert(buf + 6, 2, Cell(7.0f));
    const float b[] = { 1, 9, 9, 2, 3, 4, 7, 7, 5 };
    if (!Check(v, b, 9)) { std::printf("008f6c90_TEST FAIL\n"); return 1; }
    // n == 0: no change; no room: overflow path
    v._M_fill_insert(buf, 0, Cell(1.0f));
    v._M_fill_insert(buf, 3, Cell(1.0f));
    if (!Check(v, b, 9) || g_overflowCalls != 1) { std::printf("008f6c90_TEST FAIL\n"); return 1; }
    std::printf("008f6c90_TEST PASS\n");
    return 0;
}