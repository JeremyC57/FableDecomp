// Behaviour test for 0x00A01290 vector<CGraphicInfo>::_M_fill_insert (STLport fill insert, non-POD element).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct Frame {                                   // stands in for CGraphicInfo (counted copies)
    unsigned int FrameNum;
    Frame(unsigned int n) : FrameNum(n) { ++g_live; }
    Frame(const Frame& o) : FrameNum(o.FrameNum) { ++g_live; }
    Frame& operator=(const Frame& o) { FrameNum = o.FrameNum; return *this; }
    virtual ~Frame() { --g_live; }
};

static Frame* UninitCopy(Frame* first, Frame* last, Frame* result)
{
    for (; first != last; ++first, ++result) new (result) Frame(*first);
    return result;
}
static Frame* UninitFillN(Frame* first, unsigned int n, const Frame& x)
{
    for (; n > 0; --n, ++first) new (first) Frame(x);
    return first;
}
static Frame* CopyBackward(Frame* first, Frame* last, Frame* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void Fill(Frame* first, Frame* last, const Frame& v) { for (; first != last; ++first) *first = v; }

struct Vec {
    Frame* _M_start; Frame* _M_finish; Frame* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(Frame* pos, const Frame& x, unsigned int fill_len)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Frame* new_start = (Frame*)std::malloc(len * sizeof(Frame));
        Frame* new_finish = UninitCopy(_M_start, pos, new_start);
        new_finish = UninitFillN(new_finish, fill_len, x);
        new_finish = UninitCopy(pos, _M_finish, new_finish);
        for (Frame* d = _M_start; d != _M_finish; ++d) d->~Frame();
        std::free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
    void _M_fill_insert(Frame* pos, unsigned int n, const Frame& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                Frame x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - pos);
                Frame* old_finish = _M_finish;
                if (elems_after > n) {
                    UninitCopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    CopyBackward(pos, old_finish - n, old_finish);
                    Fill(pos, pos + n, x_copy);
                } else {
                    UninitFillN(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    UninitCopy(pos, old_finish, _M_finish);
                    _M_finish += elems_after;
                    Fill(pos, old_finish, x_copy);
                }
            } else
                _M_insert_overflow(pos, x, n);
        }
    }
};

static int Check(const Vec& v, const unsigned int* want, unsigned int n, unsigned int cap)
{
    if (v.size() != n || (unsigned int)(v._M_end_of_storage - v._M_start) != cap) return 0;
    for (unsigned int i = 0; i < n; ++i) if (v._M_start[i].FrameNum != want[i]) return 0;
    return 1;
}
static void Make(Vec& v, unsigned int n, unsigned int cap)
{
    v._M_start = (Frame*)std::malloc(cap * sizeof(Frame));
    for (unsigned int i = 0; i < n; ++i) new (v._M_start + i) Frame(i + 1);
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
}
static void Free(Vec& v) { for (Frame* d = v._M_start; d != v._M_finish; ++d) d->~Frame(); std::free(v._M_start); }

int main()
{
    int ok = 1;
    Frame x(9);
    Vec v;
    Make(v, 4, 8); v._M_fill_insert(v._M_start + 1, 2, x);          // elems_after 3 > 2
    { unsigned int w[] = { 1, 9, 9, 2, 3, 4 }; ok &= Check(v, w, 6, 8); } Free(v);
    Make(v, 4, 8); v._M_fill_insert(v._M_start + 3, 3, x);          // elems_after 1 <= 3
    { unsigned int w[] = { 1, 2, 3, 9, 9, 9, 4 }; ok &= Check(v, w, 7, 8); } Free(v);
    Make(v, 2, 2); v._M_fill_insert(v._M_start + 1, 3, x);          // overflow: 2 + max(2, 3)
    { unsigned int w[] = { 1, 9, 9, 9, 2 }; ok &= Check(v, w, 5, 5); }
    v._M_fill_insert(v._M_start, 0, x);                              // n == 0: untouched
    { unsigned int w[] = { 1, 9, 9, 9, 2 }; ok &= Check(v, w, 5, 5); } Free(v);
    if (g_live != 1) ok = 0;                                         // only x is alive
    if (!ok) { std::printf("00a01290_TEST FAIL\n"); return 1; }
    std::printf("00a01290_TEST PASS\n");
    return 0;
}