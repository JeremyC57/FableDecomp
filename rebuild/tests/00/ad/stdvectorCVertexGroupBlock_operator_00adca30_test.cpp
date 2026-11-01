// Behaviour test for std::vector<CVertexGroupBlock>::operator= 0x00ADCA30: the three STLport assignment paths (reallocate,
// shrink-assign + destroy surplus, assign prefix + copy-construct tail) and self-assignment
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Block {                                   // stands in for CVertexGroupBlock (24 bytes in retail)
    int id;
    Block(int i) : id(i) { ++g_live; }
    Block(const Block& o) : id(o.id) { ++g_live; }
    ~Block() { --g_live; }
    Block& operator=(const Block& o) { id = o.id; return *this; }
};
struct Vec {
    Block* _M_start; Block* _M_finish; Block* _M_end_of_storage;
    Vec() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned size() const { return (unsigned)(_M_finish - _M_start); }
    unsigned capacity() const { return (unsigned)(_M_end_of_storage - _M_start); }
    static Block* ucopy(const Block* f, const Block* l, Block* r) { for (; f != l; ++f, ++r) new (r) Block(*f); return r; }
    static void destroy(Block* f, Block* l) { for (; f != l; ++f) f->~Block(); }
    static Block* copy(const Block* f, const Block* l, Block* r) { for (int n = int(l - f); n > 0; --n) *r++ = *f++; return r; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                Block* tmp = xlen ? (Block*)std::malloc(xlen * sizeof(Block)) : 0;
                ucopy(x._M_start, x._M_finish, tmp);
                destroy(_M_start, _M_finish);
                if (_M_start) std::free(_M_start);
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                Block* i = copy(x._M_start, x._M_finish, _M_start);
                destroy(i, _M_finish);
            } else {
                copy(x._M_start, x._M_start + size(), _M_start);
                ucopy(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
    void push(int id) { /* test helper: capacity is reserved up front */ new (_M_finish) Block(id); ++_M_finish; }
    void reserve(unsigned n) { _M_start = _M_finish = (Block*)std::malloc(n * sizeof(Block)); _M_end_of_storage = _M_start + n; }
    void release() { destroy(_M_start, _M_finish); std::free(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; }
};
static bool Same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned i = 0; i < a.size(); ++i) if (a._M_start[i].id != b._M_start[i].id) return false;
    return true;
}
int main()
{
    Vec src; src.reserve(5); for (int i = 1; i <= 5; ++i) src.push(i * 10);
    Vec a;                                       // empty: reallocate path
    a = src;
    if (!Same(a, src) || a.capacity() != 5 || g_live != 10) { std::printf("00adca30_TEST FAIL\n"); return 1; }
    Vec small; small.reserve(2); small.push(7); small.push(8);
    a = small;                                   // shrink: assign 2, destroy 3
    if (!Same(a, small) || a.capacity() != 5 || g_live != 9) { std::printf("00adca30_TEST FAIL\n"); return 1; }
    a = src;                                     // grow within capacity: assign 2, construct 3
    if (!Same(a, src) || g_live != 12) { std::printf("00adca30_TEST FAIL\n"); return 1; }
    a = a;                                       // self-assignment is a no-op
    if (!Same(a, src) || g_live != 12) { std::printf("00adca30_TEST FAIL\n"); return 1; }
    a.release(); small.release(); src.release();
    if (g_live != 0) { std::printf("00adca30_TEST FAIL\n"); return 1; }
    std::printf("00adca30_TEST PASS\n");
    return 0;
}