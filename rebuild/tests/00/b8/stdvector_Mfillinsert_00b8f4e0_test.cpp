// Behaviour test for std::vector<CEngineOcclusionManager::COccluderFlareEntry>::_M_fill_insert (0x00B8F4E0): the STLport
// 4.6 algorithm on a plain-array vector model -- n == 0 does nothing; with spare capacity the tail slides up inside the
// storage (elems_after > n: copy the last n into raw storage, copy_backward the middle, fill the hole; else: fill the raw
// part, move the tail after it, fill the old range); without capacity it grows through _M_insert_overflow (old + max(old, n)).
#include <cstdio>
#include <cstdlib>

class C3DVector { public: float X, Y, Z; };
struct COccluderFlareEntry { C3DVector Position; float Radius; unsigned int CircleSegments, MaxPixelCount, OccluderHandle; };
typedef COccluderFlareEntry T;

struct Vec { T* _M_start; T* _M_finish; T* _M_eos; };

static T* UninitCopy(T* first, T* last, T* result) { for (; first != last; ++first, ++result) *result = *first; return result; }
static T* UninitFillN(T* first, unsigned int n, const T& x) { for (; n > 0; --n, ++first) *first = x; return first; }
static T* CopyBackward(T* first, T* last, T* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
static void Fill(T* first, T* last, const T& x) { for (; first != last; ++first) *first = x; }
static void InsertOverflow(Vec& v, T* pos, const T& x, unsigned int fill_len)
{
    unsigned int old_size = (unsigned int)(v._M_finish - v._M_start);
    unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
    T* ns = (T*)std::malloc(len * sizeof(T));
    T* nf = UninitCopy(v._M_start, pos, ns);
    nf = UninitFillN(nf, fill_len, x);
    nf = UninitCopy(pos, v._M_finish, nf);
    std::free(v._M_start);
    v._M_start = ns; v._M_finish = nf; v._M_eos = ns + len;
}
static void FillInsert(Vec& v, T* position, unsigned int n, const T& x)
{
    if (n != 0) {
        if ((unsigned int)(v._M_eos - v._M_finish) >= n) {
            T x_copy = x;
            const unsigned int elems_after = (unsigned int)(v._M_finish - position);
            T* old_finish = v._M_finish;
            if (elems_after > n) {
                UninitCopy(v._M_finish - n, v._M_finish, v._M_finish);
                v._M_finish += n;
                CopyBackward(position, old_finish - n, old_finish);
                Fill(position, position + n, x_copy);
            } else {
                UninitFillN(v._M_finish, n - elems_after, x_copy);
                v._M_finish += n - elems_after;
                UninitCopy(position, old_finish, v._M_finish);
                v._M_finish += elems_after;
                Fill(position, old_finish, x_copy);
            }
        } else
            InsertOverflow(v, position, x, n);
    }
}

static T Make(unsigned int h) { T t; t.Position.X = (float)h; t.Position.Y = 0; t.Position.Z = 0; t.Radius = 1; t.CircleSegments = 8; t.MaxPixelCount = 2; t.OccluderHandle = h; return t; }
static Vec MakeVec(unsigned int count, unsigned int cap)
{
    Vec v; v._M_start = (T*)std::malloc(cap * sizeof(T)); v._M_finish = v._M_start; v._M_eos = v._M_start + cap;
    for (unsigned int i = 0; i < count; ++i) *v._M_finish++ = Make(i + 1);
    return v;
}
static int Check(const Vec& v, const unsigned int* want, unsigned int count)
{
    if ((unsigned int)(v._M_finish - v._M_start) != count) return 0;
    for (unsigned int i = 0; i < count; ++i) if (v._M_start[i].OccluderHandle != want[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    T x = Make(99);
    { Vec v = MakeVec(4, 10); FillInsert(v, v._M_start + 1, 2, x);          // elems_after 3 > n 2
      unsigned int w[] = { 1, 99, 99, 2, 3, 4 }; ok &= Check(v, w, 6); std::free(v._M_start); }
    { Vec v = MakeVec(4, 10); FillInsert(v, v._M_start + 3, 3, x);          // elems_after 1 <= n 3
      unsigned int w[] = { 1, 2, 3, 99, 99, 99, 4 }; ok &= Check(v, w, 7); std::free(v._M_start); }
    { Vec v = MakeVec(4, 5); FillInsert(v, v._M_start + 2, 3, x);           // no room: grow to 4 + max(4, 3) = 8
      unsigned int w[] = { 1, 2, 99, 99, 99, 3, 4 }; ok &= Check(v, w, 7); ok &= (v._M_eos - v._M_start) == 8; std::free(v._M_start); }
    { Vec v = MakeVec(2, 2); FillInsert(v, v._M_start, 0, x);              // n == 0: untouched
      unsigned int w[] = { 1, 2 }; ok &= Check(v, w, 2); std::free(v._M_start); }
    if (!ok) { std::printf("00b8f4e0_TEST FAIL\n"); return 1; }
    std::printf("00b8f4e0_TEST PASS\n");
    return 0;
}