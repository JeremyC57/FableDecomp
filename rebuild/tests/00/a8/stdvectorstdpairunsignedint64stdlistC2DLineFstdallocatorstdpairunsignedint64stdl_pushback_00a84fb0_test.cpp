// Link test for 0x00A84FB0: the source object's vector<pair<unsigned __int64, list<C2DLineF> > >::push_back copy-
// constructs in place while there is room and calls _M_insert_overflow(finish, x, tag, 1, true) when full; the pair copy
// constructor (0x00A825D0) and _M_insert_overflow (0x00A84C20) are supplied here.
#include <cstdio>
#include <new>
class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
struct __false_type { __false_type() { } };        // user ctor: no zero-byte store for the tag temporary

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x08
    pair(const pair& __p);                         // 0x00A825D0 (the implicit copy, out of line)
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _Tp* _M_end_of_storage;                        // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00A84C20
    void push_back(const _Tp& __x);                // from the source object
};

typedef pair<unsigned __int64, list<C2DLineF> > _CSwitchableLineEntry;
static int g_copies = 0, g_overflows = 0;
static const void* g_overflowPos = 0;
static unsigned int g_fill = 0;
static bool g_atend = false;
template <> pair<unsigned __int64, list<C2DLineF> >::pair(const pair& __p) : first(__p.first), second(__p.second) { ++g_copies; }
template <> void vector<_CSwitchableLineEntry>::_M_insert_overflow(_CSwitchableLineEntry* __position, const _CSwitchableLineEntry&, const __false_type&, unsigned int __fill_len, bool __atend)
{ ++g_overflows; g_overflowPos = __position; g_fill = __fill_len; g_atend = __atend; }
}
typedef std::_CSwitchableLineEntry Entry;
int main()
{
    static int nodes[2];
    static unsigned char storage[2 * sizeof(Entry)];
    std::vector<Entry> v;
    v._M_start = v._M_finish = (Entry*)storage;
    v._M_end_of_storage = v._M_start + 2;
    unsigned char raw[sizeof(Entry)];
    Entry* x = (Entry*)raw;
    x->first = ((unsigned __int64)7 << 32) | 9; x->second._M_node = &nodes[0];
    v.push_back(*x);
    x->first = 5; x->second._M_node = &nodes[1];
    v.push_back(*x);
    int ok = sizeof(Entry) == 16 && std::g_copies == 2 && v._M_finish == v._M_start + 2 && std::g_overflows == 0;
    ok = ok && v._M_start[0].first == (((unsigned __int64)7 << 32) | 9) && v._M_start[0].second._M_node == &nodes[0] && v._M_start[1].first == 5;
    v.push_back(*x);
    ok = ok && std::g_overflows == 1 && std::g_overflowPos == v._M_start + 2 && std::g_fill == 1 && std::g_atend && std::g_copies == 2 && v._M_finish == v._M_start + 2;
    if (!ok) { std::printf("00a84fb0_TEST FAIL\n"); return 1; }
    std::printf("00a84fb0_TEST PASS\n");
    return 0;
}
