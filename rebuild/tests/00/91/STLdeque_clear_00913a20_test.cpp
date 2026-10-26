// Behaviour test for 0x00913A20 deque<CBezierCurve>::clear: frees all buffers but the start one, finish = start.
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

static int g_mallocs = 0;
static int g_frees = 0;
static void* TMalloc(size_t n) { ++g_mallocs; return std::malloc(n); }
static void TFree(void* p) { ++g_frees; std::free(p); }

static int g_dtors = 0;
class C3DVector { public: float X, Y, Z; };
class CBezierCurve { public:
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
    Deque<CBezierCurve> d;
    d._M_initialize_map(7);                     // 4 node buffers
    int frees = g_frees;
    d.clear();
    if (g_frees - frees != 3) ok = 0;           // two interior + the finish buffer
    if (d._M_finish._M_cur != d._M_start._M_cur || d._M_finish._M_node != d._M_start._M_node) ok = 0;
    Deque<CBezierCurve> e;
    e._M_initialize_map(1);                     // a single buffer
    frees = g_frees;
    e.clear();
    if (g_frees != frees) ok = 0;
    if (e._M_finish._M_cur != e._M_start._M_cur) ok = 0;
    if (!ok) { std::printf("00913a20_TEST FAIL\n"); return 1; }
    std::printf("00913a20_TEST PASS\n");
    return 0;
}