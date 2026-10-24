// Behaviour test for 0x00913930 __destroy_aux over deque iterators: visits every element from first to last across
// node buffers (two elements per buffer).
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

static int g_mallocs = 0;
static int g_frees = 0;
static void* TMalloc(size_t n) { ++g_mallocs; return std::malloc(n); }
static void TFree(void* p) { ++g_frees; std::free(p); }

static int g_dtors = 0;
struct C3DVector { float X, Y, Z; };
struct CBezierCurve {
    C3DVector Point1, Point2, Point3, Point4;
};
struct CCountedCurve {                          // test-only element: same 48-byte size, counts destructor calls
    float w[12];
    ~CCountedCurve() { ++g_dtors; }
};

template <class _Tp> struct DequeIter {
    enum { __buffer_size = 128 / sizeof(_Tp) };
    _Tp* _M_cur; _Tp* _M_first; _Tp* _M_last; _Tp** _M_node;
    DequeIter() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    void _M_set_node(_Tp** n) { _M_last = (_M_first = *(_M_node = n)) + (ptrdiff_t)__buffer_size; }
    void _M_increment() { if (++_M_cur == _M_last) { _M_set_node(_M_node + 1); _M_cur = _M_first; } }
};

template <class _Tp> struct Deque {
    DequeIter<_Tp> _M_start, _M_finish;
    _Tp** _M_map;
    size_t _M_map_size;
    static size_t buffer_size() { return (size_t)DequeIter<_Tp>::__buffer_size; }
    void _M_initialize_map(size_t n)
    {
        size_t nodes = n / buffer_size() + 1;
        size_t want = nodes + 2;
        _M_map_size = 8 < want ? want : 8;
        _M_map = _M_map_size != 0 ? (_Tp**)TMalloc(_M_map_size * sizeof(_Tp*)) : 0;
        _Tp** nstart = _M_map + (_M_map_size - nodes) / 2;
        _Tp** nfinish = nstart + nodes;
        for (_Tp** cur = nstart; cur < nfinish; ++cur)
            *cur = (_Tp*)TMalloc(buffer_size() * sizeof(_Tp));
        _M_start._M_set_node(nstart);
        _M_finish._M_set_node(nfinish - 1);
        _M_start._M_cur = _M_start._M_first;
        _M_finish._M_cur = _M_finish._M_first + n % buffer_size();
    }
    void clear()
    {
        for (_Tp** node = _M_start._M_node + 1; node < _M_finish._M_node; ++node)
            if (*node) TFree(*node);
        if (_M_start._M_node != _M_finish._M_node) {
            if (_M_finish._M_first) TFree(_M_finish._M_first);
        }
        _M_finish = _M_start;
    }
    void _M_push_back_aux_v(const _Tp& t)       // test stand-in for 0x00913D60 (map assumed large enough)
    {
        _Tp copy = t;
        *(_M_finish._M_node + 1) = (_Tp*)TMalloc(buffer_size() * sizeof(_Tp));
        new (_M_finish._M_cur) _Tp(copy);
        _M_finish._M_set_node(_M_finish._M_node + 1);
        _M_finish._M_cur = _M_finish._M_first;
    }
    void push_back(const _Tp& t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            new (_M_finish._M_cur) _Tp(t);
            ++_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(t);
    }
};

template <class _Tp> void DestroyAux(DequeIter<_Tp> first, DequeIter<_Tp> last)
{
    for (; first._M_cur != last._M_cur; first._M_increment())
        first._M_cur->~_Tp();
}

int main()
{
    int ok = 1;
    CCountedCurve* buffers[4];
    for (int i = 0; i < 4; ++i)
        buffers[i] = (CCountedCurve*)std::malloc(2 * sizeof(CCountedCurve));
    DequeIter<CCountedCurve> first, last;
    first._M_set_node(buffers + 0);
    first._M_cur = first._M_first + 1;          // second slot of buffer 0
    last._M_set_node(buffers + 2);
    last._M_cur = last._M_first + 1;            // second slot of buffer 2
    g_dtors = 0;
    DestroyAux(first, last);                    // 1 + 2 + 1 elements
    if (g_dtors != 4) ok = 0;
    g_dtors = 0;
    DestroyAux(first, first);
    if (g_dtors != 0) ok = 0;
    for (int i = 0; i < 4; ++i)
        std::free(buffers[i]);
    if (!ok) { std::printf("00913930_TEST FAIL\n"); return 1; }
    std::printf("00913930_TEST PASS\n");
    return 0;
}