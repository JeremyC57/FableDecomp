// Behaviour test for 0x00B4A2C0 std::vector<CEngineLightingManager::CThemeChannel>::_M_fill_insert (STLport 4.6 vector.c logic).
#include <cstdio>
#include <cstdlib>

class C3DVector { public: float X, Y, Z; };
struct CThemeChannel { C3DVector Center; float Radius; float Rest[24]; };   // stands for the 112-byte CThemeChannel

struct ChannelVector {
    CThemeChannel* _M_start;
    CThemeChannel* _M_finish;
    CThemeChannel* _M_end_of_storage;
    int overflowCalls;
};

static CThemeChannel* UninitCopy(CThemeChannel* f, CThemeChannel* l, CThemeChannel* r)
{
    for (; f != l; ++f, ++r) *r = *f;
    return r;
}
static CThemeChannel* CopyBackward(CThemeChannel* f, CThemeChannel* l, CThemeChannel* r)
{
    for (int n = (int)(l - f); n > 0; --n) *--r = *--l;
    return r;
}
static void Fill(CThemeChannel* f, CThemeChannel* l, const CThemeChannel& v)
{
    for (; f != l; ++f) *f = v;
}
static void InsertOverflow(ChannelVector* v, CThemeChannel* pos, const CThemeChannel& x, unsigned int n)
{
    unsigned int size = (unsigned int)(v->_M_finish - v->_M_start);
    unsigned int len = size + (size > n ? size : n);
    CThemeChannel* s = (CThemeChannel*)std::malloc(len * sizeof(CThemeChannel));
    CThemeChannel* f = UninitCopy(v->_M_start, pos, s);
    for (unsigned int i = 0; i < n; ++i, ++f) *f = x;
    f = UninitCopy(pos, v->_M_finish, f);
    std::free(v->_M_start);
    v->_M_start = s; v->_M_finish = f; v->_M_end_of_storage = s + len;
    ++v->overflowCalls;
}

static void FillInsert(ChannelVector* v, CThemeChannel* position, unsigned int n, const CThemeChannel& x)
{
    if (n != 0) {
        if ((unsigned int)(v->_M_end_of_storage - v->_M_finish) >= n) {
            CThemeChannel x_copy = x;
            const unsigned int elems_after = (unsigned int)(v->_M_finish - position);
            CThemeChannel* old_finish = v->_M_finish;
            if (elems_after > n) {
                UninitCopy(v->_M_finish - n, v->_M_finish, v->_M_finish);
                v->_M_finish += n;
                CopyBackward(position, old_finish - n, old_finish);
                Fill(position, position + n, x_copy);
            } else {
                CThemeChannel* cur = v->_M_finish;
                for (unsigned int k = n - elems_after; k > 0; --k, ++cur) *cur = x_copy;
                v->_M_finish += n - elems_after;
                UninitCopy(position, old_finish, v->_M_finish);
                v->_M_finish += elems_after;
                Fill(position, old_finish, x_copy);
            }
        } else
            InsertOverflow(v, position, x, n);
    }
}

static CThemeChannel S(float r) { CThemeChannel s; for (int k = 0; k < 24; ++k) s.Rest[k] = r + k; s.Center.X = r; s.Center.Y = 2 * r; s.Center.Z = 3 * r; s.Radius = r; return s; }

int main()
{
    int fail = 0;
    ChannelVector v;
    v._M_start = (CThemeChannel*)std::malloc(10 * sizeof(CThemeChannel));
    v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 10; v.overflowCalls = 0;
    for (int i = 0; i < 5; ++i) *v._M_finish++ = S((float)i);          // 0 1 2 3 4
    FillInsert(&v, v._M_start + 3, 1, S(9.0f));                          // elems_after 2 > 1: 0 1 2 9 3 4
    float want1[] = { 0, 1, 2, 9, 3, 4 };
    if (v._M_finish - v._M_start != 6) fail = 1;
    for (int i = 0; i < 6 && !fail; ++i) if (v._M_start[i].Radius != want1[i] || v._M_start[i].Center.Z != 3 * want1[i]) fail = 1;
    FillInsert(&v, v._M_start + 5, 3, S(7.0f));                          // elems_after 1 <= 3: 0 1 2 9 3 7 7 7 4
    float want2[] = { 0, 1, 2, 9, 3, 7, 7, 7, 4 };
    if (v._M_finish - v._M_start != 9) fail = 1;
    for (int i = 0; i < 9 && !fail; ++i) if (v._M_start[i].Radius != want2[i] || v._M_start[i].Rest[23] != want2[i] + 23) fail = 1;
    FillInsert(&v, v._M_start, 0, S(5.0f));                              // n == 0: no change
    if (v._M_finish - v._M_start != 9 || v.overflowCalls != 0) fail = 1;
    FillInsert(&v, v._M_start, 2, S(5.0f));                              // no room: overflow path
    if (v.overflowCalls != 1 || v._M_finish - v._M_start != 11 || v._M_start[0].Radius != 5 || v._M_start[2].Radius != 0) fail = 1;
    std::free(v._M_start);
    if (fail) { std::printf("00b4a2c0_TEST FAIL\n"); return 1; }
    std::printf("00b4a2c0_TEST PASS\n");
    return 0;
}