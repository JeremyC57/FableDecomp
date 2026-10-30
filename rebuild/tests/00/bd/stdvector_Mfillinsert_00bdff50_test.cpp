// Behaviour test for 0x00BDFF50 std::vector<C3DBoundingSphere>::_M_fill_insert (STLport 4.6 vector.c logic).
#include <cstdio>
#include <cstdlib>

class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };

struct SphereVector {
    C3DBoundingSphere* _M_start;
    C3DBoundingSphere* _M_finish;
    C3DBoundingSphere* _M_end_of_storage;
    int overflowCalls;
};

static C3DBoundingSphere* UninitCopy(C3DBoundingSphere* f, C3DBoundingSphere* l, C3DBoundingSphere* r)
{
    for (; f != l; ++f, ++r) *r = *f;
    return r;
}
static C3DBoundingSphere* CopyBackward(C3DBoundingSphere* f, C3DBoundingSphere* l, C3DBoundingSphere* r)
{
    for (int n = (int)(l - f); n > 0; --n) *--r = *--l;
    return r;
}
static void Fill(C3DBoundingSphere* f, C3DBoundingSphere* l, const C3DBoundingSphere& v)
{
    for (; f != l; ++f) *f = v;
}
static void InsertOverflow(SphereVector* v, C3DBoundingSphere* pos, const C3DBoundingSphere& x, unsigned int n)
{
    unsigned int size = (unsigned int)(v->_M_finish - v->_M_start);
    unsigned int len = size + (size > n ? size : n);
    C3DBoundingSphere* s = (C3DBoundingSphere*)std::malloc(len * sizeof(C3DBoundingSphere));
    C3DBoundingSphere* f = UninitCopy(v->_M_start, pos, s);
    for (unsigned int i = 0; i < n; ++i, ++f) *f = x;
    f = UninitCopy(pos, v->_M_finish, f);
    std::free(v->_M_start);
    v->_M_start = s; v->_M_finish = f; v->_M_end_of_storage = s + len;
    ++v->overflowCalls;
}

static void FillInsert(SphereVector* v, C3DBoundingSphere* position, unsigned int n, const C3DBoundingSphere& x)
{
    if (n != 0) {
        if ((unsigned int)(v->_M_end_of_storage - v->_M_finish) >= n) {
            C3DBoundingSphere x_copy = x;
            const unsigned int elems_after = (unsigned int)(v->_M_finish - position);
            C3DBoundingSphere* old_finish = v->_M_finish;
            if (elems_after > n) {
                UninitCopy(v->_M_finish - n, v->_M_finish, v->_M_finish);
                v->_M_finish += n;
                CopyBackward(position, old_finish - n, old_finish);
                Fill(position, position + n, x_copy);
            } else {
                C3DBoundingSphere* cur = v->_M_finish;
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

static C3DBoundingSphere S(float r) { C3DBoundingSphere s; s.Center.X = r; s.Center.Y = 2 * r; s.Center.Z = 3 * r; s.Radius = r; return s; }

int main()
{
    int fail = 0;
    SphereVector v;
    v._M_start = (C3DBoundingSphere*)std::malloc(10 * sizeof(C3DBoundingSphere));
    v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 10; v.overflowCalls = 0;
    for (int i = 0; i < 5; ++i) *v._M_finish++ = S((float)i);          // 0 1 2 3 4
    FillInsert(&v, v._M_start + 3, 1, S(9.0f));                          // elems_after 2 > 1: 0 1 2 9 3 4
    float want1[] = { 0, 1, 2, 9, 3, 4 };
    if (v._M_finish - v._M_start != 6) fail = 1;
    for (int i = 0; i < 6 && !fail; ++i) if (v._M_start[i].Radius != want1[i] || v._M_start[i].Center.Z != 3 * want1[i]) fail = 1;
    FillInsert(&v, v._M_start + 5, 3, S(7.0f));                          // elems_after 1 <= 3: 0 1 2 9 3 7 7 7 4
    float want2[] = { 0, 1, 2, 9, 3, 7, 7, 7, 4 };
    if (v._M_finish - v._M_start != 9) fail = 1;
    for (int i = 0; i < 9 && !fail; ++i) if (v._M_start[i].Radius != want2[i]) fail = 1;
    FillInsert(&v, v._M_start, 0, S(5.0f));                              // n == 0: no change
    if (v._M_finish - v._M_start != 9 || v.overflowCalls != 0) fail = 1;
    FillInsert(&v, v._M_start, 2, S(5.0f));                              // no room: overflow path
    if (v.overflowCalls != 1 || v._M_finish - v._M_start != 11 || v._M_start[0].Radius != 5 || v._M_start[2].Radius != 0) fail = 1;
    std::free(v._M_start);
    if (fail) { std::printf("00bdff50_TEST FAIL\n"); return 1; }
    std::printf("00bdff50_TEST PASS\n");
    return 0;
}